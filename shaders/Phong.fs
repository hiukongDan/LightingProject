#version 330 core
in vec2 TexCoords;
in vec3 FragPos;
in vec3 CameraPos;
in vec3 LightPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform sampler2D texture_diffuse;

void main(){
    vec3 vertexColor = vec3(texture(texture_diffuse, TexCoords));
    
    // normal
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    vec3 viewDir = normalize(CameraPos - FragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    
    // calculat ambient
    vec3 ambientColor = ambient * lightColor;
    
    // calculate diffuse;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuseColor = diff * diffuse * lightColor;
    
    // calculate specular
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
    vec3 specularColor = spec * specular * lightColor;
    
    vec3 color = (diffuseColor + ambientColor + specularColor) * vertexColor;
    
    FragColor = vec4(color, 1.0);
}