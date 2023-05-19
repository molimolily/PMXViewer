#version 150
uniform float aspect;
uniform mat4 modelView;
uniform mat4 projection;
uniform mat3 normalMatrix;
in vec4 position;
in vec3 normal;
in vec2 uv;
in float edgeScale;
out vec4 pos;
out vec3 norm;
void main()
{
	pos = modelView * position;
	gl_Position = projection * pos;
	norm = normalize(normalMatrix * normal);
	// gl_Position = position;
}