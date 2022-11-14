#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Plane
{
public:
	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 C;
	glm::vec3 v;
	glm::vec3 u;

	Plane(glm::vec3 A, glm::vec3 B, glm::vec3 C);
	Plane(glm::vec3 A, glm::vec3 v, glm::vec3 u, bool ref);
	glm::vec3 Lerp(GLfloat s, GLfloat t);
	~Plane();
};

