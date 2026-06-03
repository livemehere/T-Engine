#include "IndexBuffer.h"

Engine::IndexBuffer::IndexBuffer(const unsigned int *indices, const unsigned int count) {
    // save at GPU
    m_count = count;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, indices, GL_STATIC_DRAW);
}

Engine::IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &id);
}

void Engine::IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void Engine::IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
