#version 150
uniform float aspect;
uniform mat4 modelView;
uniform mat4 projection;
in vec4 position;
in vec3 normal;
in vec2 uv;
in float edgeScale;
out vec3 pos;
void main()
{
	pos = vec3(position);
	gl_Position = projection * modelView * position;
	// gl_Position = position;
}