#version 450

#extension GL_EXT_multiview : enable
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

//Output varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) out vec2 texCoord;
layout(location = 1) out vec4 wVertex;
layout(location = 2) out vec3 wNormal;
layout(location = 3) out vec3 wLightDir;
layout(location = 4) out vec3 wViewDir;

//Uniform
//layout(set = n, binding = m) for Opengl, we don't assign set. (default set is 0)
//Uniform Camera Buffer.
layout(set = 0, binding = 0) uniform VRCameraUniforms {
    mat4 projs[2];
	mat4 views[2];
	vec4 viewEyes[2];
} vrcamera;

//Uniform Geometry Buffer.
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

void main()
{
	vec4 vertex = vec4(vertices, 1.0);
	texCoord = texCoords;
    gl_Position = vrcamera.projs[gl_ViewIndex] * vrcamera.views[gl_ViewIndex] * geometry.world * vertex;
}