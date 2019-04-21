
// glm
#include <gtc/random.hpp>

// project
#include "sphere.hpp"
#include "boid.hpp"
#include "scene.hpp"
#include "cgra/cgra_mesh.hpp"


glm::vec3 Boid::color() const {
	if (id == 0) {
		return glm::vec3(0, 1, 0);
	}
	return glm::vec3(0, 0, 1);
	}


void Boid::calculateForces(Scene *scene) {

	glm::vec3 force(0); // start with zero force
	
    //check against all other boids.
	glm::vec3 cohere(0);
	glm::vec3 avoid(0);
	glm::vec3 align(0);
	float count = 0;

	for (Boid b : scene->boids()) {
		if (isNeighbour(b, scene)) {	
		if (b.boidIdentifier != boidIdentifier) {
			if (b.id == id) {
				//calculating the cohere only for same flock
				cohere += b.position();

				//calculate the align only for same flock
				align += b.velocity();
			}
			//calculate the avoid
			glm::vec3 af = m_position - b.position();
			glm::vec3 awayForce(glm::normalize(af));
			float bal = 1.0f / (glm::length(af));
			avoid += (bal * awayForce);
			//increase count to know that all nodes are being affected
			count++;
		}
		}
	}
	glm::vec3 predAvoid(0);
	for (Predator p : scene->m_predators) {
		if (glm::distance(p.p_position, m_position) < scene->predatorNeighSize) {
			glm::vec3 af = m_position - p.p_position;
			glm::vec3 awayForce(glm::normalize(af));
			float bal = 1.0f / (glm::length(af));
			predAvoid += (bal * awayForce);
		}
	}
	sphereCheck(scene);

	cohere /= count;
	align /= count;
	cohere -= m_position;
	align -= m_velocity;

	neighCount = count;

	glm::vec3 cohereForce(scene->cohereAmount * cohere);
	glm::vec3 alignForce(scene->alignAmount * align);
	glm::vec3 avoidForce(scene->avoidAmount * avoid);
	glm::vec3 fearForce(scene->predatorAvoidWeight * predAvoid);

	force += cohereForce;
	force += alignForce;
	force += avoidForce;
	force += fearForce;
	force += m_sphereForce *1.1f;
	
	m_acceleration = force;
	
	if (glm::length(m_acceleration) > scene->accel) {
		m_acceleration = scene->accel * glm::normalize(m_acceleration);
	}


}


//Need to test the update parameters
void Boid::update(float timestep, Scene *scene) {
	//calculate velocity based on the acceleration
	//check that it won't exceed maximum / minimum speed
	//-------------------------------------------------------------
	// [Assignment 3] :
	// Integrate the velocity of the boid using the timestep.
	// Update the position of the boid using the new velocity.
	// Take into account the bounds of the scene which may
	// require you to change the velocity (if bouncing) or
	// change the position (if wrapping).
	//-------------------------------------------------------------
	m_velocity = m_velocity + (m_acceleration*timestep);
	
	//cap velocity here, if less than min set to min. If greater than max set to max. Allow min to be at 0, stop it from going below though. Gives the ability to check the cohesion
	if (glm::length(m_velocity) < scene->minSpeed) {
		m_velocity = scene->minSpeed * glm::normalize(m_velocity);
	}
	else if (glm::length(m_velocity) > scene->maxSpeed) {
		m_velocity = scene->maxSpeed * glm::normalize(m_velocity);
	}
	
	//bounce

	if (scene->boundType == 1 && !changedRecent) {
		if (((m_position.x + m_velocity.x) > scene->bound().x) || ((m_position.x + m_velocity.x) < (-1.0f * scene->bound().x))) {
			m_velocity.x *= -1;
			changedRecent = true;
		}
		if (((m_position.y + m_velocity.y) > scene->bound().y) || ((m_position.y + m_velocity.y) < (-1.0f * scene->bound().y))) {
			m_velocity.y *= -1;
			changedRecent = true;
		}
		if (((m_position.z + m_velocity.z) > scene->bound().z) || ((m_position.z + m_velocity.z) < (-1.0f * scene->bound().z))) {
			m_velocity.z *= -1;
			changedRecent = true;
		}
	m_position += (m_velocity * timestep);

	}
	if (scene->boundType == 2) {
		if ((glm::abs(m_position.x) > glm::abs(scene->bound().x)) || (glm::abs(m_position.y) > glm::abs(scene->bound().y)) || (glm::abs(m_position.z) > glm::abs(scene->bound().z))) {
			m_position = -m_position;
			m_position -= m_velocity * timestep;
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
}

bool Boid::isNeighbour(Boid b, Scene * scene){
	//compare the distance between the two vectors to the length of the neighbourhood 
	if (glm::distance(b.position(), m_position) < scene->neighbourhood) {
		return true;
	}
	return false;
}

void Boid::sphereCheck(Scene *scene)
{
	glm::vec3 sForce(0); 
	float ta = 0;
	float tb = 0;
	float t = -1;
	Sphere closest;
	bool chosenSphere = false;
	glm::vec3 d(m_position);
	glm::vec3 o(0);

	for (Sphere s : scene->m_spheres) {
		float radius = s.m_radius + rayCynd;
		float a = glm::dot(d, d);
		glm::vec3 oc(s.m_centre - o);
		float b = glm::dot((2.f*oc), d);
		float c = glm::dot(oc, oc) - (radius*radius);

		float discrim = (b*b) - (4*a*c);
		if (discrim > 0) {
			float ta = (-b + (glm::sqrt((b*b) - (4*a*c)))) / (2*a); 
			float tb = (-b - (glm::sqrt((b*b) - (4 * a*c)))) / (2 * a);
			if (ta < 0 && tb < 0) {
				break;
			}
			if (t == -1) {
				if (ta > tb) {
					t = tb;
				}
				else{
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
		glm::vec3 d(m_position);
		float oe((glm::dot(oc, d)) / glm::length(d));
		glm::vec3 e(o + (oe * glm::normalize(d)));
		sForce = glm::normalize(e - closest.m_centre);
	}
	m_sphereForce = sForce;
}


