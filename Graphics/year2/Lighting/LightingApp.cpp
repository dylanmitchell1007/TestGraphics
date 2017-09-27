#include <gl_core_4_4.h>
#include <glfw3.h>
#include "LightingApp.h"
#include <Mesh.h>
#include <Shader.h>
#include <Camera.h>
#include <assert.h>
#include <glm/gtc/constants.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

void generateSphere(unsigned int segments, unsigned int rings,
	unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
	unsigned int& indexCount) {

	unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

	// using AIEVertex for now, but could be any struct as long as it has the correct elements
	Vertex* vertices = new Vertex[vertCount];
	unsigned int* indices = new unsigned int[indexCount];

	float ringAngle = glm::pi<float>() / (rings + 1);
	float segmentAngle = 2.0f * glm::pi<float>() / segments;

	Vertex* vertex = vertices;

	for (unsigned int ring = 0; ring < (rings + 2); ++ring) {
		float r0 = glm::sin(ring * ringAngle);
		float y0 = glm::cos(ring * ringAngle);

		for (unsigned int segment = 0; segment < (segments + 1); ++segment, ++vertex) {
			float x0 = r0 * glm::sin(segment * segmentAngle);
			float z0 = r0 * glm::cos(segment * segmentAngle);

			vertex->position = glm::vec4(x0 * 0.5f, y0 * 0.5f, z0 * 0.5f, 1);
			vertex->normal = glm::vec4(x0, y0, z0, 1);

			//vertex->tangent = glm::vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);

			//// not a part of the AIEVertex, but this is how w generate bitangents
			//vertex->bitangent = glm::vec4(glm::cross(glm::vec3(vertex->normal), glm::vec3(vertex->tangent)), 0);

			//vertex->texcoord = glm::vec2(segment / (float)segments, ring / (float)(rings + 1));
		}
	}

	unsigned int index = 0;
	for (unsigned i = 0; i < (rings + 1); ++i) {
		for (unsigned j = 0; j < segments; ++j) {
			indices[index++] = i * (segments + 1) + j;
			indices[index++] = (i + 1) * (segments + 1) + j;
			indices[index++] = i * (segments + 1) + (j + 1);

			indices[index++] = (i + 1) * (segments + 1) + (j + 1);
			indices[index++] = i * (segments + 1) + (j + 1);
			indices[index++] = (i + 1) * (segments + 1) + j;
		}
	}

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &vao);

	// all changes will apply to this handle
	glBindVertexArray(vao);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	// texcoords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	// tangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3 + sizeof(glm::vec2)));

	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
	delete[] vertices;

}
unsigned int vao = 0, vbo = 0, ibo = 0, indexCount = 0;
void LightingApp::Ambient()
{
	m_ambient->load("ambient.vert", GL_VERTEX_SHADER);
	m_ambient->load("ambient.frag", GL_FRAGMENT_SHADER);

	m_ambient->attach();
}
void LightingApp::Diffuse()
{
	m_diffuse->load("Diffuse.vert", GL_VERTEX_SHADER);
	m_diffuse->load("Diffuse.frag", GL_FRAGMENT_SHADER);
	m_diffuse->attach();
}
void LightingApp::Specular()
{
	m_specular->load("specular.vert", GL_VERTEX_SHADER);
	m_specular->load("specular.frag", GL_FRAGMENT_SHADER);

	m_specular->attach();
}
void LightingApp::Phong()
{
	m_phong->load("phong.vert", GL_VERTEX_SHADER);
	m_phong->load("phong.frag", GL_FRAGMENT_SHADER);

	m_phong->attach();

}
void LightingApp::BlinPhong()
{
	m_blinphong->load("blinphong.vert", GL_VERTEX_SHADER);
	m_blinphong->load("blinphong.frag", GL_FRAGMENT_SHADER);

	m_blinphong->attach();
}




