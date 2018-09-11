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

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("glewInit failed");

	// Load Shaders (program, vertex, fragment)
	ResourceManager::LoadShader("resources/shaders/sprite.vert", "resources/shaders/sprite.frag", nullptr, "sprite_shader");

	// Load textures
	ResourceManager::LoadTexture("resources/textures/container.jpg", GL_TRUE, "container");
	ResourceManager::LoadTexture("resources/textures/awesomeface.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("resources/textures/pokeball_200px.png", GL_TRUE, "pokeball");

	SceneManager *sm = new SceneManager(m_glfw_environment);

	/* Create one Scene in the Scene Manager */
	sm->createScene("Aloha");

	// Select a scene to be active
	sm->selectScene("Aloha");

	/* Get the Scene Manager's first scene */
	sm->getScene("Aloha")->getEntities().emplace_back(Entity("container_entity", 0));
	sm->getScene("Aloha")->getEntities().emplace_back(Entity("face_entity", 1));
	sm->getScene("Aloha")->getEntities().emplace_back(Entity("pokeball_entity", 2));
	std::cout << sm->getScene("Aloha")->getEntities()[0].name << std::endl;

	sm->start();
	
	return 0;
}
