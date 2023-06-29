#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 texCoord;
uniform mat4 model_pos;
uniform mat4 projection;

uniform bool use_phong_shading;
out vec3 FragPos;
out vec3 Normal;
out vec2 fragTexCoord;
out mat4 viewPos;

uniform vec3 lightDirection;
uniform sampler2D texture_sampler;
uniform vec4 custom_color;
  

uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;  
out vec3 LightingColor;


void main()
{
    gl_Position = projection * model_pos * vec4(aPos.xyz,1);
    Normal =  mat3(transpose(inverse(model_pos))) * -aPos.xyz;
    FragPos =  vec3(model_pos * aPos);
    fragTexCoord = texCoord;
    viewPos = projection;
    if(!use_phong_shading){    
        // gouraud shading
        // ------------------------
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(-lightDirection);

        // Ambient lighting
        vec3 ambient = ambientColor; 

        // Diffuse lighting
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diffuseColor * diff;

        // Specular lighting
        vec3 viewDir = normalize(-FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularColor * spec;

        LightingColor = ambient + diffuse + specular;
          
    }
}