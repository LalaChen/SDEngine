#version 450
#extension GL_KHR_vulkan_glsl : enable

//input varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) in vec2 texCoord;

//output
layout(location = 0) out vec4 fragColor;

//Uniform
//layout(set = n, binding = m) for Opengl, we don't assign set. (default set is 0)
//Uniform basic Buffer.
layout(set = 0, binding = 0) uniform VRCameraUniforms {
    mat4 projs[2];
	mat4 views[2];
	vec4 viewEyes[2];
} vrcamera;

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
	fragColor = texture(textures[0], texCoord);
}