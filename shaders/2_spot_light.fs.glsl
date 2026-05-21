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
    vec3  position;
    vec3  direction;
    float cutoff; //< spotlight radius angle

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
uniform Light light;

uniform float mixer = 1.0f;
uniform vec3  viewPosition;

void main()
{
    // atenuation
    float distance    = length(light.position - FragPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb * attenuation;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPosition); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseSample = texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse       = light.diffuse * diff * diffuseSample * attenuation;

    // specular
    vec3 viewDir = normalize(viewPosition - FragPosition); 
    vec3 reflectionDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess); 
    vec3 specularSample =  texture(material.specular, TexCoords).rgb; 
    vec3 specular       =  light.specular * spec * specularSample * attenuation; 

    // emmission
    vec3 emission = step(specularSample, vec3(0.0)) * mixer * texture(material.emission, TexCoords).rgb;

    // add light contributes 
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}