#version 450
//#extension GL_ARB_separate_shader_objects : enable

//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec3 vertice;
layout(location = 2) in vec2 texCoords;

//Output varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) out vec2 texCoord;

void main()
{
	 texCoord = texCoords;
     //gl_Position = vertex;
	 gl_Position = vec4(vertice.x, vertice.y, vertice.z, 1.0);
}