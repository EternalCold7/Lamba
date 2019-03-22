#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_
#include<string>
#include"Renderer.hpp"
class Texture {

public:
	Texture(const std::string & image_path);
	void Bind();
	inline unsigned int GetTextureDescriptor() const { return m_TextureDescriptor; }
private:
	static unsigned short int BindedTexturesAmount;
	unsigned int m_TextureDescriptor;
	void SetUpParams() const;
	unsigned short int m_TextureNumber;
};

#endif //_TEXTURE_HPP_