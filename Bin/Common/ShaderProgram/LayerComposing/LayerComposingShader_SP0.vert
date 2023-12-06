#version 450

#extension GL_KHR_vulkan_glsl : enable

//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec3 vertices;
layout(location = 2) in vec2 texCoords;
//Output varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) out vec2 texCoord;

layout(set = 0, binding = 0) uniform UVScaleUniforms {
    vec4 uv; //(x,y) offset, (z,w) size.
} uvData;

layout(set = 0, binding = 1) uniform sampler2D mainTexture;

void main()
{
    vec4 vertex;
    vertex.x = uvData.uv.z * vertices.x + uvData.uv.x;
    vertex.y = uvData.uv.w * vertices.y + uvData.uv.y;
    vertex.z = 0.0f;
    vertex.w = 1.0f;
    texCoord = texCoords;
    gl_Position = vertex;
}