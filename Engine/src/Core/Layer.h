#pragma once

#include <string>

namespace Engine {
    class Layer {
    public:
        Layer(std::string name) : m_name(std::move(name)) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnRender(){}

        const std::string& GetName() { return m_name; }
    protected:
        std::string m_name;
    };
}

