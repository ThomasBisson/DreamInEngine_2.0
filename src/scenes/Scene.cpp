//
// Created by ThomasBI on 30/01/2018.
//

#include <scenes/Scene.h>
#include <scenes/SceneManager.h>

Scene::Scene(SceneManager *sceneManager, const std::string &sceneName) : m_sceneManager(sceneManager),
m_name(sceneName),
m_box_physics_system(),
m_sprites(), m_box_physique(), m_entities() {
	m_world = new b2World(b2Vec2(0.0, -0.1));
}

void Scene::update() {
	//m_sprites[0]->get_position().x += 0.005f;
	//m_sprites[0]->get_position().y += 0.006f;
	//m_sprites[1]->get_position().x += 0.005f;
	//m_sprites[1]->get_position().y -= 0.006f;
	m_box_physics_system.update(m_box_physique, m_sprites);
	m_world->Step(1.0 / 30.0, 8, 3);
}

void Scene::render() {
	update();
	for (auto &sprite : m_sprites.getVector()) {
		m_sceneManager->update_sprite(sprite);
		m_sceneManager->render_sprite(sprite);
	}
}

Scene::~Scene() {
	for (int i = 0; i < m_box_physique.getVector().size(); i++) {
		m_world->DestroyBody(m_box_physique.getVector()[i]->getBody());
	}
	m_sprites.kill();
	m_box_physique.kill();
	delete m_world;
	delete m_sceneManager;
}

void Scene::add_sprite(Entity entity, Sprite *sprite) {
	//m_sprites.emplace_back(sprite);
	m_sprites.add(sprite, entity.id);
}

void Scene::add_box_physics(Entity entity, int x, int y, int w, int h, bool dyn) {
	//m_box_physics_vector.emplace_back(new BoxPhysics());
	//m_box_physics_map[entity.id] = m_box_physics_map.size()-1;

	//m_box_physics_vector[m_box_physics_map.size()-1]->addRec(world,x,y,w,h,dyn);

	m_box_physique.add(new BoxPhysics(), entity.id);
	m_box_physique.get(entity.id)->addRec(m_world, x, y, w, h, dyn);
}

