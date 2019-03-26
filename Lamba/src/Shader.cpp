#include "Shader.h"
#include<GL/glew.h>
#include<fstream>
#include<sstream>
#include<iostream>
Shader::ShaderSource Shader::parse()
{
	enum class Mode { None = -1, Vertex, Fragment };

	std::fstream file(m_path);
	if (!file.is_open()) {
		throw "SHADER FILE NOT OPENED";
	}
	std::string line;
	std::stringstream ss[2];
	Mode currentMode = Mode::None;
	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				currentMode = Mode::Vertex;
			}
			else if (line.find("fragment") != std::string::npos) {
				currentMode = Mode::Fragment;
			}
		}
		else {
			if (currentMode == Mode::None)
				throw "NO SHADER SPECIFICATION";
			ss[(int)currentMode] << line << '\n';
		}
	}
	return { ss[(int)Mode::Vertex].str(),ss[(int)Mode::Fragment].str() };
}
void Shader::buildProgram() {
	Shader::ShaderSource src = parse();
	int vShader = compileShader(src.m_VertexSource, GL_VERTEX_SHADER);
	int fShader = compileShader(src.m_FragmentSource, GL_FRAGMENT_SHADER);

	glAttachShader(m_RendererID, vShader);
	glAttachShader(m_RendererID, fShader);
	glLinkProgram(m_RendererID);
	glValidateProgram(m_RendererID);
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	Bind();
}
unsigned int Shader::compileShader(const std::string & source, unsigned int type)
{
	unsigned int retValue = glCreateShader(type);
	const char * csource = source.c_str();
	glShaderSource(retValue, 1, &csource, nullptr);
	glCompileShader(retValue);

	int res;
	glGetShaderiv(retValue, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		int len;
		glGetShaderiv(retValue, GL_INFO_LOG_LENGTH, &len);
		char * msg = (char *)alloca(len * sizeof(char));
		glGetShaderInfoLog(retValue, len, &len, msg);
		std::cout << "FAILED TO COMPILE A SHADER" << std::endl;
		std::cout << msg << std::endl;
	}
	return retValue;
}
void Shader::Bind() const {
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
	glUseProgram(0);
}
Shader::~Shader() {
	if(m_RendererID != 0)
		glDeleteProgram(m_RendererID);
}

void Shader::SetUniformMat4(const std::string & name, const float * data) const
{
	auto loc = glGetUniformLocation(m_RendererID, name.c_str());
	if (loc < 0)
		throw;
	glUniformMatrix4fv(loc, 1, GL_FALSE, data);
}

void Shader::SetUniformui(const std::string & name, const uint32_t val) const
{
	auto loc = glGetUniformLocation(m_RendererID, name.c_str());
	if (loc < 0)
		throw;
	glUniform1ui(loc, val);
}

void Shader::SetUnifrom3f(const std::string & name, const float * val) const
{
	auto loc = glGetUniformLocation(m_RendererID, name.c_str());
	if (loc < 0)
		throw;
	glUniform3fv(loc, 1, val);
}

void Shader::SetUniformf(const std::string & name, const float val) const
{
	auto loc = glGetUniformLocation(m_RendererID, name.c_str());
	if (loc < 0)
		throw;
	glUniform1f(loc, val);
}


Shader::Shader(const std::string & vPath) : m_path(vPath), m_RendererID(0)
{
	m_RendererID = glCreateProgram();
	buildProgram();
}
Shader::Shader(Shader && sh) :m_RendererID(sh.m_RendererID),
	m_path(sh.m_path)
{
	sh.m_RendererID = 0;
}