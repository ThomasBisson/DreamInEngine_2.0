#include <ResourceManager.h>

#include <GLFWEnvironment.h>

#include <scenes/SceneManager.h>
#include <glfw/glfw3.h>

//#define SCREEN_WIDTH 1920
//#define SCREEN_HEIGHT 1080

int main() {
	//GLFWEnvironment* m_glfw_environment = new GLFWEnvironment("DreamIn Engine", 1920, 1080, GL_FALSE);
	GLFWEnvironment* m_glfw_environment = new GLFWEnvironment(false);
	m_glfw_environment->init();

	// TODO: Move this into a GLEWEnvironment/GLEWContext class
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("glewInit failed");
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load Shaders (program, vertex, fragment)
	ResourceManager::LoadShader("resources/shaders/sprite.vert", "resources/shaders/sprite.frag", nullptr, "sprite_shader");

	// Load textures
	ResourceManager::LoadTexture("resources/textures/container.jpg", GL_TRUE, "container");
	ResourceManager::LoadTexture("resources/textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("resources/textures/pokeball_200px.png", GL_TRUE, "pokeball");
	ResourceManager::LoadTexture("resources/textures/buttons/button_play.png", GL_TRUE, "play");
	ResourceManager::LoadTexture("resources/textures/buttons/button_stop.png", GL_TRUE, "stop");
	ResourceManager::LoadTexture("resources/textures/buttons/button_play_and_pause.png", GL_TRUE, "play-pause");
	ResourceManager::LoadTexture("resources/textures/buttons/button_pause.png", GL_TRUE, "pause");
	ResourceManager::LoadTexture("resources/textures/buttons/button_replay.png", GL_TRUE, "replay");
	ResourceManager::LoadTexture("resources/textures/buttons/button_open.png", GL_TRUE, "open");

	SceneManager *sm = new SceneManager(m_glfw_environment);

	// Create a new Scene and add it to the Scene Manager
	sm->createScene("Aloha");

	// Select a scene to be marked as active
	sm->selectScene("Aloha");

	sm->start();

	return 0;
}
