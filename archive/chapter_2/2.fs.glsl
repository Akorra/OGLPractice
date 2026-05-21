#version 330 core

in vec3 Normal;
in vec3 FragPosition;

out vec4 FragColor;

struct Material {
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float shininess;
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
    float specularStrength = 0.5; //< medium bright color
    float ambientStrength  = 0.1;

    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPosition); //< direction between light source and fragment
    float diff = max(dot(norm, lightDir), 0.0); //< light does not subtract color (at 90º we get negative values)
    vec3 diffuse = light.diffuse * (diff * material.diffuse); //< diffuse impact of the light on fragment (angle)

    // specular
    vec3 viewDir = normalize(viewPosition - FragPosition); 
    vec3 reflectionDir = reflect(-lightDir, norm); //< reflection direction
    
    // negate lightdir because reflect() first vector to point from light source to fragment. 
    //but lightdir was claculated from fragment to light source

    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess); //< 32 is the shininess value of highlight (higher -> scatters less reflects more)
    vec3 specular =  light.specular * (spec * material.specular); //< specular impact (angle between viewer and reflection)

    // add light contributes 
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}