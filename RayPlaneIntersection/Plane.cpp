#include "Plane.h"



Plane::Plane(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
	this->A = A;
	this->B = B;
	this->C = C;
	v = B - A;
	u = C - A;
}

Plane::Plane(glm::vec3 A, glm::vec3 v, glm::vec3 u, bool ref) 
{
	this->A = A;
	this->v = glm::vec3(v.x, v.y, v.z);
	this->u = glm::vec3(u.x, u.y, u.z);
}

glm::vec3 Plane::Lerp(GLfloat s, GLfloat t) 
{
	GLfloat xst = A.x + v.x*s + u.x*t;
	GLfloat yst = A.y + v.y*s + u.y*t;
	GLfloat zst = A.z + v.z*s + u.z*t;

	return glm::vec3(xst, yst, zst);
}

Plane::~Plane()
{
}
