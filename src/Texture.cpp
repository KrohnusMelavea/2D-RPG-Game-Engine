#include "Texture.h"

#include "stb_image/stb_image.h"

static bool image_debug = false;


Texture::Texture(const std::string& filepath)
: m_RendererAddress(0), m_FilePath(filepath), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	uint64_t w = uint64_t(m_Width), h = uint64_t(m_Height);
	m_PixelData.resize(w * h * 4.0);

	GLCall(glGenTextures(1, &m_RendererAddress));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererAddress));

	//Stretch
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//No Stretch
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	Unbind();

	if (m_LocalBuffer)
	{
		std::cout << "Texture \"" << m_FilePath << "\" Loaded Successfully" << std::endl;
		m_PixelData = std::vector<unsigned char>(m_LocalBuffer, m_LocalBuffer + w * h * 4);

		uint64_t pos = 0;
		int r = 0, g = 0, b = 0, a = 0;

		if (image_debug)
		{	
			std::cout << "Image Contents:\n" << std::endl;
			for (unsigned int i = 0; i < m_Width; i++)
			{
				std::cout << "Row [" << i << "] : " << std::endl;
				for (unsigned int j = 0; j < m_Height; j++)
				{	

					pos = (j * w + i) * 4;
					r = static_cast<int>(m_PixelData[pos]);
					g = static_cast<int>(m_PixelData[pos] + 1);
					b = static_cast<int>(m_PixelData[pos] + 2);
					a = static_cast<int>(m_PixelData[pos] + 3);

					std::cout << "(" << r << "," << g << "," << b << "," << a << ") ";
				}
				std::cout << std::endl;
			}
		}
		stbi_image_free(m_LocalBuffer);
	}
	else
	{
		std::cout << "Image \"" << m_FilePath << "\" Loaded Unsuccessfully" << std::endl;
	}
	
}
Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererAddress));
}

void Texture::Bind(uint8_t slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererAddress));
}
void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}