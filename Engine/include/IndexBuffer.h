#pragma once

#include "EngineCommon.h"

namespace Engine {
    class IndexBuffer {
    public:
        IndexBuffer(const unsigned int* indices, unsigned int count);
        ~IndexBuffer();

        unsigned int GetId() const { return m_id; }
        void Bind() const;
        void UnBind() const;

        unsigned int GetCount() const { return m_count; }
    private:
        unsigned int m_id = 0;
        unsigned int m_count = 0;
    };
}
