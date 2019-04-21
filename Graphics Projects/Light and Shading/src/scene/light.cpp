
// glm
#include <gtc/constants.hpp>

// project
#include "light.hpp"


bool DirectionalLight::occluded(Scene *scene, const glm::vec3 &point) const {
	Ray reverseDirection = Ray(point, -m_direction);
	RayIntersection intersect = scene->intersect(reverseDirection);
	if (intersect.m_valid) {
		if (glm::dot(intersect.m_normal, m_direction) > 0) {
			return true;
		}
	}
	return false;
}


glm::vec3 DirectionalLight::incidentDirection(const glm::vec3 &) const {
	return m_direction;
}


glm::vec3 DirectionalLight::irradiance(const glm::vec3 &) const {
	return m_irradiance;
}


bool PointLight::occluded(Scene *scene, const glm::vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Determine whether the given point is being occluded from
	// this directional light by an object in the scene.
	// Remember that point lights are somewhere in the scene and
	// an occulsion has to occur somewhere between the light and 
	// the given point.
	//-------------------------------------------------------------
 
	// YOUR CODE GOES HERE
	// ...
	Ray reverseDirection = Ray(point, m_position-point);
	RayIntersection intersect = scene->intersect(reverseDirection);
	if (intersect.m_valid){
		glm::vec3 lighttoobj = point - m_position;
		glm::vec3 colllight = intersect.m_position - m_position;
		float dp = glm::dot(lighttoobj, colllight);
		return dp >= 0;
		
	}

	return false;
}


glm::vec3 PointLight::incidentDirection(const glm::vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Return the direction of the incoming light (light to point)
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	//calculate a normalized vector from light to point
	glm::vec3 dir = point - m_position;
	//return glm::normalize(dir);
	return dir;
}


glm::vec3 PointLight::irradiance(const glm::vec3 &point) const {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Return the total irradiance on the given point.
	// This requires you to convert the flux of the light into
	// irradiance by dividing it by the surface of the sphere
	// it illuminates. Remember that the surface area increases
	// as the sphere gets bigger, ie. the point is further away.
	//-------------------------------------------------------------
	// YOUR CODE GOES HERE
	// ...
	float dist = glm::distance(point, m_position);
	dist = dist * dist;

	float pi = glm::pi<float>(); 
	float surf = 4.f * pi * dist;
	glm::vec3 intensity = m_flux / surf;
//	return glm::normalize(intensity);
	return intensity;
}

