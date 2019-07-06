#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform sampler2D texture_diffuse;
uniform vec3 lightPos;
uniform vec3 cameraPos;


void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 vertexColor = vec3(texture(texture_diffuse, aTexCoords));
    
    // normal
    vec3 normal = normalize(aNormal);
    vec3 lightDir = normalize(lightPos - aPos);
    vec3 viewDir = normalize(cameraPos - aPos);
    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    
    // calculat ambient
    vec3 ambientColor = ambient * lightColor;
    
    // calculate diffuse;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diff * diffuse * lightColor;
    
    // calculate specular
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
    vec3 specularColor = spec * specular * lightColor;

    
    color = (diffuseColor + ambientColor + specularColor) * vertexColor;
}