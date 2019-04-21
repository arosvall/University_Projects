#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "opengl.hpp"
#include "imgui.h"

#include "cgra/matrix.hpp"
#include "cgra/wavefront.hpp"

#include "ex1.hpp"
#include "lattice.hpp"

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
	
    // Create the cube mesh
    createCube();
}

void Application::createCube() {

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
	m_resolution = glm::vec3(1);
	cgra::Wavefront obj;
	// Wrap the loading in a try..catch block
	try {
		obj = cgra::Wavefront::load(filename);
	}
	catch (std::exception e) {
		std::cerr << "Couldn't load file: '" << e.what() << "'" << std::endl;
		return;
	}

	unsigned int numVertices = obj.m_positions.size();
	unsigned int numTriangles = obj.m_faces.size();
	vertNum = numVertices;
	cgra::Matrix<double> vertices(numVertices, 3);
	cgra::Matrix<unsigned int> triangles(numTriangles, 3);
	glm::vec3 vmin(0);
	glm::vec3 vmax(0);

	for (size_t i = 0; i < obj.m_positions.size(); i++) {
		vertices.setRow(i, { obj.m_positions[i][0], obj.m_positions[i][1], obj.m_positions[i][2] });
		glm::vec3 v(obj.m_positions[i][0], obj.m_positions[i][1], obj.m_positions[i][2]);
		vmin = glm::min(vmin, v);
		vmax = glm::max(vmax, v);
	}

	for (size_t i = 0; i < obj.m_faces.size(); i++) {
		std::vector<cgra::Wavefront::Vertex> m_vertices = obj.m_faces[i].m_vertices;
		triangles.setRow(i, { m_vertices[0].m_p - 1, m_vertices[1].m_p - 1, m_vertices[2].m_p - 1 });
	}
	
	m_mesh.setData(vertices, triangles);
	m_modified_vertices = vertices;
	m_original_triangles = triangles;
	m_original_vertices = vertices;
	m_lattice = Lattice::Lattice(vmin, vmax, m_resolution);
	storeSphere();
}

void Application::storeSphere() {
	cgra::Wavefront obj;
	static char filename[] = "work/res/models/sphere.obj";
	// Wrap the loading in a try..catch block
	try {
		obj = cgra::Wavefront::load(filename);
	}
	catch (std::exception e) {
		std::cerr << "Couldn't load file: '" << e.what() << "'" << std::endl;
		return;
	}
	printf("Num of vertexes: %d", obj.m_positions.size());
	unsigned int numVertices = obj.m_positions.size();
	unsigned int numTriangles = obj.m_faces.size();
	vertNum = numVertices;
	printf("num of vertices for obj is: %d \n,", numVertices);
	cgra::Matrix<double> vertices(numVertices, 3);
	cgra::Matrix<unsigned int> triangles(numTriangles, 3);

	for (size_t i = 0; i < obj.m_positions.size(); i++) {
		vertices.setRow(i, { obj.m_positions[i][0], obj.m_positions[i][1], obj.m_positions[i][2] });
	}

	for (size_t i = 0; i < obj.m_faces.size(); i++) {
		std::vector<cgra::Wavefront::Vertex> m_vertices = obj.m_faces[i].m_vertices;
		triangles.setRow(i, { m_vertices[0].m_p - 1, m_vertices[1].m_p - 1, m_vertices[2].m_p - 1 });
	}
		m_sphere.setData(vertices, triangles);
		sphereDone = true;
}

void Application::drawScene() {
	int colorID = -1;
	GLuint loc = glGetUniformLocation(
		m_program.glName(), "gColor");
	glUniform1i(loc, colorID);
	
	float aspectRatio = m_viewportSize.x / m_viewportSize.y;
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	m_program.setProjectionMatrix(projectionMatrix);

	modelTransform = glm::mat4 (1.0f);
	glm::mat4 sphereTransform(1.0f);
	modelTransform *= m_rotationMatrix;
	glm::vec3 scaleVec(m_scale, m_scale, m_scale);
	modelTransform = glm::translate(modelTransform, m_translation);
	modelTransform = glm::scale(modelTransform, scaleVec);

	m_program.setModelMatrix(modelTransform);
	// Draw the mesh
	m_mesh.draw(GL_TRIANGLES);

	if (m_lattice.created) {
		m_lattice.draw();
		m_sphere_scale = glm::vec3(0.125);
		for (LatticeNode n : m_lattice.m_nodes) {
			glm::mat4 sphereTransform(1.0f);
			sphereTransform = glm::translate(modelTransform, n.m_position);
			sphereTransform = glm::scale(sphereTransform, m_sphere_scale);
			m_program.setModelMatrix(sphereTransform);
			if (sphereDone) {
				m_sphere.draw(GL_TRIANGLES);
			}
		}
		
	}
}