void LightingApp::ColorSphere()
{
	m_shader->load("lighting.vert", GL_VERTEX_SHADER);
	m_shader->load("lighting.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();
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

glm::mat4 world = glm::mat4(1);
glm::mat4 view = glm::mat4(1);
glm::mat4 proj = glm::mat4(1);
void LightingApp::startup()
{
	view = glm::lookAt(glm::vec3(15, 15, -30), glm::vec3(0), glm::vec3(0, 1, 0));
	proj = glm::perspective(glm::quarter_pi<float>(), 1.f, 3.f, 1000.0f);

	m_shader = new Shader();
	/*m_shader->load("lighting.vert", GL_VERTEX_SHADER);
	m_shader->load("lighting.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();*/
	m_ambient = new Shader();
	m_blinphong = new Shader();
	m_diffuse = new Shader();
	m_phong = new Shader();
	m_specular = new Shader();
	m_sphere = new Mesh();
	generateSphere(100, 100, m_sphere->m_vao, m_sphere->m_vbo, m_sphere->m_ibo, m_sphere->index_count);
	this->plane_Mesh = new Mesh();
	generatePlane(this->plane_Mesh);
	
	this->myshader = new Shader();
	this->myshader->load("lighting.vert", GL_VERTEX_SHADER);
	this->myshader->load("lighting.frag", GL_FRAGMENT_SHADER);
	this->myshader->attach();

	Specular();
	Diffuse();
	Ambient();
	Phong();
	BlinPhong();
}

void LightingApp::shutdown()
{
}


void LightingApp::update(float deltaTime)
{
	int amb = glfwGetKey(m_window, GLFW_KEY_KP_5);
	{
		if (amb == GLFW_PRESS)
			Ambient();
	}
	
	int diff = glfwGetKey(m_window, GLFW_KEY_KP_7);
	{
		if (diff == GLFW_PRESS)
		Diffuse();
	}

	int phong = glfwGetKey(m_window, GLFW_KEY_KP_9);
	{
		if (phong == GLFW_PRESS)
			Phong();
	}
	int spec = glfwGetKey(m_window, GLFW_KEY_KP_6);
	{
		if (spec == GLFW_PRESS)
			Specular();
	}
	int color = glfwGetKey(m_window, GLFW_KEY_KP_8);
	{
		if (color == GLFW_PRESS)
			ColorSphere();
	}


	if (glfwGetKey(m_window, GLFW_KEY_W))
	{
		auto t = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(0, 0, -1, 1)
		);

		world = world * t;
		view = glm::inverse(world);
	}

	if (glfwGetKey(m_window, GLFW_KEY_S))
	{
		auto t = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(0, 0, 1, 1)
		);

		world = world * t;
		view = glm::inverse(world);
	}

	if (glfwGetKey(m_window, GLFW_KEY_A))
	{
		auto t = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(1, 0, 0, 1)
		);

		world = world * t;
		view = glm::inverse(world);
	}

	if (glfwGetKey(m_window, GLFW_KEY_D))
	{
		auto t = glm::mat4(
			glm::vec4(1, 0, 0, 0),
			glm::vec4(0, 1, 0, 0),
			glm::vec4(0, 0, 1, 0),
			glm::vec4(-1, 0, 0, 1)
		);

		world = world * t;
		view = glm::inverse(world);
	}

	if (glfwGetKey(m_window, GLFW_KEY_SPACE))
	{
		view = lookAt(glm::vec3(view[3].x, view[3].y, view[3].z), glm::vec3(0), glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(m_window, GLFW_KEY_P))
	{
		view = glm::lookAt(glm::vec3(15, 15, -30), glm::vec3(0), glm::vec3(0, 1, 0));
	}
}

