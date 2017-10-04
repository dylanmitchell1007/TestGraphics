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

Mesh* generateGrid(unsigned int rows, unsigned int cols)
{
	auto aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			Vertex verts = {
				glm::vec4(float(c), 0, float(r), 1), //POSITION
				glm::vec4(0), //COLOR
				glm::vec4(0, 1, 0, 0), //NORMAL
				glm::vec2(float(c) / float(cols - 1), float(r) / float(rows - 1)) //TEXTURE COORDINATE
			};
			aoVertices[r * cols + c] = verts;
		}
	}

	std::vector<Vertex> verts = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	//Defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			//Triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			//Triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}
	}

	for (unsigned int i = 0; i < (rows * cols); i++)
	{
		verts.push_back(aoVertices[i]);
	}

	for (unsigned int i = 0; i < index; i++)
	{
		indices.push_back(auiIndices[i]);
	}

	Mesh* plane = new Mesh();
	plane->initialize(verts, indices);

	delete[] aoVertices;
	delete[] auiIndices;
	return plane;
}

float* generatePerlin(unsigned int width, unsigned int height)
{
	int dims = 64;
	float *perlinData = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 6;
	for (int x = 0; x < 64; ++x)
	{
		for (int y = 0; y < 64; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;
			perlinData[y * dims + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlinSample = glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlinData[y * dims + x] += perlinSample * amplitude;
				amplitude *= persistence;
			}
		}
	}
	return perlinData;
}

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

	
	this->plane_Mesh = generateGrid(25,25);
	/*generatePlane(this->plane_Mesh);*/

	m_perlinShader = new Shader();
	this->m_perlinShader->load("perlin.vert", GL_VERTEX_SHADER);
	this->m_perlinShader->load("perlin.frag", GL_FRAGMENT_SHADER);
	this->m_perlinShader->attach();

	m_texture = new Texture();
	m_texture->generate2D(64, 64, generatePerlin(64, 64));
	//m_texture->load("..//bin//textures//370z.jpg");
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


	m_perlinShader->bind();
	m_texture->bind();

	auto textureWVPuniform = m_perlinShader->getUniform("WVP");

	auto textureSampler2DUniform = m_perlinShader->getUniform("perlinTexture");

	glUniform1i(textureSampler2DUniform, 0);
	glUniformMatrix4fv(textureWVPuniform, 1, GL_FALSE, glm::value_ptr(mvp));
	plane_Mesh->draw(GL_TRIANGLES);
	m_perlinShader->unbind();

}
void TextureApp::update(float deltaTime)
{
	if (glfwGetKey(m_window, GLFW_KEY_P))
	{
		view = glm::lookAt(glm::vec3(2.5f, 15, -5.1f), glm::vec3(12.5f, 0, 12.5f), glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(m_window, GLFW_KEY_KP_7))
	{
		this->m_perlinShader->load("perlin.vert", GL_VERTEX_SHADER);
		this->m_perlinShader->load("perlin.frag", GL_FRAGMENT_SHADER);
		this->m_perlinShader->attach();
	}

}

void TextureApp::shutdown()
{
}