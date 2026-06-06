#include "VertexBuffer.h"

namespace Engine {
    VertexBuffer::VertexBuffer(const float *vertices, unsigned int size) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        LOG_INFO("[VertexBuffer] ctor (id: {}, size: {}bytes)", id, size);
    }

    VertexBuffer::~VertexBuffer() {
        LOG_INFO("[VertexBuffer] dtor (id: {})", id);
        if (id != 0) {
            glDeleteBuffers(1, &id);
            LOG_INFO("[VertexBuffer] GPU free (id: {})", id);
        }
    }

    void VertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
