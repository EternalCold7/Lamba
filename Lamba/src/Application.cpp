#include"Application.hpp"
#include "Mesh/Loaders/MyObjLoader.hpp"
Application::Application(Rx::observable<int> obs, Rx::observable < glm::vec<2, int> > mouse) :
	m_eventObserver(obs),m_mouseObservable(mouse),m_Camera(obs,mouse),m_Avent(m_Camera)
{

	m_Window.AddToDraw(&m_Avent);
	m_eventObserver.filter([](int val) {return val == GLFW_KEY_R; }).subscribe([this](int val) {
		m_Avent.Go();
	});
}