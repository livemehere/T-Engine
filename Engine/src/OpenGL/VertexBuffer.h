#pragma once

namespace Engine {

    struct RectVertex;

    enum class ShaderDataType {
        Bool,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4
    };

    inline GLuint GetShaderDataTypeSize(const ShaderDataType& type) {
        switch (type) {
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::Float: return sizeof(float);
            case ShaderDataType::Float2: return sizeof(float) * 2;
            case ShaderDataType::Float3: return sizeof(float) * 3;
            case ShaderDataType::Float4: return sizeof(float) * 4;
            case ShaderDataType::Int: return sizeof(int);
            case ShaderDataType::Int2: return sizeof(int) * 2;
            case ShaderDataType::Int3: return sizeof(int) * 3;
            case ShaderDataType::Int4: return sizeof(int) * 4;
        }
        throw std::runtime_error("Invalid ShaderDataType");
    }

    inline GLuint GetShaderDataTypeCount(const ShaderDataType& type){
        switch (type) {
            case ShaderDataType::Bool: return 1;
            case ShaderDataType::Float: return 1;
            case ShaderDataType::Float2: return 2;
            case ShaderDataType::Float3: return 3;
            case ShaderDataType::Float4: return 4;
            case ShaderDataType::Int: return 1;
            case ShaderDataType::Int2: return 2;
            case ShaderDataType::Int3: return 3;
            case ShaderDataType::Int4: return 4;
        }
        throw std::runtime_error("Invalid ShaderDataType");
    }

    inline GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType& type) {
        switch (type) {
            case ShaderDataType::Bool: return GL_BOOL;
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
        }
        throw std::runtime_error("Invalid ShaderDataType");
    }

    struct BufferElement {
        std::string name;
        ShaderDataType type;
        bool normalized;

        // auto clac
        GLuint size;
        GLuint offset;

        BufferElement(ShaderDataType type, std::string name,  bool normalized = false) :
            type(type), name(std::move(name)), size(GetShaderDataTypeSize(type)), offset(0), normalized(normalized)
        {}
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement> elements) : m_elements(elements) {
            CalcOffsetAndStride();
        }
        ~BufferLayout() = default;

        GLuint GetStride() const { return m_stride; }
        const std::vector<BufferElement>& GetElements() const { return m_elements;}
    private:
        std::vector<BufferElement> m_elements;
        GLuint m_stride = 0;
        void CalcOffsetAndStride() {
            GLuint offset = 0;
            m_stride = 0;
            for (auto& element : m_elements) {
                element.offset = offset;
                offset+= element.size;
                m_stride += element.size;
            }
        }
    };

    class VertexBuffer {
    public:
        VertexBuffer(const RectVertex* vertices, GLuint size);
        ~VertexBuffer();

        GLuint GetId() const { return m_id; }
        void Bind() const;
        void UnBind() const;

        const BufferLayout& GetLayout() const { return m_layout; }
        void SetLayout(const BufferLayout& layout) { m_layout = layout; }

        void SetData(const void* data, GLuint size);
    private:
        GLuint m_id = 0;
        BufferLayout m_layout;
    };
}
