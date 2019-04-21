
// std
#include <random>

// stb
#include <stb_image.h>

// imgui
#include <imgui.h>

// glm
#include <gtc/matrix_transform.hpp>
#include <gtc/random.hpp>
#include <gtc/type_ptr.hpp>

// project
#include "sphere.hpp"
#include "scene.hpp"
#include "boid.hpp"
#include "cgra/cgra_wavefront.hpp"


Scene::Scene() {

	// load meshes
	cgra::mesh_data simple_boid_md = cgra::load_wavefront_mesh_data("work/res/models/boid.obj");
	m_simple_boid_mesh = simple_boid_md.upload_mesh(m_simple_boid_mesh);

	cgra::mesh_data boid_md = cgra::load_wavefront_mesh_data("work/res/models/spaceship_boid.obj");
	m_boid_mesh = boid_md.upload_mesh(m_boid_mesh);

	cgra::mesh_data predator_md = cgra::load_wavefront_mesh_data("work/res/models/predator_boid.obj");
	m_predator_mesh = predator_md.upload_mesh(m_predator_mesh);

	cgra::mesh_data sphere_md = cgra::load_wavefront_mesh_data("work/res/models/sphere.obj");
	m_sphere_mesh = sphere_md.upload_mesh(m_sphere_mesh);

	// load color shader
	cgra::shader_program color_sp;
	color_sp.set_shader(GL_VERTEX_SHADER, "work/res/shaders/simple_color.glsl");
	color_sp.set_shader(GL_FRAGMENT_SHADER, "work/res/shaders/simple_color.glsl");
	m_color_shader = color_sp.upload_shader();

	// load aabb shader
	cgra::shader_program aabb_sp;
	aabb_sp.set_shader(GL_VERTEX_SHADER, "work/res/shaders/aabb.glsl");
	aabb_sp.set_shader(GL_GEOMETRY_SHADER, "work/res/shaders/aabb.glsl");
	aabb_sp.set_shader(GL_FRAGMENT_SHADER, "work/res/shaders/aabb.glsl");
	m_aabb_shader = aabb_sp.upload_shader();

	// load axis shader
	cgra::shader_program axis_prog;
	axis_prog.set_shader(GL_VERTEX_SHADER, "work/res/shaders/axis.glsl");
	axis_prog.set_shader(GL_GEOMETRY_SHADER, "work/res/shaders/axis.glsl");
	axis_prog.set_shader(GL_FRAGMENT_SHADER, "work/res/shaders/axis.glsl");
	m_axis_shader = axis_prog.upload_shader();

	// load skymap shader
	cgra::shader_program skymap_sp;
	skymap_sp.set_shader(GL_VERTEX_SHADER, "work/res/shaders/skymap.glsl");
	skymap_sp.set_shader(GL_GEOMETRY_SHADER, "work/res/shaders/skymap.glsl");
	skymap_sp.set_shader(GL_FRAGMENT_SHADER, "work/res/shaders/skymap.glsl");
	m_skymap_shader = skymap_sp.upload_shader();


	//fill vectors with blank for the initial drawing
	maxSpeed = 1.f;
	minSpeed = 1.f;
	accel = 1.f;
	neighbourhood = 10.f;
	avoidAmount = 1.f;
	alignAmount = 1.f;
	cohereAmount = 1.f;
	//comp
	predatorMinSpeed = 1.f;
	predatorMaxSpeed = 1.f;
	predatorMaxAccel = .5f;
	predatorNeighSize = 10.f;
	predatorAvoidWeight = 1.5f;
	predatorChaseWeight = 0.1f;




}


void Scene::loadCore() {
	//-------------------------------------------------------------
	// [Assignment 3] (Core) :
	// Initialize the scene with 100-300 boids in random locations
	// inside the current bound size.
	//-------------------------------------------------------------

	m_boids.clear();
	if (boidNum == 0) {
		boidNum = 200;
	}
	for (int i = 0; i < boidNum; i++) {
	
		glm::vec3 pos = glm::linearRand(glm::vec3(-m_bound_hsize), glm::vec3(m_bound_hsize));
		m_boids.push_back(Boid(pos, glm::sphericalRand(1.0), 0, i));
	}

	}

