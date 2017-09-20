#include "LightingApp.h"
#include <Mesh.h>
#include <Shader.h>
#include <Camera.h>
#include <glm/gtc/constants.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

Mesh* generateSphere(unsigned int segments, unsigned int rings,
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
			vertex->normal = glm::vec4(x0, y0, z0, 0);

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


	Mesh* sphere = new Mesh();
	std::vector<Vertex> verts;
	std::vector<unsigned int> indes;
	for (int i = 0; i < vertCount; i++)
	{
		verts.push_back(vertices[i]);
	}
	for (int i = 0; i < vertCount; i++)
	{
		indes.push_back(indices[i]);
	}
	sphere->initialize(verts, indes);
	return sphere;

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

void LightingApp::startup()
{
	m_camera = new Camera();
	m_camera->setLookAt(glm::vec3(10,10,10), glm::vec3(0), glm::vec3(0,1,0));
	m_camera->setPerspective(glm::pi<float>() / 4, 800/800, 0.1f, 10000.0f);

	m_shader = new Shader();
	m_shader->load("lighting.vert", GL_VERTEX_SHADER);
	m_shader->load("lighting.frag", GL_FRAGMENT_SHADER);
	m_shader->attach();

	unsigned int vao = 0, vbo = 0, ibo = 0, indexCount = 0;
	m_sphere = generateSphere(100, 100, vao, vbo, ibo, indexCount);
}

void LightingApp::shutdown()
{
}

glm::mat4 sphereMatrix = glm::mat4(1);
void LightingApp::update(float)
{
}

void LightingApp::draw()
{
	
	glm::mat4 view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	/*glm::pi<float>() / 4.0f, (int)(800/800), 0.1f, 10000.0f);*/
	
	auto aspectRatio = 800 / 800.0f;
	auto FOV = glm::pi<float>() / 4;
	auto farClip = 1000.0f;
	auto nearClip = 0.1f;


	float X = 1 / (aspectRatio * tanf(FOV / 2));
	float Y = 1 / (tanf(FOV / 2));
	float Z = -((farClip + nearClip) / (farClip - nearClip));
	float Zz = -(2 * (farClip * nearClip) / (farClip - nearClip));

	auto P = glm::mat4(
		glm::vec4(X, 0, 0, 0),
		glm::vec4(0, Y, 0, 0),
		glm::vec4(0, 0, Z, -1),
		glm::vec4(0, 0, Zz, 0));

	auto projView = P * view;

	//draw object
	m_shader->bind();
	auto projectionViewUniform = m_shader->getUniform("ProjectionViewWorld");
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(projView * sphereMatrix));
	m_sphere->draw(GL_TRIANGLES);
	m_shader->unbind();
}
