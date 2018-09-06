//
// Created by Thomas Bisson and Valentin Dumas on 30/01/2018.
//

// @TODO: each loop, scene manager updates the active scene
// @TODO: The active scene is updated: the scene calls scenemanager->update/render to render each sprites !
// @TODO: the rendering job is left to the scene manager, as it maintains shaderManager's and GUI's references !
// @TODO: if necessary, store matrices (projection/view/model) in sprites, but it might be a counterproductive idea..

#include <HUD/ImGuiHUD.h>

#include <GLEW/glew.h>
#include <GLFWEnvironment.h>

#include <scenes/SceneManager.h>
#include <ResourceManager.h>
#include <GLFW/glfw3.h>
#include "SpriteRenderer.h"

#define IDENTITY_MATRICE 1.0f
#define EMPTY_STRING ""

enum GLBufferType {
	GL_ARRAY = GL_ARRAY_BUFFER,
	GL_ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER
};

enum GLDrawStyle {
	STREAM = GL_STREAM_DRAW,
	STATIC = GL_STATIC_DRAW,
	DYNAMIC = GL_DYNAMIC_DRAW
};

SceneManager::SceneManager(GLFWEnvironment *glfw_environment) {
	m_glfw_environment = glfw_environment;
	//m_ImGui_HUD = nullptr;

	// @TODO: Store those attributes into the Sprite class ?
	int m_projection_location = 0;
	int m_view_location = 0;
	int m_model_location = 0;
	int m_transform_location = 0;
	glm::mat4 m_projection = glm::mat4(IDENTITY_MATRICE);
	glm::mat4 m_view = glm::mat4(IDENTITY_MATRICE);
	glm::mat4 m_model = glm::mat4(IDENTITY_MATRICE);
	glm::mat4 m_transform = glm::mat4(IDENTITY_MATRICE);

	m_actualScene = EMPTY_STRING;
	m_scenes = std::vector<Scene *>(0);
	m_index_scene = std::unordered_map<std::string, unsigned int>();

	// TODO: Pass WINDOW Width & Height to manager !
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(1920), static_cast<GLfloat>(1080), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite_shader").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite_shader").SetMatrix4("projection", projection);
	
	// Load Renderers
	spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("sprite_shader"));
}

bool SceneManager::init() {
	bool success = true;

	// @TODO: Do initialization stuff here !

	return success;
}

void SceneManager::start() {
	if (!init()) {
		std::cout << "Error: Could not initialize SceneManager [init error]" << std::endl;
	}
	run();
}

void SceneManager::run() {
	while (!m_glfw_environment->quit()) {
		glfwPollEvents();

		m_glfw_environment->process_input(); // Order ??
		m_glfw_environment->update_viewport();
		m_glfw_environment->clear_screen(0.2, 0.2, 0.2, 1.0);

		//m_ImGui_HUD->update();

		/* Search for active scenes */
		for (auto &active_scene : m_scenes) {
			/* Update active scene */
			active_scene->update();

			/* Render active scene */
			active_scene->render();
		}

		//m_ImGui_HUD->render();

		m_glfw_environment->refresh();
	}

	// Cleanup
	//m_ImGui_HUD->close();
	m_glfw_environment->close();
}

void SceneManager::createScene(const std::string &nameScene) {
	m_scenes.emplace_back(new Scene(this, nameScene));
	m_index_scene[nameScene] = m_scenes.size() - 1;
	//m_index_scene[nameScene] = m_scenes.getActualSize()-1;
}

void SceneManager::selectScene(const std::string &nameScene) { m_actualScene = nameScene; }

void SceneManager::changeSceneName(std::string oldName, std::string newName) {
	//Change the name of the scene in the vector
	m_scenes[m_index_scene[oldName]]->setName(newName);

	//Change the key in the map
	std::unordered_map<std::string, unsigned int>::iterator i = m_index_scene.find(oldName);
	unsigned int tmp = i->second;
	m_index_scene.erase(i);
	m_index_scene[newName] = tmp;
}

void SceneManager::eraseScene(std::string nameScene) {
	//Remove the scene from the vector
	int i = m_index_scene[nameScene];
	m_scenes.at(i) = m_scenes[m_scenes.size() - 1];
	m_scenes.pop_back();


	//Remove the scene from the map
	std::unordered_map<std::string, unsigned int>::iterator it2;
	for (std::unordered_map<std::string, unsigned int>::iterator it = m_index_scene.begin();
		it != m_index_scene.end(); ++it) {
		int ii = it->second;
		if (ii == i) {
			it2 = it;
		}
		else if (ii > i) {
			it->second -= 1;
		}
	}
	m_index_scene.erase(it2);
}

Scene *SceneManager::getScene(std::string nameScene) {
	return m_scenes[m_index_scene[nameScene]];
}

std::vector<Scene *> SceneManager::getScenes() { return m_scenes; }

std::string SceneManager::getActualScene() const {
	return m_actualScene;
}

std::unordered_map<std::string, unsigned int> SceneManager::getIndexScene() const { return m_index_scene; }

void SceneManager::new_sprite(std::string sceneName, Entity entity, Texture texture, glm::vec2 position) {
	Sprite *sprite = new Sprite(texture, position);

	/* Add newly created Sprite object to the specified Scene */
	getScene(sceneName)->add_sprite(entity, sprite);

	//delete sprite;
	//sprite = nullptr;
}

void SceneManager::update_sprite(Sprite *sprite) {

}

void SceneManager::render_sprite(Sprite *sprite) {
	// TODO: Do mirror towards X-axis too...  => Make a struct to store the Axis
	spriteRenderer->DrawSprite(sprite->Texture, sprite->Position, sprite->Size, sprite->Rotation, sprite->Color, GL_FALSE);
}

void SceneManager::addBox2D(std::string sceneName, Entity entity, Sprite *sprite, bool dynamicBody) {
	m_scenes[m_index_scene[sceneName]]->add_box_physics(entity, sprite->Position.x, sprite->Position.y,
		sprite->Size.x, sprite->Size.y, dynamicBody);
}

SceneManager::~SceneManager() {
	//@TODO: fix memory leak
	//m_index_scene.clear();
	//m_scenes.kill();
	//delete m_ImGui_HUD;
	delete m_glfw_environment;
	delete spriteRenderer;
	//m_ImGui_HUD = nullptr;
	m_glfw_environment = nullptr;
	spriteRenderer = nullptr;
	for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it) {
		delete (*it);
		*it = nullptr;
	}
	m_scenes.clear();
}
