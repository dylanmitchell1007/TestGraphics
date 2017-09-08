#include "Mesh.h"
#include <glm\fwd.hpp>
#include <glm\ext.hpp>
#include <glm\glm.hpp>

Mesh::Mesh() {};
Mesh::~Mesh() {};

void Mesh::initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices)
{
	
	m_vertices.clear();
	m_indices.clear();

	for (auto v : verts)
		m_vertices.push_back(v);
	for (auto index : indices)
		m_indices.push_back(index);

	index_count = m_indices.size();
	vertex_count = m_vertices.size();

	this->create_buffers();
}

//void Mesh::create_buffers()
//{
//	const GLfloat colors[4][3] = {
//		{ 1.0,  0.0,  0.0 }, /* Red */
//		{ 0.0,  1.0,  0.0 }, /* Green */
//		{ 0.0,  0.0,  1.0 }, /* Blue */
//		{ 1.0,  1.0,  1.0 } }; /* White */
//	//Allocate and assign vertex array to our handle
//	glGenVertexArrays(1, &m_vao);
//
//	//Bind our Vertex array object
//	glBindVertexArray(m_vao);
//
//	//Allocate and assign two Vertex Buffer Objects to our handle 
//	glGenBuffers(1, &m_vbo);
//
//	//Bind our first VBO as being the active buffer and storing vertex attributes(coordinates)
//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//
//	glBufferData(GL_ARRAY_BUFFER, this->vertex_count * sizeof(Vertex), m_indices.data(), GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//
//	glEnableVertexAttribArray(0);
//
//	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GL_FLOAT), colors, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
//
//	glEnableVertexAttribArray(1);
//
//	glBindVertexArray(0); //clean vao
//	glBindBuffer(GL_ARRAY_BUFFER, 0); //clean vbo
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //clean ibo
//}

void Mesh::create_buffers()
{
	glGenVertexArrays(1, &this->m_vao); //GENRATE THE VERTEX ARRAY OBJECT BEFORE THE VBO AND IBO
	glBindVertexArray(this->m_vao);

	glGenBuffers(1, &this->m_vbo); //GENERATE A VBO
	glGenBuffers(1, &this->m_ibo); //GENERATE A IBO
	
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo); //BIND THE VBO
	//glBufferData(GL_ARRAY_BUFFER, this->vertex_count * sizeof(Vertex), glm::value_ptr(_vertices), GL_STATIC_DRAW); //SEND OPENGL THE VBO'S INFORMATION
	glBufferData(GL_ARRAY_BUFFER, this->vertex_count * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW); //SEND OPENGL THE VBO'S INFORMATION

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ibo); //BIND THE IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		this->index_count * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW); //SEND OPENGL THE IBO'S INFORMATION

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));

	glBindVertexArray(0); //UNBIND THE VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0); //UNBIND THE VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //UNBIND THE IBO

};


void Mesh::bind()
{
	glBindVertexArray(this->m_vao);
}
void Mesh::unbind()
{
	glBindVertexArray(0);
}

void Mesh::draw(GLuint mode)
{
	this->bind();
	glDrawElements(mode, this->index_count, GL_UNSIGNED_INT, 0);
	this->unbind();	
}