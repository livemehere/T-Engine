#include "IndexBuffer.h"

namespace Engine {
    IndexBuffer::IndexBuffer(const unsigned int *indices, const unsigned int count) {
        m_count = count;
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, indices, GL_STATIC_DRAW);
        LOG_INFO("[IndexBuffer] ctor (id: {}, count: {})", id, m_count);
    }

    IndexBuffer::~IndexBuffer() {
        LOG_INFO("[IndexBuffer] dtor (id: {}, count: {})", id, m_count);
        if (id != 0) {
            glDeleteBuffers(1, &id);
            LOG_INFO("[IndexBuffer] GPU free (id: {})", id);
        }
    }

    void IndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void IndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
