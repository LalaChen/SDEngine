#version 450
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
//Uniform basic Buffer.
layout(binding = 0) uniform BasicUniforms {
    mat4 clip;
    mat4 proj;
	mat4 view;
	mat4 world;
	mat4 normal;
	vec4 viewEye;
} basic;

//Uniform Light Buffer.
layout(binding = 1) uniform LightUniforms {
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
layout(binding = 2) uniform MaterialUniforms {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;
} material;

layout(binding = 3) uniform sampler2D mainTexture; 

//------- light vertices basic function -------
vec3 CalculateLightDir(in vec4 iVertex)
{
	//Directional Light
    if (light.kind == 0) {
		return normalize(light.position.xyz);
	} 
	else if (light.kind == 1) {
		return normalize(light.position.xyz - iVertex.xyz);
	}
	else if (light.kind == 2) {
		return normalize(light.position.xyz - iVertex.xyz);
	}
	else {
		return normalize(light.direction.xyz);//error case
	}
}

vec3 CalculateViewDir(in vec4 iVertex)
{
	return normalize(basic.viewEye.xyz - iVertex.xyz);
}

void main()
{
	vec4 vertex = vec4(vertices, 1.0);
	wNormal = normalize((basic.normal * vec4(normals, 0.0)).xyz);
	texCoord = texCoords;
	wLightDir = CalculateLightDir(vertex).xyz;
	wViewDir = CalculateViewDir(vertex).xyz;
	wVertex = vertex;
    gl_Position = basic.clip * basic.proj * basic.view * basic.world * vertex;
}