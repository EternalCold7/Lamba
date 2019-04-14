#ifndef _TRANSFORMABLE_HPP_
#define _TRANSFORMABLE_HPP_
#include<glm/matrix.hpp>
#include<glm/gtc/matrix_transform.hpp>
class Transformable {
protected:
	glm::mat4 m_ModelMatrix = glm::mat4(1.f);
	const glm::mat4 * m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;

public:
	void Rotate(const glm::vec3& on, bool local = false) {
		glm::mat4 rotMatrix(1.f);
		rotMatrix = glm::rotate(rotMatrix, on.x, { 1,0,0 });
		rotMatrix = glm::rotate(rotMatrix, on.y, { 0,1,0 });
		rotMatrix = glm::rotate(rotMatrix, on.z, { 0,0,1 });
		m_ModelMatrix = local ? rotMatrix * m_ModelMatrix : m_ModelMatrix * rotMatrix;
	}
	void Translate(const glm::vec3& on) {
		m_ModelMatrix = glm::translate(m_ModelMatrix, on);
	}
	void Scale(const glm::vec3& on) {
		if (on.x < 0 || on.y < 0 || on.z < 0)
			throw "qwer";
		m_ModelMatrix = glm::scale(m_ModelMatrix, on);

	}

	void SetViewMatrix(const glm::mat4 * view) {
		m_ViewMatrix = view;
	}
	void SetProjectionMatrix(const glm::mat4 & projection) {
		m_ProjectionMatrix = projection;
	}
	glm::mat4 GetMVPMatrix() {
		return m_ProjectionMatrix * *m_ViewMatrix * m_ModelMatrix;
	}
};

#endif
