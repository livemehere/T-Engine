#pragma once

namespace Engine {
    class IndexBuffer {
    public:
        IndexBuffer(const GLuint* indices, GLuint count);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        GLuint GetId() const { return m_id; }
        void Bind() const;
        void UnBind() const;

        GLuint GetCount() const { return m_count; }
    private:
        GLuint m_id = 0;
        GLuint m_count = 0;
    };
}
