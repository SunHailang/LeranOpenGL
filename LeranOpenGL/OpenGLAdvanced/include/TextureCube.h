#pragma once

#include <vector>

class TextureCube
{
public:
	TextureCube(const std::vector<std::string> faces);
	~TextureCube();

	void Bind(unsigned int slot = 0) const;
	void Unbind();

private:
	unsigned int m_RendererID;


};