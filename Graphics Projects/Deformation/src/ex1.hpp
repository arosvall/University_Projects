#pragma once

#include "cgra/mesh.hpp"
#include "cgra/shader.hpp"
#include "cgra/matrix.hpp"
#include "lattice.hpp"
#include "glm/glm.hpp"

class Application {
public:
    // The window object managed by GLFW
    GLFWwindow *m_window;

    // The shader program used for drawing
    cgra::Program m_program;
    // The mesh data
    cgra::Mesh m_mesh;
	int vertNum;
	cgra::Matrix<double> m_modified_vertices;
	cgra::Matrix<unsigned int> m_original_triangles;
	cgra::Matrix<double> m_original_vertices;
	Lattice m_lattice;
	cgra::Mesh m_sphere;
	glm::mat4 modelTransform;
	int interpType;

	glm::vec3 sphereMoveBy;

    // The current size of the viewport
    glm::vec2 m_viewportSize;
    // The current mouse position
    glm::vec2 m_mousePosition;

    // The translation of the mesh as a vec3
    glm::vec3 m_translation;
    // The scale of the mesh
    float m_scale;
	const char *deformType;

	//resolution vector
	glm::vec3 m_resolution;
	glm::vec3 m_sphere_scale;
	bool sphereDone = false;


    // A 3x3 matrix representing the rotation of the
    // mesh
    glm::mat4 m_rotationMatrix;

    // Whether or not the left, middle or right buttons are down.
    bool m_mouseButtonDown[3];

    Application(GLFWwindow *win)
        : m_window(win),
          m_viewportSize(1, 1), m_mousePosition(0, 0),
          m_translation(0), m_scale(1), m_rotationMatrix(1) {
        m_mouseButtonDown[0] = false;
        m_mouseButtonDown[1] = false;
        m_mouseButtonDown[2] = false;
    }

    void setWindowSize(int width, int height) {
        m_viewportSize.x = float(width);
        m_viewportSize.y = float(height);
    }

    void init();

    void createCube();
    void loadObj(const char *filename);

    void drawScene();
    void doGUI();
	void storeSphere();
    void onKey(int key, int scancode, int action, int mods);

    void onMouseButton(int button, int action, int mods);

    void onCursorPos(double xpos, double ypos);

    void onScroll(double xoffset, double yoffset);

	void deformMeshTri();

	void deformMeshBez();

	void deformMeshCat();
};
