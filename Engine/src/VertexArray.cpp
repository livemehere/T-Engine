#include "VertexArray.h"

namespace Engine {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &m_id);
        glBindVertexArray(m_id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1,&m_id);
    }

    void VertexArray::Bind() const {
        glBindVertexArray(m_id);
        for (const auto& vb : m_vertexBuffers) {
            vb->Bind();
        }
        m_indexBuffer->Bind();
    }

    void VertexArray::UnBind() const {
        for (const auto& vb : m_vertexBuffers) {
            vb->Unbind();
        }
        m_indexBuffer->Unbind();
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
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
        glBindVertexArray(m_id);
        indexBuffer->Bind();

        m_indexBuffer = indexBuffer;
    }
}
