//
// Created by ThomasBI on 06/03/2018.
//

#ifndef DREAMINENGINE_ENTITY_H
#define DREAMINENGINE_ENTITY_H

#include <string>

struct Entity {
	std::string name;
	unsigned int id;

	Entity(std::string _name, unsigned int _id) : name(_name), id(_id) {}
};

#endif //DREAMINENGINE_ENTITY_H
