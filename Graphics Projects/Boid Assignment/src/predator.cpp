
#pragma once

// glm
#include <glm.hpp>
#include <gtc/random.hpp>

// project
#include "boid.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "cgra/cgra_mesh.hpp"


glm::vec3 Predator::color() const {
return glm::vec3(1, 0, 0);
}

void Predator::calculateForces(Scene * scene)
{
	glm::vec3 force(0);
	for (Boid b : scene->boids()) {
		if (!pickedTar) {
			if (target.id == -1) {
				targetBoidId = predId;
				break;
			}
			if (target.neighCount > b.neighCount) {
				target = b;
				pickedTar = true;
			}
		}

		else {
			if (tarTimer > 100) {
				pickedTar = false;
			}
			else {
				tarTimer++;
			}
		}
	}
	glm::vec3 avoid(0.0f);

	//steer away from other predators if they're close
	for (Predator p : scene->m_predators) {
		if (p.predId != predId) {
			if (glm::distance(p.p_position, p_position) < 5) {

				glm::vec3 af = p_position - p.p_position;
				glm::vec3 awayForce(glm::normalize(af));
				float bal = 1.0f / (glm::length(af));
				avoid += (bal * awayForce);
			}
		}
	}
	//pick up on any changes from within the target boid
	target = scene->boids().at(targetBoidId);

	//find the vector to intercept the location of the target boid
	glm::vec3 tarpos(target.position());
	
	float d = glm::distance(p_position, target.position());
	float t = d * scene->predatorChaseWeight;
	glm::vec3 predictedVec = target.position() + (target.velocity() * t);
	predictedVec -= p_position;

	sphereCheck(scene);
	//update the force
	force += avoid;
	force += predictedVec;
	force += m_sphereForce *1.1f;
	p_acceleration = force / mass; 

	//cap to the max acceleration
	if (glm::length(p_acceleration) > scene->predatorMaxAccel) {
		p_acceleration = scene->predatorMaxAccel * glm::normalize(p_acceleration);
	}
}

void Predator::update(float timestep, Scene * scene) {
	p_velocity = p_acceleration * timestep;
	
	if(glm::length(p_velocity) > scene->predatorMaxSpeed) {
	p_velocity = scene->predatorMaxSpeed * glm::normalize(p_velocity);
}

else if(glm::length(p_velocity) < scene->predatorMinSpeed) {
p_velocity = scene->predatorMinSpeed * glm::normalize(p_velocity);
}

if (scene->boundType == 2) {
	if ((glm::abs(p_position.x) > glm::abs(scene->bound().x)) || (glm::abs(p_position.y) > glm::abs(scene->bound().y)) || (glm::abs(p_position.z) > glm::abs(scene->bound().z))) {
		p_position = -p_position;
	}
}
if (scene->boundType == 1 && !changedRecent) {
//	if (((p_position.x + p_velocity.x) > scene->bound().x) || ((p_position.x + p_velocity.x) < (-1.0f * scene->bound().x))) {
	if(glm::abs(p_position.x) > glm::abs(scene->bound().x)) {
		p_velocity.x *= -1;
		changedRecent = true;
	}
	if (((p_position.y + p_velocity.y) > scene->bound().y) || ((p_position.y + p_velocity.y) < (-1.0f * scene->bound().y))) {
		p_velocity.y *= -1;
		changedRecent = true;
	}
	if (((p_position.z + p_velocity.z) > scene->bound().z) || ((p_position.z + p_velocity.z) < (-1.0f * scene->bound().z))) {
		p_velocity.z *= -1;
		changedRecent = true;
	}
}
if (changedRecent) {
	if (steps < 10) {
		++steps;
	}
	else {
		changedRecent = false;
		steps = 0;
	}
}
p_position += p_velocity * timestep;
}

void Predator::sphereCheck(Scene * scene)
{
	glm::vec3 sForce(0);
	float ta = 0;
	float tb = 0;
	float t = -1;
	Sphere closest;
	bool chosenSphere = false;
	glm::vec3 d(p_position);
	glm::vec3 o(0);

	for (Sphere s : scene->m_spheres) {
		float radius = s.m_radius + rayCynd;
		float a = glm::dot(d, d);
		glm::vec3 oc(s.m_centre - o);
		float b = glm::dot((2.f*oc), d);
		float c = glm::dot(oc, oc) - (radius*radius);

		float discrim = (b*b) - (4 * a*c);
		if (discrim > 0) {
			float ta = (-b + (glm::sqrt((b*b) - (4 * a*c)))) / (2 * a);
			float tb = (-b - (glm::sqrt((b*b) - (4 * a*c)))) / (2 * a);
			if (ta < 0 && tb < 0) {
				break;
			}
			if (t == -1) {
				if (ta > tb) {
					t = tb;
				}
				else {
					t = ta;
				}
				chosenSphere = true;
				closest = s;
			}
			else {
				if (ta == tb && ta < t) {
					closest = s;
					chosenSphere = true;
				}
				else if (t > ta) {
					t = ta;
					closest = s;
					chosenSphere = true;
				}
				else if (tb < t) {
					t = tb;
					closest = s;
					chosenSphere = true;
				}
			}
		}

		//still in sphere

	}
	//if a sphere has been chosen, aka one will be intersected
	if (chosenSphere && t < 10) {
		glm::vec3 c = closest.m_centre;
		glm::vec3 oc(c - o);
		glm::vec3 d(p_position);
		float oe((glm::dot(oc, d)) / glm::length(d));
		glm::vec3 e(o + (oe * glm::normalize(d)));
		sForce = glm::normalize(e - closest.m_centre);
	}
	m_sphereForce = sForce;


}
