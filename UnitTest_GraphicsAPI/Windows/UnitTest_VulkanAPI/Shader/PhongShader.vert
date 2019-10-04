#version 450
#extension GL_ARB_separate_shader_objects : enable

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
layout(location = 0) out vec2 fragTexCoord;

//Uniform Buffer --- basic.
layout(set = 0, binding = 0) uniform BasicUniforms {
    mat4 clip;
    mat4 proj;
	mat4 view;
	mat4 world;
} basic;

//Uniform Light Buffer.
layout(set = 0, binding = 1) uniform LightUniforms {
	vec4  ambient;
	vec4  diffuse;
	vec4  specular;
	vec4  emission;
	vec4  position; //pointer light.
	vec4  direction; //spot and direction light.
	float spotExponent;
	float spotCutoff;
	float spotCosCutoff;
	float shineness;
	int   lightKind;
	int   IsAffectByLight;
} light;