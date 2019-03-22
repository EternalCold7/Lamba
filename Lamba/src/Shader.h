#ifndef _SHADER_HPP_
#define _SHADER_HPP_
#include<string>

class Shader {
private:
	struct ShaderSource {
		std::string m_VertexSource;
		std::string m_FragmentSource;
	};
	unsigned int m_RendererID;
	std::string m_path;
	std::string m_VertexShader;
	std::string m_FragmentShader;
	ShaderSource parse();
	void buildProgram();
	unsigned int compileShader(const std::string & source, unsigned int type);

public:
	void Bind() const;
	void Unbind() const;
	Shader(const std::string & vPath);
	~Shader();
	inline unsigned int getRendererID() const { return m_RendererID; }
	void SetUniformMat4(const std::string & name, const float * data)const;
};
#endif //_SHADER_HPP_									