#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPosition;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float     shininess;
};
uniform Material material;

struct Light {
    vec3 direction; //< position no longer necessary when using directional lights.
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
uniform float mixer = 1.0f;
uniform vec3  viewPosition;

void main()
{
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction); // invert since calculations so far expect direction to be from fragment to light
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseSample = texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse       = light.diffuse * diff * diffuseSample;

    // specular
    vec3 viewDir = normalize(viewPosition - FragPosition); 
    vec3 reflectionDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess); 
    vec3 specularSample =  texture(material.specular, TexCoords).rgb; 
    vec3 specular       =  light.specular * spec * specularSample; 

    // emmission
    vec3 emission = step(specularSample, vec3(0.0)) * mixer * texture(material.emission, TexCoords).rgb;

    // add light contributes 
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}