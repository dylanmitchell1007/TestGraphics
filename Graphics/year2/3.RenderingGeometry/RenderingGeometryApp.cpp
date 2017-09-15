
#include "RenderingGeometryApp.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <vector>
#include <Camera.h>
#include "Mesh.h"
#include "Shader.h"
#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <Gizmos.h>
 
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
void generateCube(Mesh* cube)
{	
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
}
void generatePlane(Mesh* plane)
{		
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
	
}
 //ToDo: Generate Sphere

void RenderingGeometryApp::startup()
{

	const glm::vec3 eye = glm::vec3(10, 10, -100);//camera in world is at positive 10
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
	cube_Mesh = new Mesh();
	generateCube(cube_Mesh);
	cube_Mesh->create_buffers();
	
	this->plane_Mesh = new Mesh();
	generatePlane(this->plane_Mesh);
	this->plane_Mesh->create_buffers();
	
	
	
	//create buffers
	
	//buffer data

	//setup vertex attributes

	//build shader
	this->myshader = new Shader();
	this->myshader->defaultLoad();
	Gizmos::create();
}

void RenderingGeometryApp::update(float delta_time)
{/*
	if (glfwGetKey(m_window, 'W') == GLFW_PRESS)
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
	}*/
	/*if (glfwGetKey(_window, 'F') == GLFW_PRESS)
	{
	m_camera->setLookAt(m_camera->setPosition(), vec3(0, 0, 0), vec3(0, 1, 0));*/
	//m_runningTime += delta_time;
	//deltaTime = delta_time;

	//(perspective) ?
	//	m_camera->setPerspective(m_fov, m_aspectRatio, m_near, m_far) :
	//	m_camera->setOrtho(m_left, m_right, m_bottom, m_top, m_near, m_far);
	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::Begin("hello");
	ImGui::End();
	m_camera->update(delta_time);
	
}

void RenderingGeometryApp::draw()
{
	//draw object
	this->myshader->bind();

	//aObject = aObject * translation;
	auto projectionViewUniform = this->myshader->getUniform("ProjectionViewWorld");
	auto plane_ModelMatrix = glm::mat4(1);
	
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(m_camera->projectionView));
	this->cube_Mesh->draw(GL_TRIANGLES);


	projectionViewUniform = this->myshader->getUniform("ProjectionViewWorld");
	auto cube_ModelMatrix = glm::mat4(1) * glm::translate(glm::vec3(5, 0, 0)) * glm::scale(glm::vec3(15));
	auto mvp = m_camera->projectionView * cube_ModelMatrix;
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(mvp));
	this->plane_Mesh->draw(GL_TRIANGLES);
	this->myshader->unbind();


}

void RenderingGeometryApp::shutdown()
{
}