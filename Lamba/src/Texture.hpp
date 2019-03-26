#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_
#include<string>
#include"Renderer.hpp"
class Texture {

public:
	Texture(const std::string & image_path);
	Texture(const Texture & tex) = delete;
	Texture(Texture && tex);
	~Texture();
	void Bind();
	std::string GetFileExtension(const std::string & filename) const;
	std::tuple<int, int> GetTextureFileAttribs(const std::string & extension) const;
	inline unsigned int GetTextureDescriptor() const { return m_TextureDescriptor; }
private:
	unsigned int m_TextureDescriptor;
	void SetUpParams() const;
};

#endif //_TEXTURE_HPP_