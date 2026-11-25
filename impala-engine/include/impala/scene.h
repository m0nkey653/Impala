#pragma once
#include "handles.h"
#include "math.h"
#include "transform.h"
#include <algorithm>
#include <atomic>
#include <bit>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <vector>

namespace impala {

    template <typename T>
    concept Component =
        std::is_object_v<T>      // not a reference or function
        && !std::is_pointer_v<T> // no Add<Transform2D*>
        && !std::is_const_v<T> &&
        std::is_nothrow_move_constructible_v<T> // moving into the pool can't fail halfway
        && std::is_nothrow_move_assignable_v<T> // stage 1: slots are overwritten with =
        && std::is_default_constructible_v<T>   // stage 1: make_unique<T[]> builds every slot
        && std::is_destructible_v<T>;
    namespace detail {

        inline std::atomic<uint32_t> componentNextCounter = 0;
        template <Component T> uint32_t ComponentTypeId() {
            const static uint32_t ID = componentNextCounter.fetch_add(1, std::memory_order_relaxed);
            return ID;
        }

        class IComponentPool {
        public:
            virtual ~IComponentPool() = default;
            virtual void Remove(EntityHandle entity) = 0;
        };

        template <Component T> class ComponentPool final : public IComponentPool {
        public:
            T& Add(EntityHandle entity, T component) {
                if (Count() == _components.size() * PAGE_SIZE) {
                    _components.push_back(std::make_unique<T[]>(PAGE_SIZE));
                }
                if (entity.id >= _entityToSlot.size()) {
                    _entityToSlot.resize(entity.id + 1, INVALID_SLOT);
                }

                size_t slot = Count();
                size_t page = slot >> PAGE_SHIFT;
                size_t offset = slot & PAGE_MASK;
                _components[page][offset] = std::move(component);
                _slotToEntity.push_back(entity);
                _entityToSlot[entity.id] = static_cast<uint32_t>(slot);
                return _components[page][offset];
            }

            uint32_t FindSlot(EntityHandle entity) const {
                if (!entity.IsValid()) {
                    return INVALID_SLOT;
                }
                if (entity.id >= _entityToSlot.size()) {
                    return INVALID_SLOT;
                }
                auto slot = _entityToSlot[entity.id];
                if (slot == INVALID_SLOT) {
                    return INVALID_SLOT;
                }
                auto slotOwner = _slotToEntity[slot];
                assert(slotOwner.id == entity.id && "Entity mismatch in component pool");
                if (entity.generation != slotOwner.generation) {
                    return INVALID_SLOT;
                }
                return slot;
            }

            bool Contains(EntityHandle entity) const { return FindSlot(entity) != INVALID_SLOT; }

            T* Get(EntityHandle entity) {
                auto slot = FindSlot(entity);
                if (slot == INVALID_SLOT) {
                    return nullptr;
                }
                size_t page = slot >> PAGE_SHIFT;
                size_t offset = slot & PAGE_MASK;
                return &_components[page][offset];
            }

            void Remove(EntityHandle entity) override {
                if (!Contains(entity)) {
                    return;
                }
                size_t slot = _entityToSlot[entity.id];
                size_t lastSlot = Count() - 1;
                if (slot != lastSlot) {
                    size_t lastPage = lastSlot >> PAGE_SHIFT;
                    size_t lastOffset = lastSlot & PAGE_MASK;
                    _components[slot >> PAGE_SHIFT][slot & PAGE_MASK] =
                        std::move(_components[lastPage][lastOffset]);
                    EntityHandle lastEntity = _slotToEntity[lastSlot];
                    _slotToEntity[slot] = lastEntity;
                    _entityToSlot[lastEntity.id] = static_cast<uint32_t>(slot);
                }
                _components[lastSlot >> PAGE_SHIFT][lastSlot & PAGE_MASK] = T{};
                _slotToEntity.pop_back();
                _entityToSlot[entity.id] = INVALID_SLOT;
            }

        private:
            static constexpr uint32_t INVALID_SLOT = UINT32_MAX;
            size_t Count() const { return _slotToEntity.size(); }

            static constexpr size_t TARGET_PAGE_BYTES = 16 * 1024;
            static constexpr size_t PAGE_SIZE =
                std::clamp(std::bit_floor(TARGET_PAGE_BYTES / sizeof(T)), size_t{16}, size_t{4096});
            static constexpr size_t PAGE_SHIFT = std::countr_zero(PAGE_SIZE);
            static constexpr size_t PAGE_MASK = PAGE_SIZE - 1;

            // paged pool of components
            // slot -> T
            // dense - paged arrays of components
            std::vector<std::unique_ptr<T[]>> _components;
            // dense - contiguous array of slots -> entity
            std::vector<EntityHandle> _slotToEntity;
            // sparse array of entity IDs corresponding to the components in the paged pool
            // entity -> slot
            // sparse - sizeof(max entity ID)
            std::vector<uint32_t> _entityToSlot;
        };

    } // namespace detail

