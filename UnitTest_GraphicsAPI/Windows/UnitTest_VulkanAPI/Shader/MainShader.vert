#version 450
#extension GL_ARB_separate_shader_objects : enable

//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec3 vertice;
layout(location = 1) in vec4 colors;

//Output varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) out vec4 fragColor;

//Uniform Buffer --- basic.
layout(set = 0, binding = 0) uniform BasicUnifroms {
    mat4 clip;
    mat4 proj;
	mat4 view;
	mat4 world;
} basic;

void main()
{
     vec4 vertex = vec4(vertice.x, vertice.y, vertice.z, 1.0);
     fragColor = colors;
     gl_Position = basic.clip * basic.proj * basic.view * basic.world * vertex;
	 //gl_Position = basic.proj * vertex;
	 //gl_Position = vec4(vertice.x, vertice.y, vertice.z, 1.0);
}