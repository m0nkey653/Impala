#pragma once
#include "handles.h"
#include "math.h"
#include "transform.h"
#include <algorithm>
#include <atomic>
#include <bit>
#include <bitset>
#include <cstdint>
#include <memory>
#include <type_traits>

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

        class IComponentHolder {
        public:
            virtual ~IComponentHolder() = default;
            virtual void* Get() = 0;
        };

        template <Component T> class ComponentHolder : public IComponentHolder {
        public:
            template <typename... Args>
            explicit ComponentHolder<T>(Args&&... args)
                : _component(std::make_unique<T>(std::forward<Args>(args)...)) {}
            virtual ~ComponentHolder() = default;

            void* Get() override { return static_cast<T*>(_component.get()); }

        private:
            std::unique_ptr<T> _component;
        };

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
            // dense array of entity IDs corresponding to the components in the paged pool
            // entity -> slot
            // sparse - sizeof(max entity ID)
            std::vector<uint32_t> _entityToSlot;
        };

    } // namespace detail

    class IScene {
    public:
        virtual ~IScene() = default;

        template <Component T> T& AddComponent(EntityHandle entity) {
            auto* componentHolder = new detail::ComponentHolder<T>();
            auto* componentPtr =
                AddComponentInternal(entity, detail::ComponentTypeId<T>(),
                                     std::unique_ptr<detail::IComponentHolder>(componentHolder));
            return *static_cast<T*>(componentPtr);
        }

        template <Component T> T& AddComponent(EntityHandle entity, T component) {
            auto* componentHolder = new detail::ComponentHolder<T>(std::move(component));
            auto* componentPtr =
                AddComponentInternal(entity, detail::ComponentTypeId<T>(),
                                     std::unique_ptr<detail::IComponentHolder>(componentHolder));
            return *static_cast<T*>(componentPtr);
        }

        template <typename T> T* GetComponent(EntityHandle entity) {
            auto* componentPtr = GetComponentInternal(entity, detail::ComponentTypeId<T>());
            return static_cast<T*>(componentPtr);
        }

        template <typename... Ts> std::vector<EntityHandle> GetEntitiesWithComponents() {

            std::bitset<64> required;
            ((required.set(detail::ComponentTypeId<Ts>())), ...);
            std::vector<EntityHandle> entities;
            GetEntitiesWithMaskInternal(required, entities);
            return entities;
        }

        virtual EntityHandle CreateEntity(const char* name) = 0;
        virtual EntityHandle CreateEntity(const char* name, math::Vec2 position,
                                          float rotation) = 0;
        virtual void TeleportEntity(EntityHandle entity, math::Vec2 position, float rotation) = 0;
        virtual void SetRotation(EntityHandle entity, float rotation) = 0;
        virtual void DestroyEntity(EntityHandle entity) = 0;
        virtual const Transform2d* GetWorldTransform(EntityHandle entity) = 0;

    protected:
        virtual void* AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                           std::unique_ptr<detail::IComponentHolder> component) = 0;
        virtual void* GetComponentInternal(EntityHandle entity, uint32_t componentTypeId) = 0;

        virtual void GetEntitiesWithMaskInternal(std::bitset<64> required,
                                                 std::vector<EntityHandle>& out) = 0;
    };

} // namespace impala
