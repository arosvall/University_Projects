#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "opengl.hpp"
#include "imgui.h"

#include "cgra/matrix.hpp"
#include "cgra/wavefront.hpp"

#include "ex1.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

void Application::init() {
    // Load the shader program
    // The use of CGRA_SRCDIR "/path/to/shader" is so you don't
    // have to run the program from a specific folder.
    m_program = cgra::Program::load_program(
        CGRA_SRCDIR "/res/shaders/simple.vs.glsl",
        CGRA_SRCDIR "/res/shaders/simple.fs.glsl");

    // Create a view matrix that positions the camera
    // 10 units behind the object
    glm::mat4 viewMatrix(1);
    viewMatrix[3] = glm::vec4(0, 0, -10, 1);
    m_program.setViewMatrix(viewMatrix);
	//set draw wireframe true or false
    // Create the cube mesh
    createCube();
}

void Application::createCube() {
    /************************************************************
     * 2. Create a Mesh                                         *
     *                                                          *
     * Add the remaining triangles for the cube                 *
     ************************************************************/

    // Use the correct number of rows for the full
    // cube.
    cgra::Matrix<double> vertices(36, 3);
    cgra::Matrix<unsigned int> triangles(12, 3);

    // Remember to make sure that the order of the vertices
    // is counter-clockwise when looking at the front of the
    vertices.setRow(0, { -1.0,-1.0,-1.0 }); 
	vertices.setRow(1, { -1.0,-1.0, 1.0 });
	vertices.setRow(2, { -1.0, 1.0, 1.0 }); 

	vertices.setRow(3, { 1.0, 1.0,-1.0 }); 
	vertices.setRow(4, { -1.0,-1.0,-1.0 });
	vertices.setRow(5, { -1.0, 1.0,-1.0 });

	vertices.setRow(6, { 1.0,-1.0, 1.0 });
	vertices.setRow(7, {-1.0, -1.0, -1.0 });
	vertices.setRow(8, { 1.0,-1.0,-1.0 });

	vertices.setRow(9, { 1.0, 1.0,-1.0 });
	vertices.setRow(10, { 1.0,-1.0,-1.0 });
	vertices.setRow(11, { -1.0,-1.0,-1.0 });

	vertices.setRow(12, { -1.0,-1.0,-1.0 });
	vertices.setRow(13, { -1.0, 1.0, 1.0 });
	vertices.setRow(14, { -1.0, 1.0,-1.0 });

	vertices.setRow(15, { 1.0,-1.0, 1.0 });
	vertices.setRow(16, { -1.0,-1.0, 1.0 });
	vertices.setRow(17, { -1.0,-1.0,-1.0 });

	vertices.setRow(18, { -1.0, 1.0, 1.0 });
	vertices.setRow(19, { -1.0,-1.0, 1.0 });
	vertices.setRow(20, { 1.0,-1.0, 1.0 });

	vertices.setRow(21, { 1.0, 1.0, 1.0 });
	vertices.setRow(22, { 1.0,-1.0,-1.0 });
	vertices.setRow(23, { 1.0, 1.0,-1.0 });

	vertices.setRow(24, { 1.0,-1.0,-1.0 });
	vertices.setRow(25, { 1.0, 1.0, 1.0 });
	vertices.setRow(26, { 1.0,-1.0, 1.0 });

	vertices.setRow(27, { 1.0, 1.0, 1.0 });
	vertices.setRow(28, { 1.0, 1.0,-1.0 });
	vertices.setRow(29, { -1.0, 1.0,-1.0 });

	vertices.setRow(30, { 1.0, 1.0, 1.0 });
	vertices.setRow(31, { -1.0, 1.0,-1.0 });
	vertices.setRow(32, { -1.0, 1.0, 1.0 });

	vertices.setRow(33, { 1.0, 1.0, 1.0 });
	vertices.setRow(34, { -1.0, 1.0, 1.0 });
	vertices.setRow(35, { 1.0,-1.0, 1.0 });

	triangles.setRow(0, { 0, 1, 2 });
	triangles.setRow(1, { 3, 4, 5 });
	triangles.setRow(2, { 6, 7, 8 });
	triangles.setRow(3, { 9, 10, 11 });
	triangles.setRow(4, { 12, 13, 14 });
	triangles.setRow(5, { 15, 16, 17 });
	triangles.setRow(6, { 18, 19, 20 });
	triangles.setRow(7, { 21, 22, 23 });
	triangles.setRow(8, { 24, 25, 26 });
	triangles.setRow(9, { 27, 28, 29 });
	triangles.setRow(10, { 30, 31, 32 });
	triangles.setRow(11, { 33, 34, 35 });


    m_mesh.setData(vertices, triangles);
}

