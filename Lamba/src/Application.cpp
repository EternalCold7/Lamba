#include"Application.hpp"

Application::Application(Rx::observable<int> obs, Rx::observable < glm::vec<2, int> > mouse) :
	m_eventObserver(obs),m_mouseObservable(mouse),m_Camera(obs,mouse)
{
	MyObjLoader b;
	m_Models.push_back(
		std::make_unique<Model>
		("CarModel/Avent.obj", "shaders/light.shader", m_Camera,b));//neeed to create builder
	for (const auto & mesh : m_Models)
		m_Window.AddToDraw(mesh.get());
}