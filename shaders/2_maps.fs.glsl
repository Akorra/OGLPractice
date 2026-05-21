#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPosition;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform vec3 viewPosition;

void main()
{
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPosition - FragPosition); 
    vec3 reflectionDir = reflect(-lightDir, norm); //< reflection direction
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess); //< 32 is the shininess value of highlight (higher -> scatters less reflects more)
    vec3 specular =  light.specular * spec * texture(material.specular, TexCoords).rgb; //< specular impact (angle between viewer and reflection)

    // add light contributes 
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}