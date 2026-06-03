#include "VertexBuffer.h"

Engine::VertexBuffer::VertexBuffer(float *vertices, unsigned int size) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

Engine::VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id);
}

void Engine::VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void Engine::VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