void LightingApp::draw()
{
	glm::mat4 sphereModel = glm::mat4(1);
	glm::mat4 planeModel = glm::mat4(1);
	glm::mat4 model = glm::mat4(1);
	
	unsigned int lightingUniform = 0;

	glm::vec3 lightColor = glm::vec3(1);
	glm::vec3 lightDirection = glm::vec3(0, 1, 0);
	glm::vec3 skyColor = glm::vec3(0.25, 0.25, 1);
	glm::vec3 groundColor = glm::vec3(0, 1, 0);
	glm::vec3 upVector = glm::vec3(0, 1, 0);
	auto mvp = proj * view * model;
	auto planeMVP = proj * view * planeModel;
	auto moveright = glm::translate(glm::vec3(5, 0, 0));
	auto moveleft = glm::translate(glm::vec3(-5, 0, 0));
	//Ambient
	m_ambient->bind();
	lightingUniform = m_ambient->getUniform("WVP"); //USE 'lightingVPUniform'
	auto ambientUpVecUniform = m_ambient->getUniform("upVector");
	auto ambientskyColorUniform = m_ambient->getUniform("skyColor");
	auto ambientgroundColorUniform = m_ambient->getUniform("groundColor");

	glUniform3fv(ambientUpVecUniform, 1, glm::value_ptr(upVector)); //SEND THE AMBIENT SHADER THE UP VECTOR
	glUniform3fv(ambientskyColorUniform, 1, glm::value_ptr(skyColor)); //SEND THE AMBIENT SHADER THE SKY COLOR
	glUniform3fv(ambientgroundColorUniform, 1, glm::value_ptr(groundColor)); //SEND THE AMBIENT SHADER THE GROUND COLOR
	glUniformMatrix4fv(lightingUniform, 1, GL_FALSE, glm::value_ptr(mvp * moveleft));
	m_sphere->draw(GL_TRIANGLES);
	plane_Mesh->draw(GL_TRIANGLES);
	m_ambient->unbind();

	////Diffuse
	m_diffuse->bind();	
	auto diffuseLightDirectionUniform = m_diffuse->getUniform("lightDirection"); //GET HANDLE FOR THE DIFFUSE SHADER UNIFORM 'lightDirection'
	auto diffuseLightColorUniform = m_diffuse->getUniform("lightColor"); //GET HANDLE FOR THE DIFFUSE SHADER UNIFORM 'lightColor'
	glUniform3fv(diffuseLightDirectionUniform, 1, glm::value_ptr(lightDirection)); //SEND THE DIFFUSE SHADER THE LIGHT'S DIRECTION
	glUniform3fv(diffuseLightColorUniform, 1, glm::value_ptr(lightColor)); //SEND THE DIFFUSE SHADER THE LIGHT'S COLOR
	lightingUniform = m_diffuse->getUniform("ProjectionViewWorld");
	glUniformMatrix4fv(lightingUniform, 1, GL_FALSE, glm::value_ptr(mvp * moveright));
	m_sphere->draw(GL_TRIANGLES);
	plane_Mesh->draw(GL_TRIANGLES);
	m_diffuse->unbind();


	//Phong
	m_phong->bind();
	lightingUniform = m_phong->getUniform("WVP");

	auto phongUpVecUniform = m_phong->getUniform("upVector");
	auto phongskyColorUniform = m_phong->getUniform("skyColor");
	auto phonggroundColorUniform = m_phong->getUniform("groundColor");

	auto phongLightDirectionUniform = m_phong->getUniform("lightDirection");
	auto phongLightColorUniform = m_phong->getUniform("lightColor");
	auto phongCameraPosUniform = m_phong->getUniform("cameraPosition");
	auto phongSpecularPowerUniform = m_phong->getUniform("specularPower");

	glUniform3fv(phongUpVecUniform, 1, glm::value_ptr(upVector)); //SEND THE PHONG SHADER THE UP VECTOR
	glUniform3fv(phongskyColorUniform, 1, glm::value_ptr(skyColor)); //SEND THE PHONG SHADER THE SKY COLOR
	glUniform3fv(phonggroundColorUniform, 1, glm::value_ptr(groundColor)); //SEND THE PHONG SHADER THE GROUND COLOR

	glUniform3fv(phongLightDirectionUniform, 1, glm::value_ptr(lightDirection)); // SEND THE PHONG SHADER THE LIGHTS DIRECTION
	glUniform3fv(phongLightColorUniform, 1, glm::value_ptr(lightColor)); // SEND THE PHONG SHADER THE LIGHTS COLOR
	glUniform3fv(phongCameraPosUniform, 1, glm::value_ptr(view[3])); // SEND THE PHONG SHADER THE CAMERA'S POSITION
	glUniform1f(phongSpecularPowerUniform, 128.0f); 
	glUniformMatrix4fv(lightingUniform, 1, GL_FALSE, glm::value_ptr(mvp ));
	m_sphere->draw(GL_TRIANGLES);
	plane_Mesh->draw(GL_TRIANGLES);
	m_phong->unbind();

	////BlinPhong
	m_blinphong->bind();
	lightingUniform = m_blinphong->getUniform("WVP");

	auto blinnUpVecUniform = m_blinphong->getUniform("upVector");
	auto blinnskyColorUniform = m_blinphong->getUniform("skyColor");
	auto blinngroundColorUniform = m_blinphong->getUniform("groundColor");

	auto blinnLightDirectionUniform = m_blinphong->getUniform("lightDirection");
	auto blinnLightColorUniform = m_blinphong->getUniform("lightColor");
	auto blinnCameraPosUniform = m_blinphong->getUniform("cameraPosition");
	auto blinnSpecularPowerUniform = m_blinphong->getUniform("specularPower");

	glUniform3fv(blinnUpVecUniform, 1, glm::value_ptr(upVector)); //SEND THE PHONG SHADER THE UP VECTOR
	glUniform3fv(blinnskyColorUniform, 1, glm::value_ptr(skyColor)); //SEND THE PHONG SHADER THE SKY COLOR
	glUniform3fv(blinngroundColorUniform, 1, glm::value_ptr(groundColor)); //SEND THE PHONG SHADER THE GROUND COLOR

	glUniform3fv(blinnLightDirectionUniform, 1, glm::value_ptr(lightDirection)); // SEND THE PHONG SHADER THE LIGHTS DIRECTION
	glUniform3fv(blinnLightColorUniform, 1, glm::value_ptr(lightColor)); // SEND THE PHONG SHADER THE LIGHTS COLOR
	glUniform3fv(blinnCameraPosUniform, 1, glm::value_ptr(view[3])); // SEND THE PHONG SHADER THE CAMERA'S POSITION
	glUniform1f(blinnSpecularPowerUniform, 128.0f); // SEND THE PHONG SHADER A VALUE FOR THE SPECULAR POWER
	glUniformMatrix4fv(lightingUniform, 1, GL_FALSE, glm::value_ptr(mvp * moveright* moveright));
	m_sphere->draw(GL_TRIANGLES);
	plane_Mesh->draw(GL_TRIANGLES);
	m_blinphong->unbind();

	////specular
	m_specular->bind();
	lightingUniform = m_specular->getUniform("WVP");
	auto specularLightDirectionUniform = m_specular->getUniform("lightDirection"); //GET HANDLE FOR THE SPECULAR SHADER UNIFORM 'lightDirection'
	auto specularLightColorUniform = m_specular->getUniform("lightColor"); //GET HANDLE FOR THE SPECULAR SHADER UNIFORM 'lightColor'
	auto specularCameraPosUniform = m_specular->getUniform("cameraPosition"); //GET HANDLE FOR THE SPECULAR SHADER UNIFORM 'cameraPosition'
	auto specularPowerUniform = m_specular->getUniform("specularPower"); //GET HANDLE FOR THE SPECULAR SHADER UNIFORM 'specularPower'

	glUniform3fv(specularLightDirectionUniform, 1, glm::value_ptr(lightDirection)); //SEND THE SPECULAR SHADER THE LIGHT'S DIRECTION
	glUniform3fv(specularLightColorUniform, 1, glm::value_ptr(lightColor)); //SEND THE SPECULAR SHADER THE LIGHT'S COLOR
	glUniform3fv(specularCameraPosUniform, 1, glm::value_ptr(view[3])); //SEND THE SPECULAR SHADER THE CAMERA'S POSITION
	glUniform1f(specularPowerUniform, 128.0f); //SEND THE SPECULAR SHADER A VALUE FOR 'specularPower'
	glUniformMatrix4fv(lightingUniform, 1, GL_FALSE, glm::value_ptr(mvp * moveright* moveright));
	m_sphere->draw(GL_TRIANGLES);
	plane_Mesh->draw(GL_TRIANGLES);
	m_specular->unbind();

	////draw sphere
	//m_shader->bind();
	//auto projectionViewUniform = m_shader->getUniform("ProjectionViewWorld");
	//glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(mvp));
	//m_sphere->draw(GL_TRIANGLES);
	//m_shader->unbind();

	//
	//myshader->bind();
	//auto planeProjectionViewUniform = myshader->getUniform("ProjectionViewWorld");
	//glUniformMatrix4fv(planeProjectionViewUniform, 1, GL_FALSE, glm::value_ptr(planeMVP));
	//plane_Mesh->draw(GL_TRIANGLES);
	//myshader->unbind();
}
