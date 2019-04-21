#pragma once

#include "cgra/mesh.hpp"
#include "cgra/shader.hpp"
#include "glm/glm.hpp"
#include "cgra/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

class LatticeNode;

class Lattice {
public:
	std::vector<LatticeNode> m_nodes;
	glm::vec3 m_min;
	glm::vec3 m_max;
	glm::vec3 m_resolution; 
	cgra::Mesh latt_mesh;
	cgra::Matrix<unsigned int> faces;
	cgra::Matrix<double> vertices;

	int vertN;
	bool created = false;

	Lattice() 
		: m_min(0), m_max(0), m_resolution(0) { }

	Lattice(glm::vec3 min,
		glm::vec3 max,
		glm::vec3 res);

	LatticeNode &getNode(
		int x, int y, int z);

	void draw();

	void recalc(LatticeNode n, glm::vec3);

};

class LatticeNode {
public:
	int m_id;
	glm::vec3 m_position;
	
	glm::vec3 m_offset;

	cgra::Mesh sphere;
	
	LatticeNode(int id,	glm::vec3 pos): m_id(id),

		m_position(pos),
		
		m_offset(0) {}

};