#version 450

#extension GL_EXT_multiview : enable
#extension GL_KHR_vulkan_glsl : enable

//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec2 vertices;
layout(location = 2) in vec2 texCoords;
layout(location = 5) in vec4 colors;

//Output varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) out vec2 texCoord;
layout(location = 5) out vec4 fColor;

//Uniform
//layout(set = n, binding = m) for Opengl, we don't assign set. (default set is 0)
//Uniform Camera Buffer.
layout(set = 0, binding = 0) uniform OffsetUniforms {
	vec4 offsets;
} offset;

layout(set = 0, binding = 1) uniform sampler2D font; 

void main()
{
	texCoord = texCoords;
	fColor = colors;
	gl_Position = vec4(vertices * vec2(offset.offsets.xy) + vec2(offset.offsets.zw), 0.0, 1.0);
	//fColor = vec4(vertices * vec2(offset.offsets.xy) + vec2(offset.offsets.zw), 0.0, 1.0);
	//gl_Position = vec4(vertices, 0.0, 1.0);
}