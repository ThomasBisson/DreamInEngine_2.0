//
// Created by ValentinDU on 20/02/2018.
//

#include "GLFWEnvironment.h"
#include <GLEW/glew.h>

#include <glfw/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWEnvironment::GLFWEnvironment() {
	m_window = nullptr;
	m_title = "Sugar World";
	m_width = 512;
	m_height = 512;
	m_fullscreen = true;
}

GLFWEnvironment::GLFWEnvironment(const std::string &title, unsigned width, unsigned height, bool fullscreen) {
	m_window = nullptr;
	m_title = title;
	m_width = width;
	m_height = height;
	m_fullscreen = fullscreen;
}

void GLFWEnvironment::process_input() {
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window, true);
	}
	else if (glfwGetKey(m_window, GLFW_KEY_H) == GLFW_PRESS) {
	}
}

int GLFWEnvironment::init() {
	glfwSetErrorCallback(error_callback);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
# endif

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL/*glfwGetPrimaryMonitor()*/, NULL);
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

void GLFWEnvironment::update_viewport() {
	int display_w, display_h;
	glfwGetFramebufferSize(get_window(), &display_w, &display_h);
	m_width = (unsigned)display_w;
	m_height = (unsigned)display_h;
	glViewport(0, 0, m_width, m_height);
}

void GLFWEnvironment::refresh() {
	glfwSwapBuffers(get_window());
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

unsigned GLFWEnvironment::get_width() {
	return m_width;
}

unsigned GLFWEnvironment::get_height() {
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
