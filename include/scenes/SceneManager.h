//
// Created by ThomasBI on 30/01/2018.
//

#ifndef DREAMINENGINE_SCENEMANAGER_H
#define DREAMINENGINE_SCENEMANAGER_H

/** NOTE: [Sprite <=> Texture + Position(s)]
 *  Sprite might be a mix of Texture and vertices/indices/text_coords,
 *  grouped together in a single structure
 */

 /** NOTE: 5 differents cordinate systems:
  * Local space (or Object space)
  * World space
  * View space (or Eye space)
  * Clip space
  * Screen space
  */

  // @TODO: Move those "deprecated includes"
#include <scenes/Scene.h>
#include <SpriteRenderer.h>

class ImGuiHUD;

class SpriteRenderer;

class GLFWEnvironment;

enum MeshType {
	TRIANGLE = 0, // @TODO: Not Implemented..
	SQUARE = 1,
};

class SceneManager {

private:
	/* Environments, Texture & Shader Loaders */
	GLFWEnvironment *m_glfw_environment;
	//ImGuiHUD *m_ImGui_HUD;

	/* Locations & Matrices */
	std::string m_actualScene;
	std::vector<Scene *> m_scenes;
	std::unordered_map<std::string, unsigned int> m_index_scene;

	SpriteRenderer *spriteRenderer;
public:

	SceneManager(GLFWEnvironment *glfw_environment);

	/* Start
	 * Begins SceneManager's initialization process.
	 * If the initialization is successful, runs its content */
	void start();

	/* Init
	 * Initialize this manager's GL, GLFW and ImGUI context
	 * Also initialize a shader manager and a texture loader
	 * Returns true if the initialization was successful, otherwise it returns false */
	bool init();

	/* Run
	 * Run this manager to start updating and rendering */
	void run();

	/* Create Scene
	 * Create a new scene with a given name
	 * The newly created scene is added to the SceneManager's scene pool */
	void createScene(const std::string &nameScene);

	//@TODO: check if the name corresponds to an existant scene: if not, returns a false boolean value with an error message
	/* Select Scene
	 * Select the scene with the given name
	 * [...] */
	void selectScene(const std::string &nameScene);

	//@TODO: Idem
	/* Change Scene Name
	 * Modify the name of the scene
	 * [...] */
	void changeSceneName(std::string oldName, std::string newName);

	//@TODO: Idem
	/* Erase Scene
	 * Erase the scene which has the good name */
	void eraseScene(std::string nameScene);

	//@TODO: Idem
	/* Get Scene
	 * Call a scene by its name
	 * [...] */
	Scene *getScene(std::string nameScene);

	/* Get Scenes
	 * Get all the scenes from this manager */
	std::vector<Scene *> getScenes();

	/* Get Actual Scene
	 * Get selected / active scene for this manager */
	std::string getActualScene() const;

	/* Get Index Scene
	 * Get a Scene's index from a name */
	std::unordered_map<std::string, unsigned int> getIndexScene() const;

	/* New Sprite
	 * Adds a New Sprite to a specific scene */
	void new_sprite(std::string sceneName, Entity entity, Texture texture, glm::vec2 position = glm::vec2(50.f, 50.f));

	/* Update Sprite
	 * Updates a specific sprites thanks to OpenGL
	 * This method is directly called by one or more scene(s) */
	void update_sprite(Sprite *sprite);

	/* Render Sprite
	 * Renders a specific sprites at the screen thanks to OpenGL
	 * This method is directly called by one or more scene(s) */
	void render_sprite(Sprite *sprite);

	// @NOTE: For Real-Time Game modification
	void addBox2D(std::string sceneName, Entity entity, Sprite *s, bool dynamicBody);

	~SceneManager();
};

#endif //DREAMINENGINE_SCENEMANAGER_H
