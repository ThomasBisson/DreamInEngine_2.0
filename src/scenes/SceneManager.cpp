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
#include <utility>

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
		/*glm::ortho(static_cast<GLfloat>((-m_glfw_environment->get_width()) / 2), static_cast<GLfloat>(m_glfw_environment->get_width() / 2), static_cast<GLfloat>(-m_glfw_environment->get_height() / 2), static_cast<GLfloat>(m_glfw_environment->get_height() / 2), -1.0f, 1.0f) // Normalized CENTER CENTER
	*/ };

	// NOTE: the call is written as "API friendly" => Easier to configure the shader (and faster !)
	// e.g. here, only one static call to "GetShader", instead of 4 !
	ResourceManager::GetShader("sprite_shader").Use()
		.SetInteger("image", 0)
		.SetInteger("projectionType", PROJECTION_NORMALIZED_TOP_LEFT)
		.SetMatrix4("projection", projections[PROJECTION_NORMALIZED_TOP_LEFT]);

	// Load Renderers
	spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("sprite_shader"));
}

bool SceneManager::init() {
	bool success = true;

	// Container (Entity ID: 0)
	this->add_component(SPRITE, 0);

	// Face (Entity ID: 1)
	this->add_component(SPRITE, 1);
	this->get_component<Sprite>(SPRITE, 1)->Texture = ResourceManager::GetTexture("face");
	this->get_component<Sprite>(SPRITE, 1)->Position = glm::vec2(50.0, 300.0f);
	this->add_component(INPUT, 1);

	// Pokeball (Entity ID: 2)
	this->add_component(SPRITE, 2);
	this->get_component<Sprite>(SPRITE, 2)->Texture = ResourceManager::GetTexture("pokeball");
	this->get_component<Sprite>(SPRITE, 2)->Position = glm::vec2(50.0, 500.0f);

	//this->get_components(0); // entity 0's component mask

	// [CRITICAL] TODO: solve the case where a component (ex: Sprite) can't be updated (Position/Rotation/etc..) while having an attached Box2D collider
	// TODO: [PB] the first time we click on "Play" Button, all the entities with a Box2D component have their params "reseted" to 0 (because of BoxPhysics default values)
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

void SceneManager::run() const {
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
		int x = m_ImGui_HUD->m_window_scene.w; // + (m_ImGui_HUD->m_window_scene.x); This one is for responsive design
		int y = m_ImGui_HUD->m_window_explorer.h + (m_glfw_environment->get_height() * 0.030f); // optional, only to have square textures
		unsigned int width = m_ImGui_HUD->m_window_entity.x - x; // NOTE: Not very reliable..
		unsigned int height = (m_glfw_environment->get_height() - m_ImGui_HUD->m_window_explorer.h) - m_ImGui_HUD->m_window_menubar.h - (m_glfw_environment->get_height() * 0.060f); // *0.035f to have square textures

		// NOTE: (Re)Define the zone where OpenGL can Draw/Render things
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, y, width, height); // Redefine the OpenGL's drawable zone
		glViewport(x, y, width, height);
		glClearColor(0.6f, 0.6f, 0.6f, 1.0f); // Clear only the defined zone
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);

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

		// TODO: From GLFW ?
		glfwSwapBuffers(m_glfw_environment->get_window());

		// TODO: call this from GLFWEnvironment (adapt the calling update_viewport method ?)
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

//@TODO: see if there is no memory leaks => Do Tests
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

////////////////////////////// COMPONENTS MANAGEMENT //////////////////////////////////////////
// TODO: Create special "Boolean type structure" to put as return type => Get an error message (pretty easily) in ImGui :D
// Usage example (in ImGui): m_scene_manager->add_component(TEST, 0).message
bool has_component(unsigned int entity_mask, unsigned int component_mask)
{
	return (entity_mask & component_mask) == component_mask;
}

