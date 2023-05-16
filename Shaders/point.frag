#version 150
in vec3 pos;
out vec4 fragment;

void main()
{
	// fragment = vec4(vec3(fwidth(pos)) * 8 , 1.0);
	fragment = vec4(1.0,1.0,1.0,1.0);

	// fragment = vec4(vec3(0.8-fwidth(pos) * 50), 1.0);
	// fragment = vec4(vec3(step(0.9999,1.0-fwidth(pos))), 1.0);
}