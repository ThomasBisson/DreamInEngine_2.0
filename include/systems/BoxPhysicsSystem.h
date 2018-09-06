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
				//std::cout << it->first << ":" << it->second << std::endl;
				//std::cout << itSprite->first << ":" << itSprite->second << std::endl;
				//std::cout << sprites.getMap().end()->first << ":" << sprites.getMap().end()->second << std::endl;

				Sprite* sprite = sprites.get(itSprite->first);
				BoxPhysics* box = boxs.get(itSprite->first);

				//std::cout << sprite->get_position().x << ":" << sprite->get_position().y << std::endl;
				std::cout << box->getBody()->GetWorldCenter().x << ":" << box->getBody()->GetWorldCenter().y << std::endl;
				sprite->Position.x = box->getBody()->GetWorldCenter().x - (sprite->Size.x / 720) / 2;
				sprite->Position.y = box->getBody()->GetWorldCenter().y - (sprite->Size.y / 1024) / 2;
				//std::cout << sprite->get_position().x << ":" << sprite->get_position().y << std::endl;
			}
		}
		int a = 0;
		a++;
	}
};


#endif //DREAMINENGINE_BOXPHYSICSSYSTEM_H
