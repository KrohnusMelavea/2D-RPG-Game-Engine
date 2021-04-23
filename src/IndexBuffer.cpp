#include "IndexBuffer.h"

#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* idxs, unsigned int count)
: m_RendererAddress(0), m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererAddress));
    Bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), idxs, GL_STATIC_DRAW));
}
IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererAddress));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererAddress));
}
void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}