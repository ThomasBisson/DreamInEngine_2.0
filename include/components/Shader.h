#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint ID;

	Shader() { }

	Shader  &Use();

	// Compiles the shader from given source code
	void    Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);

	Shader& SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	Shader&    SetInteger(const GLchar *name, GLint value, GLboolean useShader = false);
	Shader&    SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	Shader&    SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
	Shader&    SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	Shader&    SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	Shader&    SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	Shader&    SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
	Shader&    SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
	void    checkCompileErrors(GLuint object, std::string type);
};

#endif
