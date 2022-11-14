#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Line.h"
#include "DrawLine.h"
#include "Mesh.h"

const GLint WIDTH = 800, HEIGHT = 600;

Window mainWindow;
Shader *redShader;
Shader *greenShader;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader 
static const char* vShader = "Shaders/object.vert";

// Red Fragment Shader
static const char* red_fShader = "Shaders/object.frag";

// Green Fragment Shader
static const char* green_fShader = "Shaders/Green.frag";

// Line Parameters
glm::vec3 rayPoint_A = glm::vec3(0.0f, 2.0f, 0.0f);
glm::vec3 rayPoint_B = glm::vec3(0.0f, - 10.0f, 0.0f);

// Plane Parameters
glm::vec3 planePoint_A = glm::vec3(-4.0f, -5.0f, 4.0f);
glm::vec3 planePoint_B = glm::vec3(4.0f, -3.0f, 4.0f);
glm::vec3 planePoint_C = glm::vec3(-4.0f, -5.0f, -4.0f);

Line *ray = new Line(rayPoint_A, rayPoint_B, Line::SEGMENT);
Plane *plane = new Plane(planePoint_A, planePoint_B, planePoint_C);

DrawLine *drawLine = new DrawLine();
Mesh *planeMesh = new Mesh();
Mesh *cubeMesh = new Mesh();

GLfloat intersectsAt;
glm::vec3 intersectPoint;

void CreateShaders()
{
	redShader = new Shader();
	redShader->CreateFromFiles(vShader, red_fShader);

	greenShader = new Shader();
	greenShader->CreateFromFiles(vShader, green_fShader);
}

void CreateObjects() 
{
	unsigned int planeIndices[] = 
	{
		0, 2, 3,
		0, 3, 1
	};
	GLfloat planeVertices[] = 
	{
		plane->A.x, plane->A.y, plane->A.z,
		plane->B.x, plane->B.y, plane->B.z,
		plane->C.x, plane->C.y, plane->C.z,
		4.0f, -3.0f, -4.0f
	};

	unsigned int cubeIndices[] =
	{
		0, 1, 2,
		0, 2, 3,
		1, 0, 4,
		5, 1, 4,
		2, 1, 5,
		2, 5, 6,
		3, 2, 7,
		2, 6, 7,
		5, 4, 6,
		6, 4, 7,
		0, 3, 4,
		4, 3, 7
	};
	GLfloat cubeVertices[] =
	{
		0, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0,
		0, 0.5f, 0,
		0, 0, 0.5f,
		0.5f, 0, 0.5f,
		0.5f, 0, 0,
		0, 0, 0
	};

	planeMesh->CreateMesh(planeVertices, planeIndices, 12, 12);
	cubeMesh->CreateMesh(cubeVertices, cubeIndices, 36, 36);
}

int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 1.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 0.1f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.5f, 50.0f);	
	while (!mainWindow.GetShouldClose())
	{
		glLineWidth(10);

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();

		ray->LinePointControlling(ray->B, mainWindow.GetKeys(), deltaTime);
		camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		uniformModel = redShader->GetModelLocation();
		uniformProjection = redShader->GetProjectionLocation();
		uniformView = redShader->GetViewLocation();

		redShader->UseShader();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));

		ray = new Line(ray->A, ray->B, Line::SEGMENT);
		GLfloat vertices[] =
		{
			ray->A.x, ray->A.y, ray->A.z,
			ray->B.x, ray->B.y, ray->B.z
		};
		drawLine->CreateLine(vertices, 6);
		drawLine->RenderLine();

		intersectsAt = ray->IntersectsAt(*plane);
		intersectPoint = ray->Lerp(intersectsAt);

		model = glm::translate(model, intersectPoint + glm::vec3(-0.25f, -0.125f, -0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		cubeMesh->RenderMesh();

		greenShader->UseShader();
		model = glm::mat4(1);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		planeMesh->RenderMesh();

		glUseProgram(0);
		mainWindow.SwapBuffers();
	}
	return 0;
}