#version 330 core
layout(location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

uniform bool mirror;

void main()
{
	if(mirror) // Mirror, revert x axis
	{
		TexCoords = vec2(-vertex.z, vertex.w);
	}
	else // No mirror
	{
		TexCoords = vec2(vertex.z, vertex.w);
	}
	
	gl_Position = projection * model * vec4(vertex.x, vertex.y, 0.0f, 1.0f);
}