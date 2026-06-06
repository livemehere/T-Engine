#pragma once

#include "EngineCommon.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Engine {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void UnBind() const;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer); // 이건왜 & 로받는가?
        void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() { return m_indexBuffer; }
        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() { return m_vertexBuffers; }

    private:
        unsigned int id;
        unsigned int m_nextAttributeIndex = 0;
        // split VBO for dynamic update buffers
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
    };
}
