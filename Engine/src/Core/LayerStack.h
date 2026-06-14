#pragma once

#include "Layer.h"

namespace Engine {
    class LayerStack {
    public:
        LayerStack() = default;
        ~LayerStack();
        template<typename TLayer, typename ... Args>
        requires(std::is_base_of_v<Layer, TLayer>)
        TLayer* PushLayer(Args&&... args) {
            auto layer = std::make_unique<TLayer>(std::forward<Args>(args)...);
            auto layerPtr = layer.get();

            layer->OnAttach();
            m_layers.emplace(m_layers.begin() + m_layerInsertIndex, std::move(layer));
            m_layerInsertIndex++;

            return layerPtr;
        }

        template<typename TLayer, typename ... Args>
        requires(std::is_base_of_v<Layer, TLayer>)
        TLayer* PushOverlay(Args&&... args) {
            auto layer = std::make_unique<TLayer>(std::forward<Args>(args)...);
            auto layerPtr = layer.get();

            layer->OnAttach();
            m_layers.emplace_back(std::move(layer));

            return layerPtr;
        }

        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);
        void PlushPendingCommands();
        void Clear();

        auto begin() const { return m_layers.begin(); }
        auto end() const { return m_layers.end(); }
        auto rbegin() const { return m_layers.rbegin(); }
        auto rend() const { return m_layers.rend(); }
    private:
        enum class CommandType {
            PopLayer,
            PopOverlay
        };
        struct PendingCommand {
            CommandType type;
            Layer* layer;
        };
        std::vector<std::unique_ptr<Layer>> m_layers;
        std::vector<PendingCommand> m_pendingCommands;
        size_t m_layerInsertIndex = 0;
    };
}
