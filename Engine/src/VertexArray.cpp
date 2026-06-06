#include "VertexArray.h"

Engine::VertexArray::VertexArray() {
    glGenVertexArrays(1, &id);
    glBindVertexArray(id);
}

Engine::VertexArray::~VertexArray() {
    glDeleteVertexArrays(1,&id);
}

void Engine::VertexArray::Bind() const {
    for (auto& vb : m_vertexBuffers) {
        vb->Bind();
    }
    m_indexBuffer->Bind();
}

void Engine::VertexArray::UnBind() const {
    for (auto& vb : m_vertexBuffers) {
        vb->Unbind();
    }
    m_indexBuffer->Unbind();
}

void Engine::VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
    if (vertexBuffer->GetLayout().GetElements().empty()) {
        throw std::runtime_error("VertexBuffer has empty elements");
    }

    glBindVertexArray(id);
    vertexBuffer->Bind();

    const auto layout = vertexBuffer->GetLayout();
    unsigned int index = 0;
    for (auto& element : layout.GetElements()) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            element.GetComponentCount(),
            GL_FLOAT,
            element.normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            reinterpret_cast<void *>(element.offset));
        index++;
    }

    m_vertexBuffers.push_back(vertexBuffer);
}

void Engine::VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
    glBindVertexArray(id);
    indexBuffer->Bind();

    m_indexBuffer = indexBuffer;
}
