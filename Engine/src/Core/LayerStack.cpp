#include "LayerStack.h"

#include <ranges>

namespace Engine {
    LayerStack::~LayerStack() {
       Clear();
    }

    void LayerStack::PopLayer(Layer *layer) {
        if (layer == nullptr) return;

        m_pendingCommands.push_back({
            CommandType::PopLayer,
            layer
        });
    }

    void LayerStack::PopOverlay(Layer *layer) {
        if (layer == nullptr) return;

        m_pendingCommands.push_back({
            CommandType::PopOverlay,
            layer
        });
    }

    void LayerStack::PlushPendingCommands() {
        for (const auto& command : m_pendingCommands) {
            auto it = std::ranges::find_if(m_layers, [&](const auto& unique_ptr) {
                return unique_ptr.get() == command.layer;
            });

            if (it == m_layers.end()) continue;

            if (command.type == CommandType::PopLayer) {
                --m_layerInsertIndex;
            }

            (*it)->OnDetach();
            m_layers.erase(it);
        }
        m_pendingCommands.clear();
    }

    void LayerStack::Clear() {
        for (const auto& layer : m_layers | std::ranges::views::reverse) {
            layer->OnDetach();
        }
        m_layers.clear();
        m_pendingCommands.clear();
        m_layerInsertIndex = 0;
    }
}
