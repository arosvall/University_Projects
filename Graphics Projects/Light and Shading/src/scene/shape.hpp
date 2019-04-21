
#pragma once

// glm
#include <glm.hpp>

// project
#include "ray.hpp"
#include "scene.hpp"


class Shape {
public:
	virtual RayIntersection intersect(const Ray &ray) = 0;
};


class AABB : public Shape {
private:
	glm::vec3 m_center;
	glm::vec3 m_halfsize;

public:
	AABB(const glm::vec3 &c, float hs) : m_center(c), m_halfsize(hs) { }
	AABB(const glm::vec3 &c, const glm::vec3 &hs) : m_center(c), m_halfsize(hs) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};


class Sphere : public Shape {
private:
	glm::vec3 m_center;
	float m_radius;

public:
	Sphere(const glm::vec3 &c, float radius) : m_center(c), m_radius(radius) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};

//plane class consisting of three distinct points and the intersect method
class Plane : public Shape {
private:
	glm::vec3 m_p0;
	glm::vec3 m_p1;
	glm::vec3 m_p2;
	glm::vec3 m_cent;
	glm::vec3 m_norm;

public:
	Plane(const glm::vec3 centre, const glm::vec3 normal) : m_cent(centre), m_norm(normal) {}
	virtual RayIntersection intersect(const Ray &ray) override;
};

//triangle class consisting of three vertexs and the intersect method
class Triangle : public Shape {
private:
	glm::vec3 m_v0;
	glm::vec3 m_v1;
	glm::vec3 m_v2;

public:
	Triangle(const glm::vec3 &vert1, const glm::vec3 &vert2, const glm::vec3 &vert3) : m_v0(vert1), m_v1(vert2), m_v2(vert3) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};

class Disc : public Shape {
private:
	float m_radius;
	glm::vec3 m_centre;
	glm::vec3 m_norm;

public:
	Disc(const glm::vec3 &point1, const glm::vec3 &point2, float surfRad) : m_radius(surfRad), m_centre(point1), m_norm(point2) { }
	virtual RayIntersection intersect(const Ray &ray) override;
};
