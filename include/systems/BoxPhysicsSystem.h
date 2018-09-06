//
// Created by ThomasBI on 06/03/2018.
//

#ifndef DREAMINENGINE_BOXPHYSICSSYSTEM_H
#define DREAMINENGINE_BOXPHYSICSSYSTEM_H

#include <components/Sprite.h>
#include <utils/ComponentStore.h>
#include <components/BoxPhysics.h>
#include "Box2D/Box2D.h"

#include <iostream>

class BoxPhysicsSystem {
public:
	void update(ComponentStore<BoxPhysics> boxs, ComponentStore<Sprite> sprites) {
		//std::cout << sprites.getVector()[1]->get_position().x << ":" << sprites.getVector()[1]->get_position().y << std::endl;
		for (std::unordered_map<unsigned int, unsigned int>::iterator it = boxs.getMap().begin(); it != boxs.getMap().end(); ++it) {
			auto itSprite = sprites.getMap().find(it->first);
			if (itSprite != sprites.getMap().end()) {
				Sprite* sprite = sprites.get(itSprite->first);
				BoxPhysics* box = boxs.get(itSprite->first);

				sprite->Position.x = (float) box->getBody()->GetPosition().x;
				sprite->Position.y = (float) box->getBody()->GetPosition().y;
			}
		}
		int a = 0;
		a++;
	}
};


#endif //DREAMINENGINE_BOXPHYSICSSYSTEM_H
