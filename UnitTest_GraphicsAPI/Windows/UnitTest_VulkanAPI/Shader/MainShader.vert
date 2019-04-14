#version 450

//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec4 vertice;
layout(location = 1) in vec4 colors;

//Output varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) out vec4 fragColor;

//Uniform Buffer --- basic.
layout(set = 0, binding = 0) uniform BasicUnifroms {
    mat4 transform;
    mat4 view;
    mat4 proj;
} basic;

void main()
{
     fragColor = colors;
     gl_Position = basic.proj * basic.view * basic.transform * vertice;
}