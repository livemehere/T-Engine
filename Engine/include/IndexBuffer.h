#pragma once

#include "EngineCommon.h"

namespace Engine {
    class IndexBuffer {
    public:
        IndexBuffer(const unsigned int* indices, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;

        unsigned int Getcount() const { return m_count; }
    private:
        unsigned int id = 0;
        unsigned int m_count = 0;
    };
}
