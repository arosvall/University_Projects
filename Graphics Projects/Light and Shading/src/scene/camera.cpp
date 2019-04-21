
#include <iostream>

// glm
#include <gtc/matrix_transform.hpp>

// project
#include "camera.hpp"
#include "opengl.hpp"

using namespace std;


void Camera::setPositionOrientation(const glm::vec3 &pos, float yaw, float pitch) {
	m_position = pos;
	m_yaw = yaw;
	m_pitch = pitch;

	// update rotation matrix (based on yaw and pitch)
	m_rotation = glm::rotate(glm::mat4(1), m_yaw, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), m_pitch, glm::vec3(1, 0, 0));
}

Ray Camera::generateRay(const glm::vec2 &pixel) {
	//-------------------------------------------------------------
	// [Assignment 4] :
	// Generate a ray in the scene using the camera position,
	// rotation field of view on the y axis (fovy) and the image
	// size. The pixel is given in image coordinates [0, imagesize]
	// This COULD be done by first creating the ray in ViewSpace
	// then transforming it by the position and rotation to get
	// it into worldspace.
	//-------------------------------------------------------------
	
	// YOUR CODE GOES HERE
	// ...
	


	float fovx = (m_image_size.x / m_image_size.y) * m_fovy;
	
	float wy = glm::atan(m_fovy / 2);
	float wx = glm::atan(fovx / 2);

	float px = (pixel.x + 0.5) / m_image_size.x;
	float py = (pixel.y + 0.5) / m_image_size.y;

	float x = ((1 - px) * -wx) + (px * wx);
	float y = ((1 - py) * -wy) + (py * wy);

	glm::vec3 d(x, y, -1);
	glm::vec3 direction(glm::normalize(d));
	glm::vec4 dire(direction, 0);
	dire = m_rotation * dire;
	glm::vec3 dir(dire.x, dire.y, dire.z);
	glm::vec3 ori = m_position;
	Ray ray(ori, dir);


	return ray;
}