void Scene::loadCompletion() {
	m_boids.clear();
	m_predators.clear();
	if (predNum == 0) {
		predNum = 2;
	}

	if (boidNum == 0) {
		boidNum = 150;
	}

	for (int i = 0; i < boidNum; i++) {
		glm::vec3 pos = glm::linearRand(glm::vec3(-m_bound_hsize), glm::vec3(m_bound_hsize));
		
		if (i % 2 == 0) {
			m_boids.push_back(Boid(pos, glm::sphericalRand(1.0), 0, i));
		}
		else {
			m_boids.push_back(Boid(pos, glm::sphericalRand(1.0), 1, i));
		}
	}

	for (int j = 0; j < 2; j++) {
		glm::vec3 pos = glm::linearRand(glm::vec3(-m_bound_hsize), glm::vec3(m_bound_hsize));
		m_predators.push_back(Predator(pos, glm::sphericalRand(1.0), j));
	}
}


void Scene::loadChallenge() {
	
	m_boids.clear();
	if (boidNum == 0) {
		boidNum = 150;
	}
	for (int i = 0; i < boidNum; i++) {
		glm::vec3 pos = glm::linearRand(glm::vec3(-m_bound_hsize), glm::vec3(m_bound_hsize));
		m_boids.push_back(Boid(pos, glm::sphericalRand(1.0), 0, i));
	}

	float radiuses[3] = { 5.f, 3.9, 2.f };
	for (int i = 0; i < sphereNum; i++) {
		glm::vec3 pos = glm::linearRand(glm::vec3(-m_bound_hsize/4.5f), glm::vec3(m_bound_hsize/4.5f));
		m_spheres.push_back(Sphere(pos, radiuses[i], i));
	}
}


void Scene::update(float timestep) {
	for (Boid &b : m_boids) {
		b.calculateForces(this);
	}

	for (Boid &b : m_boids) {
		b.update(timestep, this);
	}

	for (Predator &p : m_predators) {
		p.calculateForces(this);
	}

	for (Predator &p : m_predators) {
		p.update(timestep, this);
	}
}


