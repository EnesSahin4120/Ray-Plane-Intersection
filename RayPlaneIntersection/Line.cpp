#include "Line.h"

Line::Line(glm::vec3 A, glm::vec3 B, LINETYPE type)
{
	this->A = A;
	this->B = B;
	this->type = type;
	v = glm::vec3(B.x - A.x, B.y - A.y, B.z - A.z);
}

Line::Line(glm::vec3 A, glm::vec3 v)
{
	this->A = A;
	this->v = v;
	B = A + v;
	type = SEGMENT;
}

glm::vec3 Line::Lerp(GLfloat t) 
{
	if (type == SEGMENT)
		glm::clamp(t, 0.0f, 1.0f);
	else if (type == RAY&&t < 0.0f)
		t = 0.0f;

	GLfloat xt = A.x + v.x * t;
	GLfloat yt = A.y + v.y * t;
	GLfloat zt = A.z + v.z * t;

	return glm::vec3(xt, yt, zt);
}

GLfloat Line::IntersectsAt(Plane p) 
{
	glm::vec3 normal = glm::cross(p.u, p.v);
	if (glm::dot(normal, v) == 0)
		return NAN;

	GLfloat t = glm::dot(normal, p.A - A) / glm::dot(normal, v);
	return t;
}

void Line::LinePointControlling(glm::vec3 &linePoint, bool* keys, GLfloat deltaTime) 
{
	GLfloat speed = 10.0f;

	if (keys[GLFW_KEY_W])
		linePoint.z += deltaTime * speed;

	if (keys[GLFW_KEY_S])
		linePoint.z -= deltaTime * speed;

	if (keys[GLFW_KEY_A])
		linePoint.x += deltaTime * speed;

	if (keys[GLFW_KEY_D])
		linePoint.x -= deltaTime * speed;
}

Line::~Line()
{
}
