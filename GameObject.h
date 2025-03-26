#ifndef LIGHTNING3D_GAME_OBJECT_HH_
#define LIGHTNING3D_GAME_OBJECT_HH_

#include "Component.hh"
#include "Matrix.hh"
#include "Drawable.hh"
#include "Vector.hh"

#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <tuple>
#include <variant>

namespace Lightning3D {

    //template <typename TComponent>
    //class QueryComponentResult {
    //public:
    //    QueryComponentResult(std::initializer_list<TComponent *> init)
    //        : m_components()

    //private:
    //    std::array<TComponent *, 32u> components;
    //};

    class GameObject {
    public:
        GameObject(GameObject *parent = nullptr)
            : m_parent(parent) {

            AttachComponent(Transform{});
        }

        GameObject(const GameObject &other)
            : m_parent(other.m_parent)
            , m_components(other.m_components) {
            std::cout << "GameObject copy ctor" << std::endl;
        }

        virtual auto Update(void) -> void {

        }

        template<typename TComponent>
        auto AttachComponent(TComponent component) -> TComponent * {
            return AttachTaggedComponent(ComponentTag::Default, component);

        }

        template<typename TComponent, typename ...Args>
        auto AttachComponent(Args &&...args) -> TComponent * {
            return AttachComponent(TComponent{ std::forward<Args>(args)... });
        }

        template<typename TComponent>
        auto AttachTaggedComponent(uint64_t tag, TComponent component = TComponent()) -> TComponent * {
            if (std::find_if(m_components.cbegin(), m_components.cend(), [](const auto &v) { return std::holds_alternative<TComponent>(v.first); }) != m_components.cend()) {
                return nullptr;
            }

            return &std::get<TComponent>(m_components.emplace_back(component, tag).first);
        }

        template<typename TComponent>
        auto QueryComponent(uint64_t tag = ComponentTag::Any) -> TComponent * {
            for (auto &component : m_components) {
                if (auto result = std::get_if<TComponent>(&component.first); result != nullptr && component.second & tag) {
                    return result;
                }
            }

            return nullptr;
        }

        template<typename ...TComponents>
        auto QueryComponents(uint64_t tags = ComponentTag::Any) -> std::tuple<std::add_pointer_t<TComponents>...> {
            std::tuple<std::add_pointer_t<TComponents>...> result{ std::add_pointer_t<TComponents>{nullptr}... };

            std::apply([this, tags](auto &...arg) {
                ((arg = QueryComponent<std::remove_pointer_t<std::decay_t<decltype(arg)>>>(tags)), ...);
                       }, result);


            return result;
        }

        auto SetParent(GameObject *parent) -> void {
            m_parent = parent;
        }

        auto GetParent(void) -> GameObject * {
            return m_parent;
        }

    private:
        GameObject *m_parent;

        std::deque<Component> m_components;
    };
}

#endif

