#include <gl_core_4_4.h>
#include <glfw3.h>
#include <Mesh.h>
#include <Shader.h>
#include <Texture.h>
#include <Camera.h>
#include <assert.h>
#include <glm/gtc/constants.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "TextureApp.h"

TextureApp::TextureApp() {};
TextureApp::~TextureApp() {};
void generatePlane(Mesh* plane)
{
	//bottomleft                                   //Color                //Normal      //Texture
	Vertex f = { glm::vec4(0, 0, 0, 1)		, glm::vec4(0.5, 0, 0, .5), glm::vec4(0), glm::vec2(0,0) };
	//bottomright                                  //Color                //Normal      //Texture
	Vertex g = { glm::vec4(5,  0, 0, 1)		, glm::vec4(0.5, 0, 0, .5), glm::vec4(0), glm::vec2(1,0) };
	//topright                                    //Color                //Normal      //Texture 
	Vertex h = { glm::vec4(5,  0, 5, 1)		, glm::vec4(0.5, 0, 0, .5), glm::vec4(0), glm::vec2(1,1) };
	//topleft                                     //Color                //Normal      //Texture
	Vertex i = { glm::vec4(0,  0, 5, 1)		, glm::vec4(0.5, 0, 0, .5), glm::vec4(0), glm::vec2(0,1) };

	//Plane Vertex
	std::vector<Vertex> myPlane = { f, g, h, i };
	//Plane Indices
	std::vector<unsigned int> planeIndices = { 0,1,3,3,2,1 };
	plane->initialize(myPlane, planeIndices);

}
glm::mat4 world = glm::mat4(1);
glm::mat4 view = glm::mat4(1);
glm::mat4 proj = glm::mat4(1);

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

void TextureApp::startup()
{
	view = glm::lookAt(glm::vec3(0, 15, 30.5), glm::vec3(5, 0, 5), glm::vec3(0, 1, 0));
	proj = glm::perspective(glm::quarter_pi<float>(), 1.f, 3.f, 1000.0f);
	m_shader = new Shader();

	this->plane_Mesh = new Mesh();
	generatePlane(this->plane_Mesh);



	m_textureShader = new Shader();
	this->m_textureShader->load("texture.vert", GL_VERTEX_SHADER);
	this->m_textureShader->load("texture.frag", GL_FRAGMENT_SHADER);
	this->m_textureShader->attach();
	m_texture = new Texture();
	m_texture->load("..//bin//textures//water.jpg");

}

glm::mat4 model = glm::mat4(1);
void TextureApp::draw()
{
	glm::mat4 sphereModel = glm::mat4(1);
	glm::mat4 planeModel = glm::mat4(1) * glm::scale(glm::vec3(100));

	
	auto mvp = proj * view * model;
	auto planeMVP = proj * view * planeModel;
	auto moveright = glm::translate(glm::vec3(5, 0, 0));
	auto moveleft = glm::translate(glm::vec3(-5, 0, 0));


	m_textureShader->bind();
	m_texture->bind();
	auto textureWVPuniform = m_textureShader->getUniform("WVP");

	auto textureSampler2DUniform = m_textureShader->getUniform("diffuseTexture");

	glUniform1i(textureSampler2DUniform, 0);
	glUniformMatrix4fv(textureWVPuniform, 1, GL_FALSE, glm::value_ptr(mvp));
	plane_Mesh->draw(GL_TRIANGLES);
	m_textureShader->unbind();

}
void TextureApp::update(float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_P))
	{
		view = glm::lookAt(glm::vec3(2.5f, 5, 0.1f), glm::vec3(2.5f, 0, 2.5f), glm::vec3(0, 1, 0));
	}
}
void TextureApp::shutdown()
{
}