    class IScene {
    public:
        virtual ~IScene() = default;

        template <Component T> T& AddComponent(EntityHandle entity, T component) {
            assert(IsAlive(entity) && "AddComponent on a dead or invalid entity");
            auto& pool = GetOrCreatePool<T>();
            assert(!pool.Contains(entity) && "entity already has this component");
            T& added = pool.Add(entity, std::move(component));
            SetComponentBitInternal(entity, detail::ComponentTypeId<T>());
            OnComponentAddedInternal(entity, detail::ComponentTypeId<T>());
            return added;
        }

        template <Component T> T& GetOrAddComponent(EntityHandle entity, T component) {
            assert(IsAlive(entity) && "AddComponent on a dead or invalid entity");
            auto& pool = GetOrCreatePool<T>();
            if (pool.Contains(entity)) {
                return *pool.Get(entity);
            }
            T& added = pool.Add(entity, std::move(component));
            SetComponentBitInternal(entity, detail::ComponentTypeId<T>());
            OnComponentAddedInternal(entity, detail::ComponentTypeId<T>());
            return added;
        }

        template <Component T> T& AddComponent(EntityHandle entity) {
            return AddComponent<T>(entity, T{});
        }

        template <Component T> T* GetComponent(EntityHandle entity) {
            auto* pool = GetPool<T>();
            return pool ? pool->Get(entity) : nullptr;
        }

        template <typename... Ts> std::vector<EntityHandle> GetEntitiesWithComponents() {
            std::bitset<64> required;
            ((required.set(detail::ComponentTypeId<Ts>())), ...);
            std::vector<EntityHandle> entities;
            GetEntitiesWithMaskInternal(required, entities);
            return entities;
        }

        virtual EntityHandle CreateEntity(const char* name) = 0;
        virtual EntityHandle CreateEntity(const char* name, EntityHandle parent) = 0;
        virtual EntityHandle CreateEntity(const char* name, math::Vec2 position,
                                          float rotation) = 0;
        virtual EntityHandle CreateEntity(const char* name, math::Vec2 position, float rotation,
                                          EntityHandle parent) = 0;
        virtual void TeleportEntity(EntityHandle entity, math::Vec2 position, float rotation) = 0;
        virtual void SetRotation(EntityHandle entity, float rotation) = 0;
        virtual void DestroyEntity(EntityHandle entity) = 0;
        virtual const Transform2d* GetWorldTransform(EntityHandle entity) = 0;
        virtual bool IsAlive(EntityHandle entity) const = 0;

        virtual void LinkEntity(EntityHandle parent, EntityHandle child) = 0;
        virtual void UnlinkEntity(EntityHandle child) = 0;

    protected:
        virtual void OnComponentAddedInternal(EntityHandle entity, uint32_t typeId) = 0;
        virtual detail::IComponentPool* GetPoolInternal(uint32_t typeId) = 0;
        virtual detail::IComponentPool*
        AddPoolInternal(uint32_t typeId, std::unique_ptr<detail::IComponentPool> pool) = 0;

        virtual void SetComponentBitInternal(EntityHandle entity, uint32_t typeId) = 0;

        virtual void GetEntitiesWithMaskInternal(std::bitset<64> required,
                                                 std::vector<EntityHandle>& out) = 0;

    private:
        template <Component T> detail::ComponentPool<T>* GetPool() {
            return static_cast<detail::ComponentPool<T>*>(
                GetPoolInternal(detail::ComponentTypeId<T>()));
        }

        template <Component T> detail::ComponentPool<T>& GetOrCreatePool() {
            if (auto* pool = GetPool<T>()) {
                return *pool;
            }
            auto* created = AddPoolInternal(detail::ComponentTypeId<T>(),
                                            std::make_unique<detail::ComponentPool<T>>());
            return *static_cast<detail::ComponentPool<T>*>(created);
        }
    };

} // namespace impala
