#pragma once

// glm
#include <glm.hpp>

// project
#include "scene.hpp"
class Sphere {

public:
	glm::vec3 m_centre;
	float m_radius;
	int sphId;
	glm::vec3 color() const;
	
	Sphere() : m_centre(glm::vec3(0)), m_radius(0.f), sphId(-1) {}
	Sphere(glm::vec3 centre, float radius, int id) : m_centre(centre), m_radius(radius), sphId(id) {}
};