void Application::loadObj(const char *filename) {
    cgra::Wavefront obj;
    // Wrap the loading in a try..catch block
    try {
        obj = cgra::Wavefront::load(filename);
    } catch (std::exception e) {
        std::cerr << "Couldn't load file: '" << e.what() << "'" << std::endl;
        return;
    }

    /************************************************************
     * 2. Create a Mesh                                         *
     *                                                          *
     * Use the data in `obj` to create appropriately-sized      *
     * vertex and index matrices and fill them with data.       *
     *                                                          *
     * Assume that all the faces in the file are triangles.     *
     ************************************************************/
	//m_mesh.setDrawWireframe(false); //on true it will draw which is counterintuitive put a button call in it and set the boolean depending on the button press 
    // Replace these with appropriate values
    unsigned int numVertices  = obj.m_positions.size();
    unsigned int numTriangles = obj.m_faces.size();

    cgra::Matrix<double> vertices(numVertices, 3);
    cgra::Matrix<unsigned int> triangles(numTriangles, 3);

    for (size_t i = 0; i < obj.m_positions.size(); i++) {
        // Add each position to the vertices matrix
		vertices.setRow(i, { obj.m_positions[i][0], obj.m_positions[i][1], obj.m_positions[i][2] });
    }

    for (size_t i = 0; i < obj.m_faces.size(); i++) {
        // Add each triangle's indices to the triangles matrix
        // Remember that Wavefront files use indices that start at 1
		std::vector<cgra::Wavefront::Vertex> m_vertices = obj.m_faces[i].m_vertices;
		triangles.setRow(i, { m_vertices[0].m_p - 1, m_vertices[1].m_p - 1, m_vertices[2].m_p - 1 });
    }

    m_mesh.setData(vertices, triangles);
}

void Application::drawScene() {
    // Calculate the aspect ratio of the viewport;
    // width / height
    float aspectRatio = m_viewportSize.x / m_viewportSize.y;
    // Calculate the projection matrix with a field-of-view of 45 degrees
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    // Set the projection matrix
    m_program.setProjectionMatrix(projectionMatrix);
	
	glm::mat4 modelTransform(1.0f);
	glm::vec3 scaleVec(m_scale, m_scale, m_scale);
	modelTransform = glm::translate(modelTransform, m_translation);
	modelTransform = glm::scale(modelTransform, scaleVec);
	
	glm::vec3 p1(m_rotationMatrix[0]);
	glm::vec3 p2(m_rotationMatrix[1]);

	float dot = glm::dot(p1, p2);
	float theta = std::acos(glm::dot(p1, p2));
	glm::vec3 axis = glm::cross(p1, p2);
	if (axis.x == 0 && axis.y == 0 && axis.x == 0) {
	}
	else {
		modelTransform = glm::rotate(modelTransform, theta, axis);
	}
	
    m_program.setModelMatrix(modelTransform);
	
    // Draw the mesh
    m_mesh.draw();
}

