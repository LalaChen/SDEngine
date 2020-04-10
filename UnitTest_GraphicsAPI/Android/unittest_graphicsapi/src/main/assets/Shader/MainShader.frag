#version 450
//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec4 color;
layout(location = 1) in vec2 texCoord;
//Output varing.
//format : layout(location = X) out GenType VarName;
//X is which attachment you want to output.
layout(location = 0) out vec4 fragColor;

//Uniform
//layout(set = n, binding = m) for Opengl, we don't assign set. (default set is 0)
layout(binding = 1) uniform sampler2D mainTexture; 

void main()
{
     fragColor = color * texture(mainTexture, texCoord);
	 //fragColor = vec4(texCoord.x, texCoord.y, 0.0, 1.0);
	 //fragColor = fragColor;
}