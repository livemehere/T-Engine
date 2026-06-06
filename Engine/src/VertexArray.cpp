#include "VertexArray.h"

namespace Engine {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
        if (m_id == 0) {
            throw std::runtime_error("Failed to create VertexArray");
        }
        glBindVertexArray(m_id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1,&m_id);
    }

    void VertexArray::Bind() const {
        if (m_indexBuffer == nullptr) {
            throw std::runtime_error("VertexArray requires an IndexBuffer before binding");
        }

        glBindVertexArray(m_id);
        for (const auto& vb : m_vertexBuffers) {
            vb->Bind();
        }
        m_indexBuffer->Bind();
    }

    void VertexArray::UnBind() const {
        if (m_indexBuffer == nullptr) {
            throw std::runtime_error("VertexArray requires an IndexBuffer before unbinding");
        }

        for (const auto& vb : m_vertexBuffers) {
            vb->UnBind();
        }
        m_indexBuffer->UnBind();
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
        if (vertexBuffer == nullptr) {
            throw std::runtime_error("Cannot add null VertexBuffer");
        }

        if (vertexBuffer->GetLayout().GetElements().empty()) {
            throw std::runtime_error("VertexBuffer has empty elements");
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
            glVertexAttribPointer(
                attributeIndex,
                element.GetComponentCount(),
                GL_FLOAT,
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<void *>(element.offset));
            attributeIndex++;
        }

        m_vertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
        if (indexBuffer == nullptr) {
            throw std::runtime_error("Cannot set null IndexBuffer");
        }

        glBindVertexArray(m_id);
        indexBuffer->Bind();

        m_indexBuffer = indexBuffer;
    }
}
