#version 450

//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec4 fragColor;

//Output varing.
//format : layout(location = X) out GenType VarName;
//X is which attachment you want to output.
layout(location = 0) out vec4 color;

//Uniform Buffer --- basic.
layout(std140, set = 0, binding = 0) uniform BasicUnifroms {
    mat4 proj;
	mat4 view;
	mat4 transform;
} basic;

void main()
{
     color = fragColor;
	 //color = vec4(basic.proj[0][0], basic.proj[1][1], basic.proj[2][2], 1.0);
	 //color = abs(vec4(basic.proj[3][0], basic.proj[3][1], basic.proj[3][2], 1.0));
}