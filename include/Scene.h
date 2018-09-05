#pragma once
#include <GLEW/glew.h>
#include "SpriteRenderer.h"

class Scene
{
public:
	int Keys[1024];

	GLuint Width, Height;

	SpriteRenderer *Renderer;

	Scene(GLuint width, GLuint height);

	~Scene();

	void Init();

	void ProcessInput(GLfloat dt);

	void Update(GLfloat dt);

	void Render();

	void Close();
};