void Application::doGUI() {
    ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Shapes");
	static char buf[256] = "";
	ImGui::InputText(".obj file", buf, 256);
	if (ImGui::Button("Load File")) {
		printf("String is now: %s \n", buf);
		loadObj(buf);
	}


    static float rotation[3] = { 0,0,0 };
	if (ImGui::InputFloat3("Rotation", rotation)) {
		glm::vec3 v1(rotation[0], rotation[1], rotation[2]);
		glm::vec3 v2(m_rotationMatrix[0]);
		glm::vec3 p1(glm::normalize(v1));  
		glm::vec3 p2(glm::normalize(v2));
		m_rotationMatrix[0] = p1;
		m_rotationMatrix[1] = p2;
		

	}

	static float arr[3] = { 0,0,0 };
	if (ImGui::InputFloat3("Translation", arr)) {
		for (int i = 0; i < 3; i++) {
			m_translation[i] = arr[i]/15;
		}
	}

	static float f = 0;
	if (ImGui::SliderFloat("Scale", &f, 1, 25)) {
		m_scale = f * 1/5;
		}

	static bool wire = false;
	if (ImGui::Checkbox("Draw wireframe", &wire)) {
		printf(wire ? "true" : "false"); 
		if (wire) {
	//		wire = false;
			m_mesh.setDrawWireframe(true);
		}
		else {
	//		wire = true;
			m_mesh.setDrawWireframe(false);
		}
		printf(wire ? "true" : "false");

	}

    ImGui::End();
}


// Input Handlers

void Application::onMouseButton(int button, int action, int) {
    if (button >=0 && button < 3) {
        // Set the 'down' state for the appropriate mouse button
        m_mouseButtonDown[button] = action == GLFW_PRESS;
    }
}

void Application::onCursorPos(double xpos, double ypos) {

	// Make a vec2 with the current mouse position
    glm::vec2 currentMousePosition(xpos, ypos);

    // Get the difference from the previous mouse position
    glm::vec2 mousePositionDelta = currentMousePosition - m_mousePosition;

    if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_LEFT]) {
		glm::vec2 cartes = (currentMousePosition - (m_viewportSize * .5f)) / (m_viewportSize * .5f);

//		m_translation = glm::vec3(-currentMousePosition.x/500, -currentMousePosition.y/250, 0);
		m_translation = glm::vec3((m_translation.x/4)+cartes.x, (-m_translation.y/4)-cartes.y, 0);
		std::cout << "x is now: " << m_translation[0] << " y is now: " << m_translation[1] << std::endl;
		std::cout << "mouse x is now: " << currentMousePosition[0] << " mouse y is now: " << currentMousePosition[1] << std::endl;
    } else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_MIDDLE]) {
		if (mousePositionDelta.y > 0) {
			m_scale -= 0.05;
		}
		else if (mousePositionDelta.y < 0) {
			m_scale += 0.05;
		}

    } else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_RIGHT]) {
		glm::vec2 v1 = m_mousePosition;
		glm::vec2 v2 = currentMousePosition;
		glm::vec2 normv1 = (v1 - (m_viewportSize * .5f)) / (m_viewportSize * .5f);
		glm::vec2 normv2 = (v2 - (m_viewportSize * .5f)) / (m_viewportSize * .5f);
		float len1 = glm::length(normv1);
		float len2 = glm::length(normv2);
		static glm::vec3 comp1;
		static glm::vec3 comp2;
		
		if(len1 > 1) { 
			glm::vec2 normed = glm::normalize(normv1);
		 	comp1 = glm::vec3(normed.x, normed.y, 0.0f);
		}
		else {
			comp1 = glm::vec3(normv1, glm::sqrt(1 - ((normv1.x*normv1.x) + (normv1.y*normv1.y))));
		}

		if(len2 > 1) {
			glm::vec2 normed = glm::normalize(normv2);
			comp2 = glm::vec3(normed.x, normed.y, 0.0f);
		}
		else {
			comp2 = glm::vec3(normv2.x, -normv2.y, glm::sqrt(1 - ((normv2.x*normv2.x) + (normv2.y*normv2.y))));
		}

		m_rotationMatrix[0] = comp1;
		m_rotationMatrix[1] = comp2;

	}
	// Update the mouse position to the current one
    m_mousePosition = currentMousePosition;
}

void Application::onKey(int key, int scancode, int action, int mods) {
    // `(void)foo` suppresses unused variable warnings
    (void)key;
    (void)scancode;
    (void)action;
    (void)mods;
}

void Application::onScroll(double xoffset, double yoffset) {
    // `(void)foo` suppresses unused variable warnings
    (void)xoffset;
    (void)yoffset;
}
