
#include"Application.hpp"
#include<glm/gtc/matrix_transform.hpp>
#include<rxcpp/rx.hpp>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow*, double x, double y);

Rx::subject<int> input_subject;
Rx::subject < glm::vec<2, int> > mouse_subject;

int main() {

	Application app(input_subject.get_observable(),mouse_subject.get_observable());
	glfwSetKeyCallback(app.GetWindow()->GetGlWindow(), key_callback);
	glfwSetCursorPosCallback(app.GetWindow()->GetGlWindow(), mouse_callback);
	app.m_eventObserver = input_subject.get_observable();
	input_subject.get_observable().subscribe([](auto val) { std::cout << val << '\n';}, []() {});
	app.Start();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
		input_subject.get_subscriber().on_next(key);
}
void mouse_callback(GLFWwindow*, double x, double y) {
	static float lastX = WIDTH / 2.f;
	static float lastY = HEIGHT / 2.f;
	static bool isFirstTime = true;

	if (isFirstTime) {
		lastX = x;
		lastY = y;
		isFirstTime = false;
	}
	float xOffset = x - lastX;
	float yOffset = lastY - y;

	lastX = x;
	lastY = y;

	mouse_subject.get_subscriber().on_next(glm::vec<2,int>( xOffset,yOffset));
}