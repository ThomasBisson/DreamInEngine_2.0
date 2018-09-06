#ifndef DREAMINENGINE_SPRITE_H
#define DREAMINENGINE_SPRITE_H

#include <components/Texture.h>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec3.hpp>

class Sprite
{
public:
	Sprite(Texture texture, glm::vec2 position, glm::vec2 size = glm::vec2(32.0f, 32.0f), glm::vec2 velocity = glm::vec2(0.0f), GLfloat rotation = 0.0f, glm::vec3 color = glm::vec3(1.0f))
		: Texture(texture), Position(position), Size(size), Velocity(velocity), Rotation(rotation), Color(color) {}

	Texture Texture;

	glm::vec2 Position, Size, Velocity; // Scale ?

	GLfloat Rotation;

	glm::vec3 Color;
};

#endif // DREAMINENGINE_SPRITE_H
