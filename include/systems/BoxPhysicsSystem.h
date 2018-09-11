//
// Created by ThomasBI on 06/03/2018.
//

#ifndef DREAMINENGINE_BOXPHYSICSSYSTEM_H
#define DREAMINENGINE_BOXPHYSICSSYSTEM_H

#include <components/Sprite.h>
#include <utils/ComponentStore.h>
#include <components/BoxPhysics.h>
#include "Box2D/Box2D.h"

class BoxPhysicsSystem {
public:
	void update(ComponentStore<BoxPhysics> boxs, ComponentStore<Sprite> sprites) {
		for (std::unordered_map<unsigned int, unsigned int>::iterator it = boxs.getMap().begin(); it != boxs.getMap().end(); ++it) {
			auto itSprite = sprites.getMap().find(it->first);
			if (itSprite != sprites.getMap().end()) {
				Sprite* sprite = sprites.get(itSprite->first);
				BoxPhysics* box = boxs.get(itSprite->first);

				// TODO: PB, solve the conflict between this update and external "sprite Position updates" (e.g. ImGui graphical position modification);
				// NOTE: the PB is that Box2D reset the sprite's position to its current Body => No sprite position update possible out of this class
				sprite->Position.x = (float) box->getBody()->GetPosition().x;
				sprite->Position.y = (float) box->getBody()->GetPosition().y;
			}
		}
	}
};


#endif //DREAMINENGINE_BOXPHYSICSSYSTEM_H
