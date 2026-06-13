#pragma once

#include "../EngineCommon.h"
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

        unsigned int GetId() const { return m_id; }
        const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_indexBuffer; }
        const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_vertexBuffers; }

    private:
        unsigned int m_id = 0;
        // split VBO for dynamic update buffers
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
    };
}
