#include "IndexBuffer.h"

namespace Engine {
    IndexBuffer::IndexBuffer(const GLuint *indices, const GLuint count) {
        if (indices == nullptr) {
            throw std::runtime_error("IndexBuffer data cannot be null");
        }

        if (count == 0) {
            throw std::runtime_error("IndexBuffer count must be greater than zero");
        }

        m_count = count;
        glGenBuffers(1, &m_id);
        if (m_id == 0) {
            throw std::runtime_error("Failed to create IndexBuffer");
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * count, indices, GL_STATIC_DRAW);
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
