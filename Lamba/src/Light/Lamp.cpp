#include "Lamp.hpp"
#include "../Mesh/Loaders/MyObjLoader.hpp"



Lamp::Lamp(const Camera * cam) : m_Camera(cam),m_Shader("shaders/lamp.shader")
{
	MyObjLoader loader;
	m_Model = std::make_unique<Model>("CarModel/", "Cube.obj", loader);
	
}
