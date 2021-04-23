#pragma once

#include "Renderer.h"
#include <string>

class Texture
{
private:
	unsigned int m_RendererAddress;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	std::vector<unsigned char> m_PixelData;
public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(uint8_t slot = 0) const;
	void Unbind() const;

	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
	inline int GetBPP() { return m_BPP; }
};