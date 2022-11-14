#pragma once

#include <GL\glew.h>

class DrawLine
{
public:
	DrawLine();

	void CreateLine(GLfloat *vertices, unsigned numofVertices);
	void RenderLine();
	void ClearLine();

	~DrawLine();

private:
	GLuint VAO, VBO;
};

