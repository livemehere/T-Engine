#include "IndexBuffer.h"

namespace Engine {
    IndexBuffer::IndexBuffer(const unsigned int *indices, const unsigned int count) {
        m_count = count;
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, indices, GL_STATIC_DRAW);
        LOG_INFO("[IndexBuffer] ctor (id: {}, count: {})", m_id, m_count);
    }

    IndexBuffer::~IndexBuffer() {
        LOG_INFO("[IndexBuffer] dtor (id: {}, count: {})", m_id, m_count);
        if (m_id != 0) {
            glDeleteBuffers(1, &m_id);
            LOG_INFO("[IndexBuffer] GPU free (id: {})", m_id);
        }
    }

    void IndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void IndexBuffer::UnBind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
