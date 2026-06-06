#pragma once

#include "EngineCommon.h"

namespace Engine {

    enum class ShaderDataType {
        Float, Float2, Float3, Float4
    };

    static unsigned int ShaderDataTypeSize(const ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float: return sizeof(float);
            case ShaderDataType::Float2: return sizeof(float) * 2;
            case ShaderDataType::Float3: return sizeof(float) * 3;
            case ShaderDataType::Float4: return sizeof(float) * 4;
        }
        return 0;
    }

    struct BufferElement {
        std::string name;
        ShaderDataType type;
        bool normalized;

        // auto clac
        unsigned int size;
        mutable unsigned int offset;

        BufferElement(ShaderDataType type, const std::string name, bool normalized = false) :
            type(type), name(name), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized)
        {}

        unsigned int GetComponentCount() const {
            switch (type) {
                case ShaderDataType::Float: return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
            }
            return 0;
        }
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement> elements) : m_elements(elements) {
            CalcOffsetAndStrid();
            LOG_INFO("Layout created");
        }
        ~BufferLayout() = default;

        unsigned int GetStride() const { return m_stride; }
        const std::vector<BufferElement>& GetElements() const { return m_elements;}
    private:
        std::vector<BufferElement> m_elements;
        unsigned int m_stride = 0;
        void CalcOffsetAndStrid() {
            unsigned int offset = 0;
            m_stride = 0;
            for (const auto& element : m_elements) {
                element.offset = offset;
                offset+= element.size;
                m_stride += element.size;
            }
        }
    };

    class VertexBuffer {
    public:
        VertexBuffer(float* vertices, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        const BufferLayout& GetLayout() const { return m_layout; }
        void SetLayout(const BufferLayout& layout) { m_layout = layout; }
    private:
        unsigned int id = 0;
        BufferLayout m_layout;
    };
}
