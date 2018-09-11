//
// Created by ValentinDU on 20/02/2018.
//

#include "GLFWEnvironment.h"
#include <GLEW/glew.h>

#include <glfw/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	// glViewport(0, 0, width, height);
}

GLFWEnvironment::GLFWEnvironment(bool fullscreen, const std::string &title) {
	m_window = nullptr;
	m_title = title;
	m_width = 0; // defined later
	m_height = 0; // idem (in init())
	m_fullscreen = fullscreen;
}

GLFWEnvironment::GLFWEnvironment(const std::string &title, unsigned width, unsigned height, bool fullscreen) {
	m_window = nullptr;
	m_title = title;
	m_width = width;
	m_height = height;
	m_fullscreen = fullscreen;
	m_viewport_rect = { 0, 0, width, height };
}

std::vector<InputEnum> GLFWEnvironment::process_input() const{
	std::vector<InputEnum> inputs;
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		inputs.push_back(SPACE);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) {
		std::cout << "Z" << std::endl;
		inputs.push_back(ARROW_UP);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		std::cout << "S" << std::endl;
		inputs.push_back(ARROW_DOWN);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		std::cout << "D" << std::endl;
		inputs.push_back(ARROW_RIGHT);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		std::cout << "Q" << std::endl;
		inputs.push_back(ARROW_LEFT);
	}
	return inputs;
}

int GLFWEnvironment::init() {
	glfwSetErrorCallback(error_callback);

	if(!glfwInit())
	{
		std::cout << "Error: Could not initialize GLFW !" << std::endl;
		return 1;
	}

	if(!m_width && !m_height) // Size undefined earlier
	{
		// Determine window size, based on screen resolution
		const GLFWvidmode* resolution = this->get_resolution();
		m_width = resolution->width;
		m_height = resolution->height;
		m_viewport_rect = { 0, 0, m_width, m_height };
	}

#if __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif	
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), (m_fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	if (m_window == 0) {
		// Window or context creation failed
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_window);

	/* Enables V-Sync */
	glfwSwapInterval(1);

	/* register the callback functions after we've created the window and before the game loop is initiated */
	glfwSetFramebufferSizeCallback(glfwGetCurrentContext(), framebuffer_size_callback);
}

void GLFWEnvironment::update_viewport(int x, int y, int width, int height) {
	m_viewport_rect.x = x;
	m_viewport_rect.y = y;
	m_viewport_rect.w = width;
	m_viewport_rect.h = height;
	glViewport(m_viewport_rect.x, m_viewport_rect.y, m_viewport_rect.w, m_viewport_rect.h);
}

int GLFWEnvironment::quit() {
	return glfwWindowShouldClose(get_window());
}

GLFWwindow *GLFWEnvironment::get_window() {
	return glfwGetCurrentContext();
}

std::string &GLFWEnvironment::get_title() {
	return m_title;
}

unsigned int GLFWEnvironment::get_width() const {
	return m_width;
}

unsigned int GLFWEnvironment::get_height() const {
	return m_height;
}

void GLFWEnvironment::close() {
	// Glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
}

void GLFWEnvironment::clear_screen(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

bool GLFWEnvironment::is_fullscreen() {
	return m_fullscreen;
}

void GLFWEnvironment::set_fullscreen(bool fullscreen) {
	m_fullscreen = fullscreen;
}

const GLFWvidmode* GLFWEnvironment::get_resolution() const
{
	return glfwGetVideoMode(glfwGetPrimaryMonitor());
}
