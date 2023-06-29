#version 330 core

in vec2 fragTexCoord;
uniform sampler2D texture_sampler;
uniform vec4 custom_color;

in vec3 FragPos;
in vec3 Normal;
in mat4 viewPos;

uniform bool use_phong_shading;
uniform vec3 lightDirection;
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float shininess;  
out vec4 FragColor;
uniform bool isShading;
in vec3 LightingColor; 

void main(){

    if(use_phong_shading){
          // Phong shading.
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

          // Apply texture
          if(!isShading){
            vec4 texColor = texture(texture_sampler, fragTexCoord);
            FragColor = vec4((ambient + diffuse + specular) * texColor.xyz, texColor.a);
          }
          else
            FragColor = vec4((ambient + diffuse + specular) * custom_color.xyz, custom_color.a);
    }else{
            if(!isShading){
                  vec4 texColor = texture(texture_sampler, fragTexCoord);
                  FragColor = vec4(LightingColor * texColor.xyz, texColor.a);
            }
            else
                  FragColor = vec4(LightingColor * custom_color.xyz, custom_color.a); 
    }

}