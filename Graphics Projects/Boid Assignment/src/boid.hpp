
#pragma once

// glm
#include <glm.hpp>

// project
#include "scene.hpp"


class Boid {
private:
	glm::vec3 m_sphereForce;
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	glm::vec3 m_acceleration;
	glm::vec3 m_neighbourhood;

public:
	float mass;
	int id;
	int boidIdentifier;
	int steps = 0;
	int neighCount = 0;
	bool changedRecent = false;
	float rayCynd = 0.5;
	Boid() : m_position(glm::vec3(0)), m_velocity(glm::vec3(0)), id(-1), boidIdentifier(-1){}
	Boid(glm::vec3 pos, glm::vec3 dir, int id, int Bid) : m_position(pos), m_velocity(dir), id (id), boidIdentifier(Bid) { }
	
	glm::vec3 position() const { return m_position; }
	glm::vec3 velocity() const { return m_velocity; }
	glm::vec3 acceleration() const { return m_acceleration; }

	glm::vec3 color() const;

	void calculateForces(Scene *scene);
	void update(float timestep, Scene *scene);
	bool isNeighbour(Boid b, Scene *scene);
	void sphereCheck(Scene *scene);

};

class Predator {
	public:
		int steps = 0;
		bool changedRecent = false;
		float mass = 1.5;
		glm::vec3 p_position;
		glm::vec3 p_velocity;
		glm::vec3 p_acceleration;
		glm::vec3 p_neighbourhood;
		bool pickedTar = false;
		float tarTimer =0;
	    Boid target;
		int targetBoidId;
		int predId;
		float rayCynd = .5f;
		glm::vec3 m_sphereForce;

		Predator(glm::vec3 pos, glm::vec3 dir, int id) : p_position(pos), p_velocity(dir), predId(id) {}
		glm::vec3 color() const;

		void calculateForces(Scene *scene);
		void update(float timestep, Scene *scene);
		void sphereCheck(Scene *scene);
};