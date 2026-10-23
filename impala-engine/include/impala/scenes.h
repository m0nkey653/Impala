#pragma once
#include "entities.h"

namespace impala {

    template <typename T>
    concept Component =
        std::is_object_v<T>                                       // not a reference or function
        && !std::is_pointer_v<T>                                  // no Add<Transform2D*>
        && !std::is_const_v<T> && std::is_move_constructible_v<T> // storage can move it
        && std::is_destructible_v<T>;
    namespace detail {

        inline std::atomic<uint32_t> componentNextCounter = 0;
        template <Component T> uint32_t ComponentTypeId() {
            const static uint32_t id = componentNextCounter.fetch_add(1, std::memory_order_relaxed);
            return id;
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

        template <typename T> T* GetComponent(EntityHandle entity) {
            auto* componentPtr = GetComponent(entity, detail::ComponentTypeId<T>());
            return static_cast<T*>(componentPtr);
        }

    protected:
        virtual void* AddComponentInternal(EntityHandle entity, uint32_t componentTypeId,
                                           std::unique_ptr<detail::IComponentHolder> component) = 0;
        virtual void* GetComponentInternal(EntityHandle entity, uint32_t componentTypeId) = 0;
    };

    class ISceneFactory {
    public:
        virtual ~ISceneFactory() = default;
        virtual IScene* CreateScene() = 0;
    };

} // namespace impala