#include"Application.hpp"
#include "Mesh/Loaders/MyObjLoader.hpp"
Application::Application(Rx::observable<int> obs, Rx::observable < glm::vec<2, int> > mouse) :
	m_eventObserver(obs),m_mouseObservable(mouse),m_Camera(obs,mouse),m_Avent(m_Camera)
{
	//MyObjLoader b;
	//m_Models.push_back(
	//	std::make_unique<Model>
	//	("CarModel/Another/","Avent.obj", m_Camera,b));//neeed to create builder
	//for (const auto & mesh : m_Models)
	//	m_Window.AddToDraw(mesh.get());
	m_Window.AddToDraw(&m_Avent);
	m_eventObserver.subscribe([this](int val) {
		m_Avent.Go();
	});
}