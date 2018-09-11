//
// Created by Thomas Bisson and Valentin Dumas on 30/01/2018.
//

#include <HUD/ImGuiHUD.h>

#include <GLEW/glew.h>
#include <GLFWEnvironment.h>
#include <GLFW/glfw3.h>

#include <scenes/SceneManager.h>
#include <ResourceManager.h>
#include <SpriteRenderer.h>

#include <utils/fps.h>

#define IDENTITY_MATRICE 1.0f

SceneManager::SceneManager(GLFWEnvironment *glfw_environment) {
	m_glfw_environment = glfw_environment;
	m_ImGui_HUD = new ImGuiHUD(*this, glfw_environment, true);
	m_ImGui_HUD->init();

	m_actualScene = nullptr;

	m_scenes = std::vector<Scene *>(0);
	m_index_scene = std::unordered_map<std::string, unsigned int>();

	m_runningConfigEnum = CONFIG;

	// TODO: Init projection mat4 ARRAY on the gpu side ! and call rendering to the good projection thanks to the enum id
	// TODO: also see how to do a GLSL array (especially for mat4 arrays)
	// Configure shaders
	// 
	glm::mat4 projections[] = {
		glm::ortho(0.0f, static_cast<GLfloat>(glfw_environment->get_width()), static_cast<GLfloat>(glfw_environment->get_height()), 0.0f, -1.0f, 1.0f), // Normalized TOP LEFT
		glm::ortho(static_cast<GLfloat>((m_glfw_environment->get_width()) / 2), static_cast<GLfloat>(m_glfw_environment->get_width() / 2), static_cast<GLfloat>(m_glfw_environment->get_height() / 2), static_cast<GLfloat>(m_glfw_environment->get_height() / 2), -1.0f, 1.0f) // Normalized CENTER CENTER
	};

	ResourceManager::GetShader("sprite_shader").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite_shader").SetInteger("projectionType", PROJECTION_NORMALIZED_TOP_LEFT); // TODO: Change this !! add an enum type
	ResourceManager::GetShader("sprite_shader").SetMatrix4("projection", projections[PROJECTION_NORMALIZED_TOP_LEFT]);

	// Load Renderers
	spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("sprite_shader"));
}

bool SceneManager::init() {
	bool success = true;

	// @TODO: Do initialization stuff here !
	this->add_component(SPRITE, 0);
	//this->get_components(0); // entity 0's component mask
	this->add_component(SPRITE, 1);
	this->add_component(INPUT, 1);

	//this->new_sprite(this->getActualScene().getEntities()[0].id, ResourceManager::GetTexture("container"), glm::vec2(50.0f, 50.0f));
	//this->new_sprite(this->getActualScene().getEntities()[1].id, ResourceManager::GetTexture("face"), glm::vec2(50.0f, 300.0f));
	this->new_sprite(this->getActualScene().getEntities()[2].id, ResourceManager::GetTexture("pokeball"), glm::vec2(50.0f, 500.0f));

	// this->getScene("Aloha")->getEntities()[0], this->getScene("Aloha")->getSprites().get(this->getScene("Aloha")->getEntities()[0].id)->Position += 400.0f;

	// [CRITICAL] TODO: solve the case where a component (ex: Sprite) can't be updated (Position/Rotation/etc..) while having an attached Box2D collider
	this->addBox2D(this->getActualScene().getEntities()[0], this->getActualScene().getSprites().get(this->getActualScene().getEntities()[0].id), true);
	//this->addBox2D(this->getScene("Aloha")->getEntities()[1], this->getScene("Aloha")->getSprites().get(this->getScene("Aloha")->getEntities()[1].id), true);
	this->addBox2D(this->getActualScene().getEntities()[2], this->getActualScene().getSprites().get(this->getActualScene().getEntities()[2].id), false);

	return success;
}

void SceneManager::start() {
	if (!init()) {
		std::cout << "Error: Could not initialize SceneManager [init error]" << std::endl;
		return;
	}
	run();
}

void SceneManager::run() {
	while (!m_glfw_environment->quit()) {

		utils::get_fps();

		glfwPollEvents();

		std::vector<InputEnum> inputs = m_glfw_environment->process_input(); // Order ??

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// TODO: Trigger a viewPort update => Update Scene(s) frame to fit the screen
		// TODO: Create window explorer at the bottom of the screen
		// TODO: Add window explorer to viewport's height calculation(s)

		// [IMPORTANT WARNING]: Viewport & Scissor have their ORIGIN (0;0) to LOWER LEFT ! 
		int x = m_ImGui_HUD->m_window_scene.w + m_ImGui_HUD->m_window_scene.x /* This last one is for responsive design ! */;
		int y = m_glfw_environment->get_height() / 7; // 400 <=> window explorer's height
		unsigned int width = m_ImGui_HUD->m_window_entity.x - x; // NOTE: Not very reliable..
		unsigned int height = m_glfw_environment->get_height() - m_glfw_environment->get_height() / 4 - m_ImGui_HUD->m_window_menubar.h; // 400 <=> window explorer's height

		/* (Re)Define the zone where OpenGL can Draw/Render things */
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, y, width, height); // Redefine the OpenGL's drawable zone
		glViewport(x, y, width, height);
		glClearColor(0.6f, 0.6f, 0.6f, 1.0f); // Clear only the defined zone
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);

		//m_glfw_environment->update_viewport();

		m_ImGui_HUD->update();

		if (m_actualScene) // Selected scene
		{
			if (RUNNING == m_runningConfigEnum) {
				m_actualScene->update(inputs);
			}

			// TODO: Test MultiRendering from different views/(<=>viewports)/perspectives
			m_actualScene->render();
		}
		else // No scene selected
		{
		}

		m_ImGui_HUD->render();

		glfwSwapBuffers(m_glfw_environment->get_window());

		glViewport(0, 0, m_glfw_environment->get_width(), m_glfw_environment->get_height());
	}

	// Cleanup
	m_ImGui_HUD->close();
	m_glfw_environment->close();
}

