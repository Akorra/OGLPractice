#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{    
    // FragColor = vec4(vec3(texture(texture1, TexCoords)), 1.0); //< force ignore transparency
    vec4 texColor = texture(texture1, TexCoords);
    //if(texColor.a < 0.2)
    //    discard;
    FragColor = texColor;
}