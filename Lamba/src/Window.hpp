#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_
#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include"Drawable.hpp"
#include<vector>
#define WIDTH 1000.f
#define HEIGHT 600.f
class Window {
	GLFWwindow* window;
	std::vector<Drawable*> m_DrawingObjects;
	void Draw();
public:
	inline GLFWwindow * GetGlWindow()  { return window; }

	Window();
	~Window();
	void AddToDraw(Drawable * object);
	
	void StartDisplay();
};

#endif // !_WINDOW_HPP_
