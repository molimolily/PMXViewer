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
out vec2 vuv;
void main()
{
	pos = modelView * position;
	norm = normalize(normalMatrix * normal);
	vuv = uv;
	gl_Position = projection * pos;
	// gl_Position = position;
}