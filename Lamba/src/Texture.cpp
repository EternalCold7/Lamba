#include"Texture.hpp"
#include"stb_image/stb_image.hpp"
#include<iostream>

unsigned short int Texture::BindedTexturesAmount = 0;
Texture::Texture(const std::string & image_path): m_TextureDescriptor(0), m_TextureNumber(0xff){
	

	stbi_set_unpremultiply_on_load(1);
	glGenTextures(1, &m_TextureDescriptor);
	glBindTexture(GL_TEXTURE_2D, m_TextureDescriptor);
	
	SetUpParams();

	int width, height, nrChannels;
	unsigned char *data = stbi_load(image_path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
	Bind();

	
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
	
	if (m_TextureNumber == 0xff) {
		glActiveTexture(GL_TEXTURE0 + BindedTexturesAmount);
		m_TextureNumber = BindedTexturesAmount;
		BindedTexturesAmount++;
	}
	else {
		glActiveTexture(m_TextureNumber);
	}
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureDescriptor));
	
}