#include "Window.hpp"
#include"Renderer.hpp"
Window::Window()
{
	if (!glfwInit())
		throw "GLFW NOT INITIALIZED";
	window = glfwCreateWindow(WIDTH, HEIGHT, "APWindow", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		throw "Window not initialized";
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit()) {
		glfwTerminate();
		throw "glfw not initialized";
	}
	glClearColor(0.4, 0.4, 0.7,1.0);
}

void Window::AddToDraw(Drawable * object)
{
	m_DrawingObjects.push_back(object);
}

void Window::Draw()
{
	for (auto & object : m_DrawingObjects)
		object->Draw();
}

void Window::StartDisplay()
{
	while (!glfwWindowShouldClose(window)) {
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		Draw();
		GLCall(glfwSwapBuffers(window));
		GLCall(glfwPollEvents());
	}
}

Window::~Window() {
	glfwTerminate();
}