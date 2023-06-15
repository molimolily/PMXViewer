#version 150
uniform vec4 diffuseColor;
uniform vec3 specularColor;
uniform float specularity;
uniform vec3 ambientColor;
uniform sampler2D tex;
// uniform vec4 Lpos;
const vec4 Lpos[2] = vec4[](vec4(0.0, 10.0, 2.0, 0.0), vec4(0.0, 5.0, -5.0, 0.0));
// const vec4 Lpos[4] = vec4[](vec4(0.0, 10.0, 2.0, 0.0), vec4(0.0, 5.0, -5.0, 0.0),vec4(2.0, 10.0, 2.0, 0.0), vec4(-2.0, 0.0, -5.0, 0.0));
const vec3 Ldiff = vec3(1.0);
const vec3 Lspec = vec3(0.0);
const vec3 Lamb = vec3(0.4);
in vec4 pos;
in vec3 norm;
in vec2 vuv;
out vec4 fragment;

void main()
{
	// fragment = vec4(vec3(fwidth(norm)) * 10 , 1.0);
	// fragment = vec4(vuv,0.0,1.0);
	// fragment = vec4(norm,1.0);
	// fragment = vec4(1.0,1.0,1.0,1.0);

	// fragment = vec4(vec3(0.8-fwidth(pos) * 50), 1.0);
	// fragment = vec4(vec3(step(0.9999,1.0-fwidth(pos))), 1.0);

	/*vec3 V = -normalize(pos.xyz);

	vec3 Idiff = vec3(0.0);
	vec3 Ispec = vec3(0.0);
	for(int i=0;i<2;i++)
	{
		vec3 L = normalize((Lpos[i] * pos.w - pos * Lpos[i].w).xyz);
		vec3 Iamb = ambientColor * Lamb;
		Idiff += max(dot(norm,L),0.0) * diffuseColor.xyz * Ldiff + Iamb;
		vec3 H = normalize(L + V);
		Ispec += pow(max(dot(norm,H),0.0),specularity) * specularColor * Lspec;
	}*/

	/*vec3 L = normalize((Lpos * pos.w - pos * Lpos.w).xyz);
	vec3 Iamb = ambientColor * Lamb;
	Idiff += max(dot(norm,L),0.0) * diffuseColor.xyz * Ldiff + Iamb;
	vec3 H = normalize(L + V);
	Ispec += pow(max(dot(norm,H),0.0),specularity) * specularColor * Lspec;*/

	vec4 texColor = texture(tex,vuv);

	// fragment = vec4(Idiff + Ispec,1.0);
	// fragment = vec4((Idiff + Ispec) * texColor.rgb, texColor.a);
	fragment = texColor;
	// fragment = vec4(texColor.rgb + Idiff + Ispec,1.0);
	// fragment = vec4(1-texColor.rgb,1.0);
	// fragment = vec4(vec3(vuv,0.0) * (Idiff + Ispec),1.0);
}