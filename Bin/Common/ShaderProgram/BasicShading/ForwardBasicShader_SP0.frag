#version 450
#extension GL_KHR_vulkan_glsl : enable

//input varing.
//format : layout(location = X) out GenType VarName;
//X is corresponding with layout(location = X) in frag shader(or other stage).
layout(location = 0) in vec2 texCoord;
layout(location = 1) in vec4 wVertex;
layout(location = 2) in vec3 wNormal;
layout(location = 3) in vec3 wLightDir;
layout(location = 4) in vec3 wViewDir;

//output
layout(location = 0) out vec4 fragColor;

//Uniform
//layout(set = n, binding = m) for Opengl, we don't assign set. (default set is 0)
//Uniform Camera Buffer.
layout(set = 0, binding = 0) uniform CameraUniforms {
    mat4 proj;
    mat4 view;
    vec4 viewEye;
} camera;

//Uniform Geometry Buffer.
layout(set = 1, binding = 0) uniform GeometryUniforms {
    mat4 world;
    mat4 normal;
} geometry;

//Uniform Light Buffer.
layout(set = 2, binding = 0) uniform LightUniforms {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position; //point light.
    vec4 direction; //direction light.
    float spotExponent;
    float spotCosCutoff;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    uint kind; //0: directional, 1: spot, 2: point
} light;

layout(set = 2, binding = 1) uniform sampler2D shadowMaps[4]; 

//Uniform Material buffer
layout(set = 3, binding = 0) uniform MaterialUniforms {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    float shininess;
} material;

layout(set = 3, binding = 1) uniform sampler2D textures[11]; 

//------- light vertices basic function -------
float calculateAttenation(in float dis)
{
    return min( 1.0 / (light.constantAttenuation + light.linearAttenuation * dis + light.quadraticAttenuation * dis * dis), 1.0);
}

// directional light.
void CalculateDirLightingDAndSAndNDL(inout vec4 ioDiffuse, inout vec4 ioSpecular, inout float ioNDotL)
{
    ioNDotL = max(dot(wNormal, wLightDir), 0.0);
    vec3 h = normalize(wLightDir + wViewDir);
    float HDotN = max(dot(h, wNormal), 0.0);
    float powHDotN = pow(HDotN, material.shininess);
    ioDiffuse = light.diffuse * material.diffuse * ioNDotL;
    ioSpecular = light.specular * material.specular * powHDotN;
}

void CalculatePointLightDAndSAndNDL(inout vec4 ioDiffuse, inout vec4 ioSpecular, inout float ioNDotL)
{
    ioNDotL = max(dot(wNormal, wLightDir), 0.0);
    float dis = distance(wVertex, light.position);
    float attenuate = calculateAttenation(dis);
    vec3 h = normalize(wLightDir + wViewDir);
    float HDotN = max(dot(h, wNormal), 0.0);
    float powHDotN = pow(HDotN, material.shininess);
    ioDiffuse = attenuate * light.diffuse * material.diffuse * ioNDotL;
    ioSpecular = attenuate * light.specular * material.specular * powHDotN;
}

void CalculateSpotLightDAndSAndNDL(inout vec4 ioDiffuse, inout vec4 ioSpecular, inout float ioNDotL)
{
    ioNDotL = max(dot(wNormal, wLightDir), 0.0);
    float dis = distance(wVertex, light.position);
    float attenuate = calculateAttenation(dis);
    vec3 h = normalize(wLightDir + wViewDir);
    float HDotN = max(dot(h, wNormal), 0.0);
    float powHDotN = pow(HDotN, material.shininess);
    float refAngle = max(dot(wLightDir, light.direction.xyz), 0.0);
    if (refAngle > 0.0) {
        refAngle = pow(refAngle, light.spotExponent);
    }
    ioDiffuse = refAngle * attenuate * light.diffuse * material.diffuse * ioNDotL;
    ioSpecular = refAngle * attenuate * light.specular * material.specular * powHDotN;
}

vec4 CalculateLighting()
{
    vec4 ambient = vec4(0.0), diffuse = vec4(0.0), specular = vec4(0.0);
    float nDotL;
    ambient = material.ambient * light.ambient;
    if (light.kind == 0) {
        CalculateDirLightingDAndSAndNDL(diffuse, specular, nDotL);
    }
    else if (light.kind == 1) {
        CalculateSpotLightDAndSAndNDL(diffuse, specular, nDotL);
    }
    else if (light.kind == 2) {
        CalculatePointLightDAndSAndNDL(diffuse, specular, nDotL);
    }
    else {
        diffuse = vec4(1.0, 0.0, 0.0, 1.0);
    }
    return (ambient + diffuse + specular + material.emission) * texture(textures[0], texCoord);
    //return vec4(wViewDir, 1.0);
    //return vec4(abs(nDotL), abs(nDotL), abs(nDotL), 1.0);
    //return texture(mainTexture, texCoord);
}

void main()
{
    fragColor = CalculateLighting();
}