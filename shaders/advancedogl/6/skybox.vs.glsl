#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    
    // setting z=w on perspective division makes it 1.0, which is the far plane 
    // forcing skybox to allways render at maximum depth (1.0)
    gl_Position =  (projection * view * vec4(aPos, 1.0)).xyww; 
}