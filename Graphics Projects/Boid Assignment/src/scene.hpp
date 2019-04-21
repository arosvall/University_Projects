
#pragma once

//std
#include <vector>

// glm
#include <glm.hpp>

// project
#include "cgra/cgra_mesh.hpp"
#include "sphere.hpp"
#include "cgra/cgra_shader.hpp"


// foward declare boid class
class Boid;
class Predator;
class Sphere;

class Scene {
private:
	// opengl draw data
	GLuint m_color_shader = 0;
	GLuint m_aabb_shader = 0;
	GLuint m_axis_shader = 0;
	GLuint m_skymap_shader = 0;
	cgra::mesh m_simple_boid_mesh;
	cgra::mesh m_boid_mesh;
	cgra::mesh m_predator_mesh;
	cgra::mesh m_sphere_mesh;

	// draw status
	bool m_show_aabb = true;
	bool m_show_axis = false;
	bool m_show_skymap = false;

	// scene data
	glm::vec3 m_bound_hsize = glm::vec3(20);
	std::vector<Boid> m_boids;

	
	//-------------------------------------------------------------
	// [Assignment 3] :
	// Create variables for keeping track of the boid parameters
	// such as min and max speed etc. These parameters can either be
	// public, or private with getter functions.
	//-------------------------------------------------------------

	// YOUR CODE GOES HERE
	// ...


public:

	Scene();

	std::vector<Predator> m_predators;
	std::vector<Sphere> m_spheres;

	//core
	float maxSpeed;
	float minSpeed;
	float accel;
	float neighbourhood;
	float avoidAmount;
	float alignAmount;
	float cohereAmount;
	
	//comp
	float predatorMinSpeed;
	float predatorMaxSpeed;
	float predatorMaxAccel;
	float predatorNeighSize;
	float predatorAvoidWeight;
	float predatorChaseWeight;
	
	//bounding box enforcement style
	int boundType = 1;
	float boidNum = 0;
	float predNum = 0;
	float sphereNum = 3;


	// functions that load the scene
	void loadCore();
	void loadCompletion();
	void loadChallenge();

	// called every frame, with timestep in seconds
	void update(float timestep);

	// called every frame, with the given projection and view matrix
	void draw(const glm::mat4 &proj, const glm::mat4 &view);

	// called every frame (to fill out a ImGui::TreeNode)
	void renderGUI();

	// returns a const reference to the boids vector
	const std::vector<Boid> & boids() const { return m_boids; }

	// returns the half-size of the bounding box (centered around the origin)
	glm::vec3 bound() const { return m_bound_hsize; }

	// YOUR CODE GOES HERE
	// ...
};