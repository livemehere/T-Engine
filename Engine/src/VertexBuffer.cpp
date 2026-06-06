#include "VertexBuffer.h"

namespace Engine {
    VertexBuffer::VertexBuffer(const float *vertices, unsigned int size) {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        LOG_INFO("[VertexBuffer] ctor (id: {}, size: {}bytes)", m_id, size);
    }

    VertexBuffer::~VertexBuffer() {
        LOG_INFO("[VertexBuffer] dtor (id: {})", m_id);
        if (m_id != 0) {
            glDeleteBuffers(1, &m_id);
            LOG_INFO("[VertexBuffer] GPU free (id: {})", m_id);
        }
    }

    void VertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
