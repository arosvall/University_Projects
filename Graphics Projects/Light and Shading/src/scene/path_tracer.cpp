
// glm
#include <gtc/constants.hpp>
#include <gtc/random.hpp>

// std
#include <random>

// project
#include "scene.hpp"
#include "shape.hpp"
#include "light.hpp"
#include "material.hpp"
#include "path_tracer.hpp"



glm::vec3 SimplePathTracer::sampleRay(const Ray &ray, int) {
	// intersect ray with the scene
	RayIntersection intersect = m_scene->intersect(ray);

	// if ray hit something
	if (intersect.m_valid) {

		// simple grey shape shading
		float f = glm::abs(glm::dot(-ray.direction, intersect.m_normal));
		glm::vec3 grey(0.5, 0.5, 0.5);
		return glm::mix(grey / 2.0f, grey, f);
	}

	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}



glm::vec3 CorePathTracer::sampleRay(const Ray &ray, int) {

	RayIntersection intersect = m_scene->intersect(ray);

	// if ray hit something
	if (intersect.m_valid) {
		//create the colour vector
		glm::vec3 col(0, 0, 0);

		for (std::shared_ptr<Light> l : m_scene->lights()) {

			//store position and light intensity in a quick named vector for convenience
			glm::vec3 pos = intersect.m_position;
			glm::vec3 lightIntensity = l->irradiance(pos);

			bool occl = l->occluded(m_scene, pos + (intersect.m_normal * glm::vec3(0.00001)));

			if (occl) {
				col += ((l->ambience() * intersect.m_material->diffuse()));
				continue;
			}

			col += ((l->ambience() * intersect.m_material->diffuse()));

			//calculate the diffuse
			glm::vec3 n = intersect.m_normal;
			glm::vec3 d = -l->incidentDirection(pos);
			float diffCos = glm::dot(n, d);

			if (diffCos <= 0.f) { continue; }

			//calculating the diffuse lighting, based on the lecture slides
			glm::vec3 diffuse = lightIntensity * intersect.m_material->diffuse() * diffCos;
			
			//add diffuse
			col += diffuse;
			glm::vec3 f = -ray.direction;

			//calculating the specular lighting
			glm::vec3 nHat = glm::normalize(n * glm::dot(n, d));
			glm::vec3 r = glm::normalize((2.f * nHat) - d);

			//find the cos ^ n function result
			float specCos = glm::dot(r, f);

			if (specCos <= 0.f) { continue; }

			float expo = intersect.m_material->shininess();
			specCos = glm::pow(specCos, expo);

			//calculating diffuse based on the formula
			glm::vec3 specular = lightIntensity * intersect.m_material->specular() * specCos;
			//add specular
			col += specular;

		}
		//return the intersected colour
		return col;

	}
	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}



glm::vec3 CompletionPathTracer::sampleRay(const Ray &ray, int depth) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Using the same requirements for the CorePathTracer add in 
	// a recursive element to calculate perfect specular reflection.
	// That is compute the reflection ray off your intersection and
	// sample a ray in that direction, using the result to additionally
	// light your object. To make this more realistic you may weight
	// the incoming light by the (1 - (1/shininess)).
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	RayIntersection intersect = m_scene->intersect(ray);

	// if ray hit something
	if (intersect.m_valid) {
		//create the colour vector
		glm::vec3 col(0, 0, 0);

		for (std::shared_ptr<Light> l : m_scene->lights()) {

			//store position and light intensity in a quick named vector for convenience
			glm::vec3 pos = intersect.m_position;
			glm::vec3 lightIntensity = l->irradiance(pos);

			bool occl = l->occluded(m_scene, pos + (intersect.m_normal * glm::vec3(0.00001)));

			if (occl) {
				col += ((l->ambience() *intersect.m_material->diffuse()));
			if (depth <= 5 && depth != 0) {
				glm::vec3 reflectDir = l->incidentDirection(pos) - (2.f*glm::dot(pos, intersect.m_normal) * (intersect.m_normal));
				Ray reflectRay = Ray(intersect.m_position, reflectDir);
				col = 0.8f * col + 0.2f * sampleRay(reflectRay, depth - 1);
			}
				continue;
			}
			
			col += ((l->ambience() * intersect.m_material->diffuse()));

			//calculate the diffuse
			glm::vec3 n = intersect.m_normal;
			glm::vec3 d = -l->incidentDirection(pos);


			float diffCos = glm::dot(n, d);

			if (diffCos <= 0.f) { continue; }
			//calculating the diffuse lighting, based on the lecture slides
			glm::vec3 diffuse = lightIntensity * intersect.m_material->diffuse() * diffCos;

			//add diffuse
			col += diffuse;
			glm::vec3 f = -ray.direction;

			//calculating the specular lighting
			glm::vec3 nHat = glm::normalize(n * glm::dot(n, d));
			glm::vec3 r = glm::normalize((2.f * nHat) - d);

			//find the cos ^ n function result
			float specCos = glm::dot(r, f);

			if (specCos <= 0.f) { continue; }

			float expo = (1-(1/intersect.m_material->shininess()));
			specCos = glm::pow(specCos, expo);

			//calculating diffuse based on the formula
			glm::vec3 specular = lightIntensity * intersect.m_material->specular() * specCos;
			//add specular
			col += specular;

			if (depth <= 5 && depth != 0) {
				glm::vec3 reflectDir = l->incidentDirection(pos) - (2.f*glm::dot(pos, intersect.m_normal) * (intersect.m_normal));
				Ray reflectRay = Ray(intersect.m_position, reflectDir);
					col = 0.8f * col + 0.2f * sampleRay(reflectRay, depth - 1);
				
			}
		}
		//return the intersected colour
		
		return col;

	}
	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}



glm::vec3 ChallengePathTracer::sampleRay(const Ray &ray, int depth) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Implement a PathTracer that calculates the diffuse and 
	// specular, for the given ray in the scene, using the 
	// Phong lighting model. Give special consideration to objects
	// that occluded from direct lighting (shadow rays).
	// Implement support for textured materials (using a texture
	// for the diffuse portion of the material).
	//
	// EXTRA FOR EXPERTS :
	// Additionally implement indirect diffuse and specular instead
	// of using the ambient lighting term.
	// The diffuse is sampled from the surface hemisphere and the
	// specular is sampled from a cone of the phong lobe (which
	// gives a glossy look). For best results you need to normalize
	// the lighting (see http://www.thetenthplanet.de/archives/255)
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...

	// no intersection - return background color
	return { 0.3f, 0.3f, 0.4f };
}