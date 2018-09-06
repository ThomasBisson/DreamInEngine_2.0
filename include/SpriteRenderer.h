#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "components/Shader.h"

class Texture;

class SpriteRenderer
{
public:
	SpriteRenderer(Shader shader);

	~SpriteRenderer();

	void DrawSprite(Texture &texture,
		glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10),
		float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f),
		GLboolean mirror = GL_FALSE);

private:
	Shader shader;
	GLuint quadVAO;

	void initRenderData();
};
