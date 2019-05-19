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

void main()
{
     color = fragColor;
}