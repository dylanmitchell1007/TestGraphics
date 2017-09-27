#pragma once
#include "gl_core_4_4.h"
#include <glm/glm.hpp>
#include <vector>
struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 normal;
	glm::vec2 texture;
};
class Mesh
{
public:
	Mesh();
	~Mesh();


	void initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices);
	void create_buffers();

	void bind();
	void unbind();
	void draw(GLuint mode);
	unsigned int index_count;
	unsigned int vertex_count;
	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
private:

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
};