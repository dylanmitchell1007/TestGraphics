#include "RenderingGeometryApp.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <vector>
#include <vec4.hpp>
#include <Camera.h>
#include <vec3.hpp>
#include <ext.hpp>
#include "Mesh.h"
#include "Shader.h"
#include <mat4x4.hpp>
RenderingGeometryApp::RenderingGeometryApp()
{
};
RenderingGeometryApp::~RenderingGeometryApp()
{

}

std::vector<glm::vec3> halfcircle(float radius, unsigned int points)
{
	std::vector<glm::vec3> tmpArray;

	for (int i = 0; i < points; i++)
	{
		auto slice = 3.14 / (points - 1);
		auto angle = slice * i;
		glm::vec3 vertex = glm::vec3(cosf(angle), sinf(angle), 0);
		tmpArray.push_back(vertex);
	}
	return tmpArray;
}
Mesh* generateCube()
{
	Mesh* cube = new Mesh();
	//front Bottom Left
	Vertex j = { glm::vec4(0.0f, 0.0f, 0, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	//front Bottom Right
	Vertex k = { glm::vec4(1.0f, 0.0f, 0, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	//front Top Right
	Vertex l = { glm::vec4(1.0f, 1.0f, 0, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	//front Top Left
	Vertex m = { glm::vec4(0.0f, 1.0f, 0, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	//Back Bottom Left
	Vertex n = { glm::vec4(0, 0.0f, 1, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	//Back Bottom Right
	Vertex o = { glm::vec4(1.0f, 0.0f, 1, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	//Back Top Right
	Vertex p = { glm::vec4(1.0f, 1.0f, 1, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };
	//Back Top Left
	Vertex q = { glm::vec4(0.0f, 1.0f, 1, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) };

	std::vector<Vertex> Cubevert = { j, k, l, m, n, o, p, q };
	std::vector<unsigned int> Cubeindices = {
		0,  1,  3,  3,  1,  2,   //frontface
		4,  5,  7, 7,  5,  6,  //backface
		4, 0, 7, 7, 0, 3,  //leftface
		1,  5,  2,  2, 5,  6, //rightface
		3, 2, 7, 7, 2, 6,  //topface
		0, 1, 4, 4, 1, 5 }; //bottom face
	cube->initialize(Cubevert, Cubeindices);

	return cube;
}
Mesh* generatePlane()
{
	Mesh* plane = new Mesh();
	//bottomleft
	Vertex f = { glm::vec4(0, 0, 0, 1)		, glm::vec4(0.5, 0, 0, .5) };
	//bottomright
	Vertex g = { glm::vec4(5,  0, 0, 1)		, glm::vec4(0.5, 0, 0, .5) };
	//topright
	Vertex h = { glm::vec4(5,  0, 5, 1)		, glm::vec4(0.5, 0, 0, .5) };
	//topleft
	Vertex i = { glm::vec4(0,  0, 5, 1)		, glm::vec4(0.5, 0, 0, .5) };

	//Plane Vertex
	std::vector<Vertex> myPlane = { f, g, h, i };
	//Plane Indices
	std::vector<unsigned int> planeIndices = { 0,1,3,3,2,1 };
	plane->initialize(myPlane, planeIndices);
	return plane;
}
//Mesh* generateSphere()
//{
//
//
//}





void RenderingGeometryApp::startup()
{

	const glm::vec3 eye = glm::vec3(10, 10, 10);//camera in world is at positive 10
	const glm::vec3 target = glm::vec3(0);
	const glm::vec3 up = glm::vec3(0, 1, 0);

	m_camera = new Camera();
	/*m_camera->setPerspective(m_fov, m_aspectRatio, m_near, m_far);*/
	m_camera->setLookAt(eye, target, up);


	Vertex a = { glm::vec4(-0.5,  0, 0, 1)		, glm::vec4(1, 0, 0, .5) };
	Vertex b = { glm::vec4(0.5,  0, 0, 1)			, glm::vec4(0, 1, 0, .5) };
	Vertex c = { glm::vec4(0.5, -0.5, 0, 1)			, glm::vec4(0, 0, 1, .5) };
	Vertex d = { glm::vec4(-0.5, -0.5, 0, 1)		, glm::vec4(1, 0, 0, .5) };
	Vertex e = { glm::vec4(-0.5,  0.5, 0, 1)		, glm::vec4(0, 1, 0, .5) };



	//Face of Cube



	//Cube Indices



	std::vector<Vertex> myObject = { a, b, c, d, e };

	std::vector<unsigned int> objectIndices = { 0, 1, 2, 0, 2, 3, 0, 4, 1 };


	//create vertex array
	//this->object = new Mesh();
	object = generateCube();
	this->newobject = generatePlane();
	/*this->object = new Mesh();
	this->object->initialize(myObject, objectIndices);*/
	//this->object = new Mesh();
	//this->object->initialize(myPlane, planeIndices);
	//create buffers
	this->object->create_buffers();
	//buffer data

	//setup vertex attributes

	//build shader
	this->myshader = new Shader();
	this->myshader->defaultLoad();
}

void RenderingGeometryApp::update(float delta_time)
{
	if (glfwGetKey(_window, 'W') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] -= m_camera->getWorldTransform()[1]);
	}
	if (glfwGetKey(_window, 'D') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] += m_camera->getWorldTransform()[0]);
	}
	if (glfwGetKey(_window, 'A') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] -= m_camera->getWorldTransform()[0]);
	}
	if (glfwGetKey(_window, 'Q') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] += m_camera->getWorldTransform()[1]);
	}
	if (glfwGetKey(_window, 'E') == GLFW_PRESS)
	{
		m_camera->setPosition(m_camera->getWorldTransform()[3] -= m_camera->getWorldTransform()[1]);
	}
	/*if (glfwGetKey(_window, 'F') == GLFW_PRESS)
	{
	m_camera->setLookAt(m_camera->setPosition(), vec3(0, 0, 0), vec3(0, 1, 0));*/
	//m_runningTime += delta_time;
	//deltaTime = delta_time;

	//(perspective) ?
	//	m_camera->setPerspective(m_fov, m_aspectRatio, m_near, m_far) :
	//	m_camera->setOrtho(m_left, m_right, m_bottom, m_top, m_near, m_far);

	m_camera->update(delta_time);



}
glm::mat4 aPlane = glm::mat4(2);
glm::mat4 aObject = glm::mat4(1);
void RenderingGeometryApp::draw()
{
	//draw object
	this->myshader->bind();

	auto translation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(10, 10, 10, 1)

	);
	//aObject = aObject * translation;
	unsigned int projectionViewUniform = this->myshader->getUniform("ProjectionViewWorld");
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionView() * aObject));
	this->object->draw(GL_TRIANGLES);


	unsigned int newprojectionViewUniform = this->myshader->getUniform("ProjectionViewWorld");
	glUniformMatrix4fv(newprojectionViewUniform, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionView() * aPlane));
	this->newobject->draw(GL_TRIANGLES);
	this->myshader->unbind();


}

void RenderingGeometryApp::shutdown()
{
}