void Scene::draw(const glm::mat4 &proj, const glm::mat4 &view) {

	// draw skymap (magically)
	//
	if (m_show_skymap) {
		static GLuint tex = 0;
		if (!tex) {
			int w, h, n;
			stbi_set_flip_vertically_on_load(true);
			unsigned char *img = stbi_load("work/res/textures/sky.jpg", &w, &h, &n, 3);
			if (!img) throw std::runtime_error("Failed to load image ");
			else {
				glGenTextures(1, &tex);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(img);
			}
		}
		glUseProgram(m_skymap_shader);
		glUniformMatrix4fv(glGetUniformLocation(m_skymap_shader, "uProjectionMatrix"), 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(m_skymap_shader, "uModelViewMatrix"), 1, false, glm::value_ptr(view));
		glUniform1f(glGetUniformLocation(m_skymap_shader, "uZDistance"), 1000.0f);
		glActiveTexture(GL_TEXTURE0); // Set the location for binding the texture
		glBindTexture(GL_TEXTURE_2D, tex); // Bind the texture
		glUniform1i(glGetUniformLocation(m_skymap_shader, "uSkyMap"), 0);  // Set our sampler (texture0) to use GL_TEXTURE0 as the source
		draw_dummy(12);
	}

	// draw axis (magically)
	//
	if (m_show_axis) {
		// load shader and variables
		glUseProgram(m_axis_shader);
		glUniformMatrix4fv(glGetUniformLocation(m_axis_shader, "uProjectionMatrix"), 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(m_axis_shader, "uModelViewMatrix"), 1, false, glm::value_ptr(view));
		glUniform1f(glGetUniformLocation(m_axis_shader, "uAxisLength"), 1000.0f);
		draw_dummy(6);
	}

	// draw the aabb (magically)
	//
	if (m_show_aabb) {
		glUseProgram(m_aabb_shader);
		glUniformMatrix4fv(glGetUniformLocation(m_aabb_shader, "uProjectionMatrix"), 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(m_aabb_shader, "uModelViewMatrix"), 1, false, glm::value_ptr(view));
		glUniform3fv(glGetUniformLocation(m_aabb_shader, "uColor"), 1, glm::value_ptr(glm::vec3(0.8, 0.8, 0.8)));
		glUniform3fv(glGetUniformLocation(m_aabb_shader, "uMax"), 1, glm::value_ptr(m_bound_hsize));
		glUniform3fv(glGetUniformLocation(m_aabb_shader, "uMin"), 1, glm::value_ptr(-m_bound_hsize));
		draw_dummy(12);
	}


	// draw boids
	//
	for (const Boid &b : m_boids) {

		// get the boid direction (default to z if no velocity)
		glm::vec3 dir = normalize(b.velocity());
		if (dir.x != dir.x) dir = glm::vec3(0, 0, 1);

		// calculate the model matrix
		glm::mat4 model(1);

		// rotate the model to point it in the direction of its velocity

		// pitch rotation
		if (dir.y != 0) {
			float angle = -asin(dir.y);
			model = glm::rotate(glm::mat4(1), angle, glm::vec3(1, 0, 0)) * model;
		}

		// yaw rotation
		if (dir.x != 0 || dir.z != 0) {
			float angle = atan2(dir.x, dir.z);
			model = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * model;
		}

		// translate the model to its worldspace position

		// translate by m_position
		model = glm::translate(glm::mat4(1), b.position()) * model;

		// calculate the modelview matrix
		glm::mat4 modelview = view * model;

		// load shader and variables
		glUseProgram(m_color_shader);
		glUniformMatrix4fv(glGetUniformLocation(m_color_shader, "uProjectionMatrix"), 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(m_color_shader, "uModelViewMatrix"), 1, false, glm::value_ptr(modelview));
		glUniform3fv(glGetUniformLocation(m_color_shader, "uColor"), 1, glm::value_ptr(b.color()));

		// draw
		m_boid_mesh.draw();
	}
	for (const Predator &p : m_predators) {

		// get the boid direction (default to z if no velocity)
		glm::vec3 dir = glm::normalize(p.p_velocity);
		if (dir.x != dir.x) dir = glm::vec3(0, 0, 1);

		// calculate the model matrix
		glm::mat4 model(1);

		// rotate the model to point it in the direction of its velocity

		// pitch rotation
		if (dir.y != 0) {
			float angle = -asin(dir.y);
			model = glm::rotate(glm::mat4(1), angle, glm::vec3(1, 0, 0)) * model;
		}

		// yaw rotation
		if (dir.x != 0 || dir.z != 0) {
			float angle = atan2(dir.x, dir.z);
			model = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * model;
		}

		// translate the model to its worldspace position

		// translate by m_position
		model = glm::translate(glm::mat4(1), p.p_position) * model;
		// calculate the modelview matrix
		glm::mat4 modelview = view * model;

		// load shader and variables
		glUseProgram(m_color_shader);
		glUniformMatrix4fv(glGetUniformLocation(m_color_shader, "uProjectionMatrix"), 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(m_color_shader, "uModelViewMatrix"), 1, false, glm::value_ptr(modelview));
		glUniform3fv(glGetUniformLocation(m_color_shader, "uColor"), 1, glm::value_ptr(p.color()));

		// draw
		m_predator_mesh.draw();
	}
	
	for (const Sphere &s : m_spheres) {

		// calculate the model matrix
		glm::mat4 model(1);

		// rotate the model to point it in the direction of its velocity

		// translate the model to its worldspace position
		// translate by m_position
		model = glm::translate(glm::mat4(1), s.m_centre) * model;
		model = glm::scale(glm::mat4(1), glm::vec3(s.m_radius)) * model;
		// calculate the modelview matrix
		glm::mat4 modelview = view * model;

		// load shader and variables
		glUseProgram(m_color_shader);
		glUniformMatrix4fv(glGetUniformLocation(m_color_shader, "uProjectionMatrix"), 1, false, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(m_color_shader, "uModelViewMatrix"), 1, false, glm::value_ptr(modelview));
		glUniform3fv(glGetUniformLocation(m_color_shader, "uColor"), 1, glm::value_ptr(s.color()));

		// draw
		m_sphere_mesh.draw();
	} 
}


