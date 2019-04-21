
// std
#include <algorithm>
#include <utility>

// glm
#include <glm.hpp>
#include <gtc/constants.hpp>

// project
#include "shape.hpp"


RayIntersection AABB::intersect(const Ray &ray) {
	RayIntersection intersect;
	glm::vec3 rel_origin = ray.origin - m_center;

	// start magic
	// x
	float rdx_inv = 1 / ray.direction.x;
	float tx1 = (-m_halfsize.x - rel_origin.x) * rdx_inv;
	float tx2 = (m_halfsize.x - rel_origin.x) * rdx_inv;

	float tmin = std::min(tx1, tx2);
	float tmax = std::max(tx1, tx2);

	// y
	float rdy_inv = 1 / ray.direction.y;
	float ty1 = (-m_halfsize.y - rel_origin.y) * rdy_inv;
	float ty2 = (m_halfsize.y - rel_origin.y) * rdy_inv;

	tmin = std::max(tmin, std::min(ty1, ty2));
	tmax = std::min(tmax, std::max(ty1, ty2));

	// z
	float rdz_inv = 1 / ray.direction.z;
	float tz1 = (-m_halfsize.z - rel_origin.z) * rdz_inv;
	float tz2 = (m_halfsize.z - rel_origin.z) * rdz_inv;

	tmin = std::max(tmin, std::min(tz1, tz2));
	tmax = std::min(tmax, std::max(tz1, tz2));

	if (tmax < tmin) return intersect;
	// end magic

	intersect.m_distance = tmin < 0 ? tmax : tmin;
	intersect.m_position = ray.origin + intersect.m_distance * ray.direction;
	intersect.m_valid = tmax >= 0;
	glm::vec3 work_out_a_name_for_it_later = glm::abs((intersect.m_position - m_center) / m_halfsize);
	float max_v = std::max(work_out_a_name_for_it_later[0], std::max(work_out_a_name_for_it_later[1], work_out_a_name_for_it_later[2]));
	intersect.m_normal = glm::normalize(glm::mix(intersect.m_position - m_center, glm::vec3(0), glm::lessThan(work_out_a_name_for_it_later, glm::vec3(max_v))));
	intersect.m_uv_coord = (glm::abs(intersect.m_normal.x) > 0) ?
		glm::vec2(intersect.m_position.y, intersect.m_position.z) :
		glm::vec2(intersect.m_position.x, intersect.m_position.y + intersect.m_position.z);
	intersect.m_shape = this;

	return intersect;
}


RayIntersection Sphere::intersect(const Ray &ray) {
	RayIntersection intersect;
	
	glm::vec3 d = ray.direction;
	glm::vec3 o = ray.origin;
	glm::vec3 oc = o - m_center;

	float t = 0;
	float r = m_radius;
	

	float a = glm::dot(d, d);
	float b = 2 * (glm::dot(oc, d));
	float c = glm::dot(oc, oc) - (r * r);

	float deter = (b*b) - (4.f*a*c);

	if (deter < 0) {
		return intersect;
	}

	float sq = glm::sqrt(deter);

	float ta = (-b + sq) / (2 * a);
	float tb = (-b - sq) / (2 * a);

	if (ta < tb && ta > 0) {
		t = ta;
	}
	if (tb < ta && tb > 0) {
		t = tb;
	}

	glm::vec3 pt(o + (t*d));
	glm::vec3 norm = glm::normalize(pt - m_center);
	
	intersect.m_valid = t > 0;
	intersect.m_distance = t;
	intersect.m_position = pt;
	intersect.m_shape = this;
	intersect.m_normal = norm;

	return intersect;
}

RayIntersection Plane::intersect(const Ray & ray)
{	
	RayIntersection intersect;
	glm::vec3 qo = m_cent - ray.origin;
	float dn = glm::dot(ray.direction, m_norm);
	if (dn == 0) {
		return intersect;
	}
	float qon = glm::dot(qo, m_norm);
	float t = qon / dn;
	if (t <= 0) {
		return intersect;
	}
		glm::vec3 pt = ray.origin + (t * ray.direction);
		intersect.m_valid = t > 0;
		intersect.m_distance = t;
		intersect.m_position = pt;
		intersect.m_shape = this;
		intersect.m_normal = glm::normalize(m_norm);
		if(dn > 0) {
			intersect.m_normal *= -1;
	}
	return intersect;
}

RayIntersection Triangle::intersect(const Ray & ray) {
	RayIntersection intersect;
	glm::vec3 q = m_v0;
	glm::vec3 norm = glm::cross(m_v1 - m_v0, m_v2 - m_v0);
	if (glm::dot(ray.direction, norm) != 0) {
		float t = (glm::dot(q - ray.origin, norm) / glm::dot(ray.direction, norm));
		if (t <= 0) {
			return intersect;
		}
		glm::vec3 pt = ray.origin + (t * ray.direction);
		glm::vec3 f0 = glm::cross(m_v1 - m_v0, pt - m_v0);
		glm::vec3 f1 = glm::cross(m_v2 - m_v1, pt - m_v1);
		glm::vec3 f2 = glm::cross(m_v0 - m_v2, pt - m_v2);
		
		float f0d = glm::dot(norm, f0);
		float f1d = glm::dot(norm, f1);
		float f2d = glm::dot(norm, f2);
		if (f2d > 0 && f1d > 0 && f0d > 0) {
			intersect.m_valid = t > 0;
			intersect.m_distance = t;
			intersect.m_normal = norm;
			intersect.m_shape = this;
			intersect.m_position = pt;
		}
	}
	return intersect;
}



RayIntersection Disc::intersect(const Ray & ray){
	RayIntersection intersect;
	glm::vec3 qo = m_centre - ray.origin;
	float dn = glm::dot(ray.direction, m_norm);
	if (dn == 0) {
		return intersect;
	}
	else {
		float qon = glm::dot(qo, m_norm);
		float t = qon / dn;
		if (t < 0) {
			return intersect;
		}
		glm::vec3 pt = ray.origin + (t*ray.direction);
		if (glm::distance(pt, m_norm) < m_radius) {
			intersect.m_valid = t > 0;
			intersect.m_distance = t;
			intersect.m_normal = glm::normalize(m_norm);
			if (dn > 0) { 
				intersect.m_normal *= -1;
			}
			intersect.m_position = pt;
		}
	}
	return intersect;
}

