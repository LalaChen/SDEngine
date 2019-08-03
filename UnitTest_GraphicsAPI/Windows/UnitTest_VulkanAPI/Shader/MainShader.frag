#version 450
//Input attribute.
//format : layout(location = X) in GenType VarName;
//X is means we need to bind buffer to attribute location X.
//--- Attribute vertice.
layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
//Output varing.
//format : layout(location = X) out GenType VarName;
//X is which attachment you want to output.
layout(location = 0) out vec4 color;

layout(set = 0, binding = 1) uniform sampler2D mainTexture; 

void main()
{
     color = fragColor * texture(mainTexture, fragTexCoord);
	 //color = vec4(fragTexCoord.x, fragTexCoord.y, 0.0, 1.0);
	 //color = fragColor;
}