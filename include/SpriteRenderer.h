#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLEW/glew.h>
#include "Shader.h"

class Texture2D;

class SpriteRenderer
{
public:
	SpriteRenderer(Shader shader);

	~SpriteRenderer();

	void DrawSprite(Texture2D &texture,
		glm::vec2 position, 
		glm::vec2 size = glm::vec2(10, 10), 
		GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f),
		GLboolean mirror = GL_FALSE);

private:
	Shader shader;
	GLuint quadVAO;

	void initRenderData();
};

