#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "include/shader.h"
#include "include/ResourceManager.h"
#include <glm/gtc/matrix_transform.inl>

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const GLint SCREEN_WIDTH = 800;
// The height of the screen
const GLint SCREEN_HEIGHT = 600;

//DreamInEngine(SCREEN_WIDTH, SCREEN_HEIGHT);

GLboolean fullscreen = false;

enum ProjectionType
{
	PROJECTION_ORIGIN_CENTER,
	PROJECTION_NORMALIZED_ORIGIN_CENTER,
	PROJECTION_NORMALIZED_ORIGIN_TOP_LEFT,
};

int main(int argc, char *argv[])
{
	ProjectionType projection_type = PROJECTION_ORIGIN_CENTER;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FuryRunner", (fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	glfwMakeContextCurrent(window);

	//glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	glfwSetKeyCallback(window, key_callback);

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	GLfloat startFrame = glfwGetTime(); int nbFrames = 0;

	float vertices[] = {
		// first triangle
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f,  0.5f, 0.0f,  // top left 
		// second triangle
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// vertex attributes (position)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ResourceManager::LoadShader("resources/shaders/square.vert", "resources/shaders/square.frag", nullptr, "square_shader");
	ResourceManager::GetShader("square_shader").Use();

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//nbFrames++;
		//if (currentFrame - startFrame >= 1.0f)
		//{
		//	//printf("%f ms/frame\n", 1000.0 / double(nbFrames));
		//	nbFrames = 0;
		//	startFrame += 1.0;
		//}

		//deltaTime = 0.001f;
		// Manage user input

		// Update Game state

		// Clear Screen before render
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT/* | GLFW_DOUBLEBUFFER*/);

		glm::mat4 projection(1.0f);
		
		if(PROJECTION_ORIGIN_CENTER)
		{
			
		}
		if(PROJECTION_NORMALIZED_ORIGIN_CENTER)
		{
			projection = glm::ortho(static_cast<GLfloat>(SCREEN_WIDTH*(-1) / 2), static_cast<GLfloat>(SCREEN_WIDTH / 2), static_cast<GLfloat>(SCREEN_HEIGHT*(-1) / 2), static_cast<GLfloat>(SCREEN_HEIGHT / 2), -1.0f, 1.0f);
		}
		if(PROJECTION_NORMALIZED_ORIGIN_TOP_LEFT)
		{
			
		}
		ResourceManager::GetShader("square_shader").SetMatrix4("projection", projection);

		// Render
		glm::mat4 model(1.0f);					// POSITION
		model = glm::translate(model, glm::vec3(-310.0f, -150, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
		ResourceManager::GetShader("square_shader").SetMatrix4("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		model = glm::mat4(1.0f);               // x  // y  // z
		model = glm::translate(model, glm::vec3(50.0f, 200.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));
		ResourceManager::GetShader("square_shader").SetMatrix4("model", model);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// glfwSwapInterval(1);
		glfwSwapBuffers(window);

		// get events
		glfwPollEvents();
	}

	// Delete all resources as loaded using the resource manager
	// DreamInEngine.Close();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_REPEAT) return; // Blocks REPEAT behavior

		// DreamInEngine.Keys[key] = action;

		// Did this beause, according to documentation:
		// #define GLFW_RELEASE 0
		// #define GLFW_PRESS 1
		// #define GLFW_REPEAT 2
	}
}