#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_
#include<GL/glew.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
	x;\
	ASSERT(GLLogCall(#x,__FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char * function,const char * file, int line);

#endif // !_RENDERER_HPP_
