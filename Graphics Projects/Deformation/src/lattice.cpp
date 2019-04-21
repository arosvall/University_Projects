//lattice file
#pragma once
#include "lattice.hpp"

#include "cgra/matrix.hpp"
#include <iostream>
#include <stdexcept>
#include "cgra/wavefront.hpp"
Lattice::Lattice(
	glm::vec3 min,
	glm::vec3 max,
	glm::vec3 res)

	: m_min(min), m_max(max),
	m_resolution(res)
{

	int xRes = m_resolution.x;
	int yRes = m_resolution.y;
	int zRes = m_resolution.z;
	created = true;

	int num = (xRes + 1) * (yRes + 1) * (zRes + 1);
	cgra::Matrix<double> v(num, 3);
	float totalRes = xRes + yRes + zRes;
	float tres = totalRes / 2.0f;
	float lines = num * tres;
	int l = glm::round(lines);
	cgra::Matrix<unsigned int> f(l, 2);
	int sqIdx = 0;
	unsigned int id = 0;

	float xDist = (m_max.x - m_min.x) / (xRes*1.0f);
	float yDist = (m_max.y - m_min.y) / (yRes*1.0f);
	float zDist = (m_max.z - m_min.z) / (zRes*1.0f);

	for (int z = 0; z <= zRes; z++) {
		for (int y = 0; y <= yRes; y++) {
			for (int x = 0; x <= xRes; x++) {
				glm::vec3 i(x, y, z);
				glm::vec3 p = i * glm::vec3(xDist, yDist, zDist) + m_min;
				int idx = x + y * (xRes + 1) + z * (xRes + 1) * (yRes + 1) +1;
				LatticeNode node(idx, p);
				m_nodes.push_back(node);
				v.setRow(idx-1, { p.x, p.y, p.z });
			}
		}
	}
	
	for (int z = 0; z <= zRes; z++) {
		for (int y = 0; y <= yRes; y++) {
			for (int x = 0; x <= xRes; x++) {
				id = x + y * (xRes + 1) + z * (xRes + 1) * (yRes + 1);
				if(z < zRes) {
					unsigned int nId = x + (y * (xRes + 1)) + ((z+1) * (xRes + 1) * (yRes + 1));
					f.setRow(sqIdx ,{ id, nId });
					sqIdx++;
				}
				if (y < yRes) {
					unsigned int nId = x + ((y+1) * (xRes + 1)) + (z * (xRes + 1) * (yRes + 1));
					f.setRow(sqIdx,{ id, nId });
					sqIdx++;
				}
				if (x < xRes) {
					unsigned int nId = (x+1) + (y * (xRes + 1)) + (z * (xRes + 1) * (yRes + 1));
					f.setRow(sqIdx,{ id, nId });
					sqIdx++;
				}

			}
		}

	}
	faces = f;
	vertices = v;
	latt_mesh.setData(vertices, faces);
	}

LatticeNode &Lattice::getNode(
	int x, int y, int z) {
	unsigned int id = ((z * m_resolution.y * m_resolution.x) + (y * m_resolution.x) + (x)) +1;
	for (LatticeNode n : m_nodes) {
		if (n.m_id == id) {
			return n;
		}
	}
}

void Lattice::draw() {
	latt_mesh.draw(GL_LINES);

}

void Lattice::recalc(LatticeNode n, glm::vec3 pos) {
	glm::vec3 newPos = n.m_position + pos;
	LatticeNode toAdd = LatticeNode(n.m_id, newPos);
	m_nodes[n.m_id-1] = toAdd;
	vertices.setRow(n.m_id-1, { newPos.x, newPos.y, newPos.z });
	latt_mesh.setData(vertices, faces);
	glm::vec3 min = m_min;
	glm::vec3 max = m_max;
	for (LatticeNode n : m_nodes) {
		min = glm::min(n.m_position, min);
		max = glm::max(n.m_position, max);
	}
	m_min = min;
	m_max = max;
}
