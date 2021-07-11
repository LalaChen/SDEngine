#version 450
#extension GL_KHR_vulkan_glsl : enable

//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec2 texCoord;
layout(location = 5) in vec4 fColor;

//Output varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) out vec4 fragColor;

//Uniform
//layout(set = n, binding = m) for Opengl, we don't assign set. (default set is 0)
//Uniform Camera Buffer.
layout(set = 0, binding = 0) uniform OffsetUniforms {
	vec4 offsets;
} offset;

layout(set = 0, binding = 1) uniform sampler2D font; 

void main()
{
	fragColor = fColor * texture(font, texCoord).aaaa;
	//fragColor = texture(font, texCoord).aaaa;
	//fragColor = vec4(texCoord, 0, 1) * texture(font, texCoord).aaaa;
	//fragColor.a = 1.0f;
	//fragColor = vec4(1.0, 1.0, 1.0, 1.0);
}