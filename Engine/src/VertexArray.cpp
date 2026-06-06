#include "VertexArray.h"

namespace Engine {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &id);
        glBindVertexArray(id);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1,&id);
    }

    void VertexArray::Bind() const {
        glBindVertexArray(id);
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

        glBindVertexArray(id);
        vertexBuffer->Bind();

        const auto layout = vertexBuffer->GetLayout();
        for (const auto& element : layout.GetElements()) {
            glEnableVertexAttribArray(m_nextAttributeIndex);
            glVertexAttribPointer(
                m_nextAttributeIndex,
                element.GetComponentCount(),
                GL_FLOAT,
                element.normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<void *>(element.offset));
            m_nextAttributeIndex++;
        }

        m_vertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
        glBindVertexArray(id);
        indexBuffer->Bind();

        m_indexBuffer = indexBuffer;
    }
}
