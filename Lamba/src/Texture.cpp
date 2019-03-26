#include"Texture.hpp"
#include"stb_image/stb_image.hpp"
#include<iostream>
#define TJE_IMPLEMENTATION
#include"Mesh/Loaders/jpeg_loader.hpp"

#include<sstream>
Texture::Texture(const std::string & image_path): m_TextureDescriptor(0) {
	

	stbi_set_unpremultiply_on_load(1);
	glGenTextures(1, &m_TextureDescriptor);
	glBindTexture(GL_TEXTURE_2D, m_TextureDescriptor);
	
	SetUpParams();
	auto[source, dest] = GetTextureFileAttribs(GetFileExtension(image_path));
	int width, height, nrChannels;
	unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, source, width, height, 0, dest, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		throw std::runtime_error("tetxture not loaded");
	}
	stbi_image_free(data);
	
	Bind();

	
}
Texture::Texture(Texture && tex) : m_TextureDescriptor(tex.m_TextureDescriptor)
{
	tex.m_TextureDescriptor = 0;
}
Texture::~Texture()
{
	if (m_TextureDescriptor != 0)
		glDeleteTextures(1, &m_TextureDescriptor);
}
void Texture::SetUpParams() const {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
} 

void Texture::Bind() {
	
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureDescriptor));
	
}

std::string Texture::GetFileExtension(const std::string & filename) const
{
	std::istringstream iss(filename);
	std::string result;
	while (std::getline(iss, result, '.'));

	std::getline(iss, result);
	return result;
}

std::tuple<int, int> Texture::GetTextureFileAttribs(const std::string & extension) const 
{
	int source;
	int dest;
	if (extension == "tga") {
		source = GL_RGB8;
		dest = GL_RGB8;
	}
	else if (extension == "jpeg")
	{
		source = GL_RGB;
		dest = GL_RGB;
	}
	else {
		source = GL_RGBA;
		dest = GL_RGBA;
	}
	return std::tuple<int, int>(source,dest);
}