void Scene::renderGUI() {

	if (ImGui::Button("Core", ImVec2(80, 0))) { loadCore(); }
	ImGui::SameLine();
	if (ImGui::Button("Completion", ImVec2(80, 0))) { loadCompletion(); }
	ImGui::SameLine();
	if (ImGui::Button("Challenge", ImVec2(80, 0))) { loadChallenge(); }

	ImGui::Checkbox("Draw Bound", &m_show_aabb);
	ImGui::Checkbox("Draw Axis", &m_show_axis);
	ImGui::Checkbox("Draw Skybox", &m_show_skymap);

	//-------------------------------------------------------------
	// [Assignment 3] :
	// Add ImGui sliders for controlling boid parameters :
	// Core :
	// - boid min speed
	// - boid max speed
	// - boid max acceleration
	// - boid neighbourhood size
	// - boid cohesion weight
	// - boid alignment weight
	// - boid avoidance weight
	// - bounding mode (optional)
	// Completion :
	// - boid anti-predator weight
	// - predator min speed
	// - predator max speed
	// - predator max acceleration
	// - predator neighbourhood size
	// - predator avoidance weight
	// - predator chase weight
	//-------------------------------------------------------------
	
	ImGui::SliderFloat3("Bound hsize", glm::value_ptr(m_bound_hsize), 0, 100.0, "%.0f");

	static int boidNumber; 
	if (ImGui::SliderInt("Number of boids (total)", &boidNumber, 100, 300)) {
		boidNum = boidNumber * 1.0f;
	}

	static int predatorNumber;
	if (ImGui::SliderInt("Number of predator boids", &predatorNumber, 0, 5)) {
		predNum = predatorNumber * 1.0f;
	}

	static float min;
	if (ImGui::SliderFloat("Minimum speed", &min, 0, 2)) {
		minSpeed = min;
	}
	
	static float maxS;
	if (ImGui::SliderFloat("Maximum speed", &maxS, 2, 6)) {
		maxSpeed = maxS;
	}

	static float maxA;
	if (ImGui::SliderFloat("Maximum acceleration", &maxA, 0, 4)) {
		accel = maxA;
	}
	
	static float nSize;
	if (ImGui::SliderFloat("Neighbourhood size", &nSize, 10, 20)) {
		neighbourhood = nSize;
	}
	
	static float cohesion;
	if (ImGui::SliderFloat("Cohesion", &cohesion, -1, 1)) {
		cohereAmount = cohesion;
	}
	
	static float alignment;
	if (ImGui::SliderFloat("Alignment", &alignment, -1, 1)) {
		alignAmount = alignment;
	}
	static float avoidance;
	if (ImGui::SliderFloat("Avoid amount", &avoidance, -1, 1)) {
		avoidAmount = avoidance;
	}

	
	static int bound_Type;
	if (ImGui::SliderInt("Bound type", &bound_Type, 1, 3)) {
		boundType = bound_Type;
	}

	ImGui::Separator();
	/* Completion slides */	
	static float predMinSpeed;
	if (ImGui::SliderFloat("Predator minimum speed", &predMinSpeed, 0, 3)) {
		predatorMinSpeed = predMinSpeed;
	}
	
	static float predMaxSpeed;
	if (ImGui::SliderFloat("Predator maximum speed", &predMaxSpeed, 3, 8)) {
		predatorMaxSpeed = predMaxSpeed;
	}
	
	static float predMaxAccel;
	if (ImGui::SliderFloat("Predator acceleration speed", &predMaxAccel, 0, 2)) {
		predatorMaxAccel = predMaxAccel;
	}
	
	static float predNeighSize;
	if (ImGui::SliderFloat("Boid-predator view size", &predNeighSize, 0, 25)) {
		predatorNeighSize = predNeighSize;
	}
	
	static float predAvoidWeight;
	if (ImGui::SliderFloat("Predator avoid weight", &predAvoidWeight, -1, 3)) {
		predatorAvoidWeight = predAvoidWeight;
	}
	
	static float predChaseWeight;
	if (ImGui::SliderFloat("Predator chase weight", &predChaseWeight, -1, 1)) {
		predatorChaseWeight = predChaseWeight;
	}
}
