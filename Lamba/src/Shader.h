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
	Shader(const Shader & sh) = delete;
	Shader(Shader && sh);
	inline unsigned int getRendererID() const { return m_RendererID; }
	void SetUniformMat4(const std::string & name, const float * data)const;
	void SetUniformMat3(const std::string & name, const float * data)const;
	void SetUniformui(const std::string & name, const uint32_t val) const;
	void SetUniformi(const std::string & name, const int32_t val) const;
	void SetUnifrom3f(const std::string & name, const float * val) const;
	void SetUnifrom4f(const std::string & name, const float * val) const;
	void SetUniformf(const std::string & name, const float val) const;
};
#endif //_SHADER_HPP_									