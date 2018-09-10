//
// Created by ValentinDU on 20/02/2018.
//

#ifndef DREAMINENGINE_GLFWENVIRONMENT_H
#define DREAMINENGINE_GLFWENVIRONMENT_H

#include <iostream>
#include <glm/detail/type_vec2.hpp>

struct GLFWwindow;

struct GLFWvidmode;

// Include Windows
#ifdef WIN32

// Include Mac
#elif __APPLE__
// #define GL3_PROTOTYPES 1
// #include <OpenGL/gl3.h>

// Include UNIX/Linux
// #else
// #define GL3_PROTOTYPES 1
// #include <GL3/gl3.h>
#endif

static void error_callback(int error, const char *description) {
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

class GLFWEnvironment {
public:
	GLFWEnvironment(bool fullscreen = false, const std::string &title = "DreamIn Engine");

	GLFWEnvironment(const std::string &title, unsigned width, unsigned height, bool fullscreen = false);

	// TODO: move this apart cause it might not be its responsibilty (to manage inputs)
	void process_input();

	int init();

	void update_viewport();

	void clear_screen(float r, float g, float b, float a);

	int quit();

	void close();

	GLFWwindow *get_window();

	std::string &get_title();

	unsigned get_width();

	unsigned get_height();

	bool is_fullscreen();

	void set_fullscreen(bool fullscreen);

	const GLFWvidmode* get_resolution() const;
private:
	GLFWwindow * m_window;
	std::string m_title;
	unsigned m_width;
	unsigned m_height;
	bool m_fullscreen = true;
};

#endif //DREAMINENGINE_GLFWENVIRONMENT_H