BooleanCustom SceneManager::add_component(unsigned int component_type, unsigned int entity_id)
{
	// check if entity_id is valid (in entity list range)
	if (entity_id >= this->getActualScene().getEntities().size())
	{
		return BooleanCustom(false, "[out of range] Entity ID does not exists");
	}

	// Add component type to entity's mask
	auto &entity_mask = this->getActualScene().getEntities()[entity_id].mask;

	if (component_type == SPRITE)
	{
		if (has_component(entity_mask, SPRITE))
		{
			// Sprite component already attached to entity
			return BooleanCustom(false, "Error: Could not add Sprite component.\nThis entity already have a Sprite component");
		}
		else
		{
			this->new_sprite(entity_id);
			this->getActualScene().getEntities()[entity_id].mask |= component_type; // TODO: Simplify call or make a reference call
			return BooleanCustom(true, "Successfully added Sprite component");
		}
	}

	// TODO: Put Box2D parameters via the mask
	if (component_type == BOX2D) // Can only have physics if the entity has a Sprite
	{
		if (!has_component(entity_mask, SPRITE))
			return BooleanCustom(false, "Error: Could not add BoxPhysics component\nPlease add a Sprite component first !");

		if (has_component(entity_mask, BOX2D))
		{
			// BoxPhysics component already attached to entity
			return BooleanCustom(false, "Error: Could not add BoxPhysics component.\nThis entity already have a BoxPhysics component");
		}
		else
		{
			// TODO: Thomas, you can solve the BoxPhysics no-param constructor here ;D
			//this->getActualScene().getBoxPhysics().add(new BoxPhysics(), entity_id);
			this->getActualScene().getEntities()[entity_id].mask |= component_type;
			return BooleanCustom(true, "Successfully added BoxPhysics component");
		}
	}
	if (component_type == INPUT)
	{
		if (!has_component(entity_mask, SPRITE))
			return BooleanCustom(false, "Error: Could not add BoxPhysics component\nPlease add a Sprite component first !");

		if (has_component(entity_mask, INPUT))
		{
			// Input component already attached to entity
			return BooleanCustom(false, "Error: Could not add Input component.\nThis entity already have an Input component");
		}
		else
		{
			this->addInput(this->getActualScene().getEntities()[entity_id]);
			this->getActualScene().getEntities()[entity_id].mask |= component_type;
			return BooleanCustom(true, "Successfully added Input component");
		}
	}

	return BooleanCustom(false, "Could not add component: Unknown Type");
}

// NOTE: In fact, it gets all Components TYPES
std::vector<unsigned int> SceneManager::get_components(unsigned int entity_id)
{
	std::vector<unsigned int> entity_component_masks;

	unsigned int entityMask = this->getActualScene().getEntities()[entity_id].mask;
	unsigned int component_type = 1;

	for (int i = 0; component_type << i < COMPONENT_TOTAL; ++i)
	{
		unsigned int component_mask = component_type << i;
		if (has_component(entityMask, component_mask))
		{
			entity_component_masks.emplace_back(component_mask);
		}
	}

	return entity_component_masks;
}

// NOTE: Even if it's a generic (return) type, we STILL need to provide the "ComponentType" parameter..
// TODO: Find a way, if possible, to call the method without the need of the "ComponentType" parameter
// TODO: Correct bugs when/if there is no component to find !
// [IMPORTANT] TODO: Replace return type "T*" by "std::shared_ptr<T>" 
// NOTE: This is far from being optimized with all these conditions..
template <typename T>
T* SceneManager::get_component(ComponentType component_type, unsigned entity_id)
{
	const unsigned int entity_mask = this->getActualScene().getEntities()[entity_id].mask;

	if (component_type == SPRITE)
	{
		if (has_component(entity_mask, SPRITE)) return (T*)this->getActualScene().getSprites().get(entity_id);
	}
	if (component_type == BOX2D)
	{
		if (has_component(entity_mask, BOX2D)) return (T*)this->getActualScene().getBoxPhysics().get(entity_id);
	}
	if (component_type == INPUT)
	{
		if (has_component(entity_mask, INPUT)) return (T*)(this->getActualScene().getInputs().get(entity_id));
	}

	return (T*)nullptr;
}
