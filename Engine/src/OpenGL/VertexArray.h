#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Engine {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        void Bind() const;
        void UnBind() const;

        void AddVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
        void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

        GLuint GetId() const { return m_id; }
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; }
        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_vertexBuffers; }

    private:
        GLuint m_id = 0;
        // split VBO for dynamic update buffers
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
    };
}
