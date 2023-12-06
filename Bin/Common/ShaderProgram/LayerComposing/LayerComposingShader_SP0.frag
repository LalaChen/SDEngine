#version 450
#extension GL_KHR_vulkan_glsl : enable

//input varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) in vec2 texCoord;

//output
layout(location = 0) out vec4 fragColor;


layout(set = 0, binding = 0) uniform UVScaleUniforms {
    vec4 uv;
} uvData;
layout(set = 0, binding = 1) uniform sampler2D mainTexture;

void main()
{
    fragColor = texture(mainTexture, texCoord);
}