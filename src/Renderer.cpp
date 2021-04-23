#include "Renderer.h"


const void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
bool GLLogCall(const char* function, const char* file, unsigned int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " (" << line << ")" << std::endl;
		return false;
	}
	return true;
}

Renderer::Renderer()
: m_RendererAddress(0)
{

}
Renderer::~Renderer()
{

}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
{
	shader.Bind();
	vao.Bind();
	ibo.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, 0));
}
void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}