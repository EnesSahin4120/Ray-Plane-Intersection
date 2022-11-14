#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>

#include "Plane.h"

class Line
{
public:
	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 v;
	enum LINETYPE { LINE, SEGMENT, RAY };
	LINETYPE type;
	Line(glm::vec3 A, glm::vec3 B, LINETYPE type);
	Line(glm::vec3 A, glm::vec3 v);
	glm::vec3 Lerp(GLfloat t);
	GLfloat IntersectsAt(Plane p);
	void LinePointControlling(glm::vec3 &linePoint, bool* keys, GLfloat deltaTime);
	~Line();
};

