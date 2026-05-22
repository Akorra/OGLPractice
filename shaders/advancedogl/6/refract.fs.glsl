#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 TexCoords;
in vec3 Position;

uniform vec3 viewPosition;
uniform samplerCube skybox;

void main()
{    
    float ratio = 1.00/1.52; // air(1) to glass(1.52) [water(1.33), ice(1.309), diamond(2.42)]
    vec3 I = normalize(Position - viewPosition);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}