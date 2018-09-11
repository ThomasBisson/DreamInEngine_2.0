//
// Created by ThomasBI on 30/01/2018.
//

#ifndef DREAMINENGINE_SCENE_H
#define DREAMINENGINE_SCENE_H

#include <string>
#include <unordered_map>
#include <components/BoxPhysics.h>
#include <entities/Entity.h>
#include <utils/ComponentStore.h>
#include <systems/BoxPhysicsSystem.h>
#include <components/Sprite.h>
#include "utils/InputEnum.h"

class SceneManager;

class Scene {
public:
	Scene(SceneManager *sceneManager, const std::string &sceneName);

	void update(std::vector<InputEnum> inputs);

	void render();

	void add_sprite(unsigned int entity_id, Sprite* sprite);

	void add_box_physics(Entity entity, int x, int y, int w, int h, bool dyn);

	void setName(std::string name) { m_name = name; }

	std::string getName() { return m_name; }

	b2World* getWorld() { return m_world; }

	std::vector<Entity>& getEntities() { return m_entities; }

	ComponentStore<Sprite> getSprites() { return m_sprites; }

	ComponentStore<BoxPhysics> getBoxPhysics() { return m_box_physique; }

	~Scene();
private:
	// Scene data
	std::string m_name;
	SceneManager* m_sceneManager = nullptr;

	// Box2D
	b2World* m_world;
	BoxPhysicsSystem m_box_physics_system;

	// Scene entities
	std::vector<Entity> m_entities;
	//std::unordered_map<unsigned int, unsigned int> m_box_physics_map;
	//std::vector<BoxPhysics*> m_box_physics_vector;
	ComponentStore<BoxPhysics> m_box_physique;
	//std::vector<Sprite*> m_sprites;
	ComponentStore<Sprite> m_sprites;
};

#endif //DREAMINENGINE_SCENE_H
