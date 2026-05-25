#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{
    if(gl_FrontFacing)
        FragColor = texture(frontTexture, TexCoords);
    else
        FragColor = texture(backTexture, TexCoords);

    if(gl_FragCoord.x < 400)
        FragColor = mix(FragColor, vec4(1.0, 0.0, 0.0, 1.0), 0.5);
    else
        FragColor = mix(FragColor, vec4(0.0, 1.0, 0.0, 1.0), 0.5);
}