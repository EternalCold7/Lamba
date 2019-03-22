#include "Model.hpp"
void Model::Draw() const noexcept
{

	for (const auto & mesh : m_Meshes)
		mesh.Draw();
}