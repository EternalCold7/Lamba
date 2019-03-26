#pragma once
#include<rxcpp/rx.hpp>
#include "Window.hpp"
#include "Camera.hpp"
#include "Mesh/Model.hpp"
#include <memory>
#include"Avent.hpp"

namespace Rx {
	using namespace rxcpp;
	using namespace rxcpp::subjects;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;
}

class Application {
private:	Window m_Window;
			Camera m_Camera;
			Avent m_Avent;
public:	Application(Rx::observable<int> obs, Rx::observable < glm::vec<2, int> > mouse);
		inline Window * GetWindow() { return &m_Window; }	void Start() { m_Window.StartDisplay(); }
		Rx::observable<int> m_eventObserver;
		Rx::observable < glm::vec<2, int> > m_mouseObservable;
};