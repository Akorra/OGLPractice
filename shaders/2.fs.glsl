#version 330 core

in vec3 Normal;
in vec3 FragPosition;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightPosition;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // direction between light source and fragment
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPosition);
    
    // diffuse impact of the light on fragment (angle)
    float diff = max(dot(norm, lightDir), 0.0); //< light does not subtract color (at 90º we get negative values)
    vec3 diffuse = diff *lightColor;

    // add light contributes 
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}