#version 450
#extension GL_KHR_vulkan_glsl : enable

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
layout(set = 0, binding = 0) uniform CameraUniforms {
    mat4 proj;
	mat4 view;
	vec4 viewEye;
} camera;

layout(set = 1, binding = 0) uniform GeometryUniforms {
	mat4 world;
	mat4 normal;
} geometry;

//Uniform Light Buffer.
layout(set = 2, binding = 0) uniform LightUniforms {
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

layout(set = 2, binding = 1) uniform sampler2D shadowMaps[4]; 

//Uniform Material buffer
layout(set = 3, binding = 0) uniform MaterialUniforms {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;
} material;

layout(set = 3, binding = 1) uniform sampler2D textures[11]; 

/*
vec2 positions[3] = vec2[](
    vec2(1.0, -1.5),
    vec2(1.5, 1.5),
    vec2(-1.5, 1.5)
);

vec3 ccolors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);
*/

void main()
{
	vec4 vertex = vec4(vertices, 1.0);
	fColor = colors;
    gl_Position = camera.proj * camera.view * geometry.world * vertex;

	///*
	//mat4 tproj = mat4(
	//	2.083, 0.000,  0.000,  0.000,
	//	0.000, 3.125,  0.000,  0.000,
	//	0.000, 0.000, -1.000, -1.000,
	//	0.000, 0.000, -0.020,  0.000
	//);
	////*/
	///*
	//mat4 tproj = mat4(
	//	0.1, 0.0,  0.0, 0.0,
	//	0.0, 0.1,  0.0, 0.0,
	//	0.0, 0.0, -0.5, 0.0,
	//	0.0, 0.0,  0.0, 1.0
	//);
	////*/
	//
	//mat4 tview = mat4(
	//	1.0, 0.0,   0.0, 0.0,
	//	0.0, 1.0,   0.0, 0.0,
	//	0.0, 0.0,  -1.0, 0.0,
	//	0.0, 0.0, -10.0, 1.0
	//);
	//
	//fColor = vec4(0.0, 0.0, 0.0, 1.0);
	//if (abs(camera.proj[0][0] - 2.083) < 0.01f) {
	//	fColor.r = 1.0;
	//}
	//if (abs(camera.proj[1][1] - 3.125) < 0.01f) {
	//	fColor.g = 1.0;
	//}
	//if (abs(camera.proj[2][3] + 0.020) < 0.01f) {
	//	fColor.b = 1.0;
	//}
	//gl_Position = camera.proj * camera.view * vertex;
	//gl_Position = camera.proj * camera.view * vec4(positions[gl_VertexIndex], 0.5, 1.0);
    //fColor = vec4(ccolors[gl_VertexIndex], 1.0);
}