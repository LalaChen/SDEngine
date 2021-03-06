#version 450
//#extension GL_KHR_vulkan_glsl : enable

//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec3 vertices;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 tangents;
layout(location = 4) in vec3 binormals;
layout(location = 5) in vec4 colors;

//output
layout(location = 5) out vec4 fColor;

//Uniform
//layout(set = n, binding = m) for Opengl, we don't assign set. (default set is 0)
//Uniform basic Buffer.
layout(set = 0, binding = 0) uniform BasicUniforms {
    mat4 clip;
    mat4 proj;
	mat4 view;
	mat4 world;
	mat4 normal;
	vec4 viewEye;
} basic;

//Uniform Light Buffer.
layout(set = 0, binding = 1) uniform LightUniforms {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 position; //point light.
	vec4 direction; //direction light.
	float spotExponent;
	float spotCosCutoff;
	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	uint kind; //0: directional, 1: spot, 2: point
} light;

//Uniform Material buffer
layout(set = 1, binding = 0) uniform MaterialUniforms {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;
} material;

layout(set = 1, binding = 1) uniform sampler2D mainTexture; 

void main()
{
	vec4 vertex = vec4(vertices, 1.0);
	fColor = colors;
    gl_Position = basic.clip * basic.proj * basic.view * basic.world * vertex;
}