#include "VertexBuffer.h"

#include "Renderer.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
: m_RendererAddress(0)
{
    GLCall(glGenBuffers(1, &m_RendererAddress));           //generate buffer and assign its address to the supplied address
    Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));    //adds raw data to the buffer 
}
VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererAddress));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererAddress));
}
void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}