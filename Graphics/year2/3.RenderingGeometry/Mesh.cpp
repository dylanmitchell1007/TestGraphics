#include <GLFW\glfw3.h>
#include <glm\fwd.hpp>
#include <glm\ext.hpp>
#include<glm\glm.hpp>
#include "RenderingGeometryApp.h"
#include "Shader.h"
#include "Mesh.h"

//void Mesh::initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices)
//{
//	m_vertices.clear();
//	m_indices.clear();
//
//	for (auto v : verts)
//		m_vertices.push_back(v);
//	for (auto index : indices)
//		m_indices.push_back(index);
//
//	index_count = m_indices.size();
//	vertex_count = m_vertices.size();
//
//	this->create_buffers();
//}
//
////void Mesh::draw(GLuint mode)
////{
//	this->bind();
//	glDrawElements(mode, this->index_count, GL_UNSIGNED_INT, 0);
//	this->unbind();
//}
//void Mesh::bind()
//{
//	glBindVertexArray(this->m_vao);
//}
//void Mesh::unbind()
//{
//	glBindVertexArray(0);
//}