void Application::doGUI() {
    ImGui::SetNextWindowSize(ImVec2(250, 250), ImGuiSetCond_FirstUseEver);
    ImGui::Begin("Shapes");
	static char buf[256] = "";
	ImGui::InputText(".obj file", buf, 256);
	if (ImGui::Button("Load File")) {
		loadObj(buf);
	}

	static float rotation[3] = { 0,0,0 };
	if (ImGui::InputFloat3("Rotation", rotation)) {
		m_rotationMatrix = glm::eulerAngleXYZ(rotation[0], rotation[1], rotation[2]);

	}

	static float resolution[3] = { 1,1,1 };
	if (ImGui::InputFloat3("Resolution", resolution)) {
		m_resolution = glm::vec3(resolution[0], resolution[1], resolution[2]);
		Lattice latt(m_lattice.m_min, m_lattice.m_max, m_resolution);
		m_lattice = latt;
	}


	static float arr[3] = { 0,0,0 };
	if (ImGui::InputFloat3("Translation", arr)) {
		for (int i = 0; i < 3; i++) {
			m_translation[i] = arr[i]/15;
		}
	}

	static float f = 0;
	if (ImGui::SliderFloat("Scale", &f, 1, 25)) {
		m_scale = 1 + f * 1/5;
		}

	static bool wire = false;
	if (ImGui::Checkbox("Draw wireframe", &wire)) {
		if (wire) {
			m_mesh.setDrawWireframe(true);
		}
		else {
			m_mesh.setDrawWireframe(false);
		}
	}

	static bool reset = false;
	if (ImGui::Checkbox("Reset Sphere data", &reset)) {
		if (reset) {
			m_mesh.setData(m_original_vertices, m_original_triangles);
		}
	}
	static int index = -1;
	static const char * items[] = { "Trilinear Interpolation", "Volumetric Bezier", "Volumetric Catmull-Rom" };
	if (ImGui::Combo("Spatial Deformation Style", &index, items, 3)) {
		switch(index){
		case 0:
			interpType = 1;
			break;
		case 1:
			interpType = 2;
			break;
		case 2:
			interpType = 3;

		}
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
		glm::vec3 mouseVec(mousePositionDelta.x / 10, mousePositionDelta.y / 10, 0);
//		printf("Change in mouseVec = %f, %f", mouseVec.x, mouseVec.y);
//		printf("Mouse click at: %f, %f, \n", currentMousePosition.x, currentMousePosition.y);
		LatticeNode selected(-1, { 0,0,0 });
		bool chosen = false;
		glClearColor(0, 0, 0, 1);
		glClearDepth(1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (LatticeNode n : m_lattice.m_nodes) {
			int colorID = n.m_id;
			GLuint loc = glGetUniformLocation(m_program.glName(), "gColor");
			glUniform1i(loc, colorID);
			glm::mat4 sphereTransform(1.0f);
			sphereTransform = glm::translate(modelTransform, (n.m_position + n.m_offset));
			sphereTransform = glm::scale(sphereTransform, m_sphere_scale);
			m_program.setModelMatrix(sphereTransform);
			if (sphereDone) {
			m_sphere.draw(GL_TRIANGLES);
				
		}
		}

		unsigned char pixel[4];
		glReadPixels(currentMousePosition.x,
			m_viewportSize.y - currentMousePosition.y, 1, 1,
			GL_RGBA, GL_UNSIGNED_BYTE, &pixel);
		int colorID = pixel[0];
		printf("Color id is: %d", colorID);
		if (colorID != 0) {
			for (LatticeNode n : m_lattice.m_nodes) {
				if (n.m_id == colorID) {
					printf("Found the id at : %d \n", n.m_id);
					selected = n;
					chosen = true;
					break;
				}
			}
		}
		if (chosen) {
//			printf("position was at: %f, %f \n", selected.m_position.x, selected.m_position.y);
		
			glm::vec3 change(mousePositionDelta.x / 50.0, -mousePositionDelta.y / 50.0, 0.0);
			m_lattice.recalc(selected, change);
			
			if (interpType == 1) {
//				printf("Entering tri \n");
				Application::deformMeshTri();
			}
			
		}
	}
	
	else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_MIDDLE]) {
	}
	
	else if (m_mouseButtonDown[GLFW_MOUSE_BUTTON_RIGHT]) {
		glm::vec2 v1 = m_mousePosition;
		glm::vec2 v2 = currentMousePosition;
		glm::vec2 normv1 = (v1 - (m_viewportSize * .5f)) / (m_viewportSize * .5f);
		glm::vec2 normv2 = (v2 - (m_viewportSize * .5f)) / (m_viewportSize * .5f);
		float len1 = glm::length(normv1);
		float len2 = glm::length(normv2);
		static glm::vec3 comp1;
		static glm::vec3 comp2;

		if (len1 > 1) {
			glm::vec2 normed = glm::normalize(normv1);
			comp1 = glm::vec3(normed.x, normed.y, 0.0f);
		}
		else {
			comp1 = glm::vec3(normv1, glm::sqrt(1 - ((normv1.x*normv1.x) + (normv1.y*normv1.y))));
		}

		if (len2 > 1) {
			glm::vec2 normed = glm::normalize(normv2);
			comp2 = glm::vec3(normed.x, normed.y, 0.0f);
		}
		else {
			comp2 = glm::vec3(normv2.x, -normv2.y, glm::sqrt(1 - ((normv2.x*normv2.x) + (normv2.y*normv2.y))));
		}

		float dot = glm::dot(comp1, comp2);
		float theta = std::acos(glm::dot(comp1, comp2));
		glm::vec3 axis = glm::cross(comp1, comp2);
		if (glm::length(axis) > 0) {
			m_rotationMatrix = glm::rotate(m_rotationMatrix, theta, axis);
	}

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


void Application::deformMeshTri() {
//	printf("Min is %f, %f, %f, and Max is: %f, %f, %f, \n", m_lattice.m_min.x, m_lattice.m_min.y, m_lattice.m_min.z, m_lattice.m_max.y, m_lattice.m_max.y, m_lattice.m_max.z);
	for (int idx = 0; idx < vertNum; idx++) {
//		printf("Deform position at: %d \n", idx);
		double *vert = m_modified_vertices[idx];
		glm::vec3 v(vert[0], vert[1], vert[2]);
//		printf("Values are: %f, %f, %f \n", v.x, v.y, v.z);
		glm::vec3 latticePos((v - m_lattice.m_min) / (m_lattice.m_max - m_lattice.m_min));
		glm::vec3 cellPos = latticePos * m_lattice.m_resolution;
		glm::vec3 cellMin = glm::floor(cellPos);
		glm::vec3 cellMax = glm::ceil(cellPos);

		float ax = cellMin.x;
		float ay = cellMin.y;
		float az = cellMin.z;
		
		float bx = cellMax.x;
		float by = cellMax.y;
		float bz = cellMax.z;

		float s = (v.x - ax) / (bx - ax);
		float t = (v.y - ay) / (by - ay);
		float u = (v.z - az) / (bz - az);
//		float s = (latticePos.x - ax) / (bx - ax);
//		float t = (latticePos.y - ay) / (by - ay);
//		float u = (latticePos.z - az) / (bz - az);
	
//		float s = (v.x - m_lattice.m_min.x) / (m_lattice.m_max.x - m_lattice.m_min.x);
//		float t = (v.y - m_lattice.m_min.y) / (m_lattice.m_max.y - m_lattice.m_min.y);
//		float u = (v.z - m_lattice.m_min.z) / (m_lattice.m_max.z - m_lattice.m_min.z);

		printf("Line parameters calculated. x axis (s) = %f, y axis (t) = %f, z axis (u) = %f \n", s, t, u);

			glm::vec3 a(ax, ay, az);  //0,0,0
			glm::vec3 e(ax, ay, bz);  //0,0,1
			glm::vec3 d(ax, by, az);  //0,1,0
			glm::vec3 h(ax, by, bz);  //0,1,1
			glm::vec3 b(bx, ay, az);  //1,0,0
			glm::vec3 f(bx, ay, bz);  //1,0,1
			glm::vec3 c(bx, by, az);  //1,1,0
			glm::vec3 g(bx, by, bz);  //1,1,1
//			printf("Now finding the x interpolation points \n");
			
			glm::vec3 i(((1 - s)*a) + (s*b));
			glm::vec3 j(((1 - s)*d) + (s*c));
			glm::vec3 k(((1 - s)*h) + (s*g));
			glm::vec3 l(((1 - s)*e) + (s*f));
//			printf("The x interpolation points are: i: %f, %f, %f, j: %f, %f, %f, k: %f, %f, %f, l: %f, %f, %f \n", i.x, i.y, i.z, j.x, j.y, j.z, k.x, k.y, k.z, l.x, l.y, l.z);

			glm::vec3 m(((1 - t)*i) + (t*j));
			glm::vec3 n(((1 - t)*l) + (t*k));
//			printf("The y interpolation points are: m: %f, %f, %f, n: %f, %f, %f \n", m.x, m.y, m.z, n.x, n.y, n.z);
			glm::vec3 p(((1 - u)*m) + (u*n));
//			printf("The z interpolation points are: p: %f, %f, %f \n", p.x, p.y, p.z);
//			printf("Setting new matrix row at %d, with the above co-ords \n", idx);
			m_modified_vertices.setRow(idx, { p.x, p.y, p.z });
		
		}
		m_mesh.setData(m_modified_vertices, m_original_triangles);
	}

void Application::deformMeshBez() {
	
}

void Application::deformMeshCat() {
/*	for (int a = 0; a < vertNum; a++) {
		double *vert = m_modified_vertices[a];
		glm::vec3 v(vert[0], vert[1], vert[2]);
		glm::vec3 cellPos = v * m_lattice.m_resolution;
		glm::vec3 cellMin = glm::floor(cellPos);
		glm::vec3 cellMax = glm::ceil(cellPos);
		float t = 0;  //x t
		float s = 0;  //y t
		float u = 0;  //z t
		glm::vec3 p0;
		glm::vec3 p1;
		glm::vec3 p2;
		glm::vec3 p3;
				
		if (a == vertNum - 1) {
			double *v2 = m_modified_vertices[a + 1];
			glm::vec3 pB(v2[0], v2[1], v2[2]);
			glm::vec3 l = pB + pB;
			p1 = l - v;

			p2 = l + l - pB;
			//make two vectors for the last vertexes
		}
		else if (a == vertNum - 2) {
			double *v2 = m_modified_vertices[a + 1];
			glm::vec3 pB(v2[0], v2[1], v2[2]);
			p1 = pB;

			glm::vec3 l = pB + pB;
			p2 = l - v;
			
		}
		else {
			double *v2 = m_modified_vertices[a + 1];
			glm::vec3 pB(v2[0], v2[1], v2[2]);
			p2 = pB;
			double *v3 = m_modified_vertices[a + 2];
			glm::vec3 p(v3[0], v3[1], v3[2]);
			p3 = p;
		}

		if (a == 0) { //make two new points 
			p1 = p2 + p2 - p3;
			p0 = p1 + p1 - p2;
		}
		else if (a == 1) {
			//make one new vector
			double *v2 = m_modified_vertices[a - 1];
			glm::vec3 pB(v2[0], v2[1], v2[2]);
			p1 = pB;
			p0 = pB + pB - p2;

		}
		else {
			double *v0 = m_modified_vertices[a - 2];
			glm::vec3 p(v0[0], v0[1], v0[2]);
			p0 = p;
			double *v2 = m_modified_vertices[a - 1];
			glm::vec3 pB(v2[0], v2[1], v2[2]);
			p1 = pB;
		}
		
		float m1x, m2x, m1y, m2y, m1z, m2z;
		m1x = (p2.x - p0.x) / 2;
		m2x = (p3.x - p1.x) / 2;
		m1y = (p2.y - p0.y) / 2;
		m2y = (p3.y - p1.y) / 2; 
		m1z = (p2.z - p0.z) / 2;
		m2z = (p3.z - p1.z) / 2;



	}
	*/
}