void SceneManager::createScene(const std::string &nameScene) {
	m_scenes.emplace_back(new Scene(this, nameScene));
	m_index_scene[nameScene] = m_scenes.size() - 1;
}

void SceneManager::selectScene(const std::string &nameScene)
{
	for (auto& m_scene : m_scenes)
	{
		if (m_scene->getName() == nameScene)
		{
			m_actualScene = m_scene;
			break;
		}

		// NOTE: no scene found
	}
}

void SceneManager::changeSceneName(std::string oldName, std::string newName) {
	//Change the name of the scene in the vector
	m_scenes[m_index_scene[oldName]]->setName(newName);

	//Change the key in the map
	std::unordered_map<std::string, unsigned int>::iterator i = m_index_scene.find(oldName);
	const unsigned int tmp = i->second;
	m_index_scene.erase(i);
	m_index_scene[newName] = tmp;
}

// Remove the scene from the vector
void SceneManager::eraseScene(std::string nameScene) {
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

std::vector<Scene *> SceneManager::getScenes() const { return m_scenes; }

Scene& SceneManager::getActualScene() const {
	return *m_actualScene;
}

std::unordered_map<std::string, unsigned int> SceneManager::getIndexScene() const { return m_index_scene; }

void SceneManager::new_sprite(unsigned int entity_id, Texture texture, glm::vec2 position) {
	Sprite *sprite = new Sprite(texture, position);

	/* Add newly created Sprite object to the specified Scene */
	this->getActualScene().add_sprite(entity_id, sprite);

	//delete sprite;
	//sprite = nullptr;
}

void SceneManager::update_sprite(Sprite *sprite) {
	//sprite->Position.x += 1;
}

void SceneManager::render_sprite(Sprite *sprite) {
	// TODO: Do mirror towards X-axis too...  => Make a struct to store the Axis
	spriteRenderer->DrawSprite(sprite->Texture, sprite->Position, sprite->Size, sprite->Rotation, sprite->Color, GL_FALSE);
}

// TODO: move to another location
void SceneManager::addBox2D(/*std::string sceneName, */Entity entity, Sprite *sprite, bool dynamicBody) {
	/*m_scenes[m_index_scene[sceneName]]*/m_actualScene->add_box_physics(entity, sprite->Position.x, sprite->Position.y,
		sprite->Size.x, sprite->Size.y, dynamicBody);
}

void SceneManager::addInput(/*std::string sceneName, */Entity entity) {
	/*m_scenes[m_index_scene[sceneName]]*/m_actualScene->add_input(entity);
}

bool has_component(unsigned int entity_mask, unsigned int component_mask)
{
	return (entity_mask & component_mask) == component_mask;
}

bool SceneManager::add_component(ComponentType component_type, unsigned int entity_id)
{
	// TODO: check if entity_id is valid (in getEntities() range);
	if (entity_id >= this->getActualScene().getEntities().size())
	{
		return false;
	}

	auto &entityMask = this->getActualScene().getEntities()[entity_id].mask |= component_type;

	if (component_type == SPRITE)
	{
		this->new_sprite(entity_id);
	}
	// TODO: make a hasComponent(entityMask, componentMask)
	if(has_component(entityMask, SPRITE))
	{
		// TODO: Put Box2D parameters via the mask
		if (component_type == BOX2D) // Can only have physics if the entity has a Sprite
		{
			//this->getActualScene().getBoxPhysics().add(new BoxPhysics(), entity_id);
		}
		if (component_type == INPUT)
		{
			this->addInput(this->getActualScene().getEntities()[entity_id]);
		}
	}

	
	// TODO: not good !
	else
	{
		std::cout << "[Warning] Could not add Component to the entity: type not implemented, yet";
		return false;
	}

	return true;
}

std::vector<unsigned int> SceneManager::get_components(unsigned int entity_id)
{
	std::vector<unsigned int> entity_component_masks;

	auto &actualScene = this->getActualScene();

	// Fake entity component mask

	unsigned int entityMask = this->getActualScene().getEntities()[entity_id].mask;
	unsigned int component_type = 1;

	for (int i = 0; component_type << i < COMPONENT_TOTAL; ++i)
	{
		unsigned int component_mask = component_type << i;
		if ((entityMask & component_mask) == component_mask)
		{
			entity_component_masks.emplace_back(component_mask);
		}
	}

	return entity_component_masks;
}

//@TODO: see if memory leak is fixed => Do Tests
SceneManager::~SceneManager() {
	delete m_ImGui_HUD;
	delete m_glfw_environment;
	delete spriteRenderer;

	m_ImGui_HUD = nullptr;
	m_glfw_environment = nullptr;
	spriteRenderer = nullptr;

	for (auto& m_scene : m_scenes)
	{
		delete m_scene;
		m_scene = nullptr;
	}

	m_scenes.clear();

	m_index_scene.clear();
}
