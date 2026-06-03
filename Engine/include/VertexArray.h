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

    private:
        unsigned int id;
        // split VBO for dynamic update buffers
        std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
    };
}