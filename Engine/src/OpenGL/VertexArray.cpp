#include "VertexArray.h"

namespace Engine {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
        if (m_id == 0) {
            throw std::runtime_error("[VertexArray] Failed to create VertexArray");
        }
        glBindVertexArray(m_id);
        LOG_INFO("[VertexArray] {} Created", m_id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1,&m_id);
        LOG_INFO("[VertexArray] {} Deleted", m_id);
    }

    void VertexArray::Bind() const {
        if (m_indexBuffer == nullptr) {
            throw std::runtime_error("[VertexArray] requires an IndexBuffer before binding");
        }
        glBindVertexArray(m_id);
    }

    void VertexArray::UnBind() const {
        if (m_indexBuffer == nullptr) {
            throw std::runtime_error("[VertexArray] requires an IndexBuffer before unbinding");
        }
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) {
        if (vertexBuffer == nullptr) {
            throw std::runtime_error("[VertexArray] Cannot add null VertexBuffer");
        }

        if (vertexBuffer->GetLayout().GetElements().empty()) {
            throw std::runtime_error("[VertexArray] VertexBuffer has empty elements");
        }

        glBindVertexArray(m_id);
        vertexBuffer->Bind();

        const auto layout = vertexBuffer->GetLayout();
        unsigned int attributeIndex = 0;
        for (const auto& vb : m_vertexBuffers) {
            attributeIndex += static_cast<unsigned int>(vb->GetLayout().GetElements().size());
        }

        for (const auto& element : layout.GetElements()) {
            glEnableVertexAttribArray(attributeIndex);
            if (element.type == ShaderDataType::Int ||element.type == ShaderDataType::Int2 || element.type == ShaderDataType::Int3 || element.type == ShaderDataType::Int4) {
                glVertexAttribIPointer(
                attributeIndex,
                GetShaderDataTypeCount(element.type),
                ShaderDataTypeToOpenGLBaseType(element.type),
                layout.GetStride(),
                reinterpret_cast<void *>(element.offset));
            }else {
                glVertexAttribPointer(
                attributeIndex,
                GetShaderDataTypeCount(element.type),
                ShaderDataTypeToOpenGLBaseType(element.type),
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<void *>(element.offset));
            }

            attributeIndex++;
        }

        m_vertexBuffers.push_back(std::move(vertexBuffer));
    }

    void VertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) {
        if (indexBuffer == nullptr) {
            throw std::runtime_error("[VertexArray] Cannot set null IndexBuffer");
        }

        glBindVertexArray(m_id);
        indexBuffer->Bind();
        m_indexBuffer = std::move(indexBuffer);
    }
}
