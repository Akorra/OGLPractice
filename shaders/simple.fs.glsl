#version 330 core
out vec4 FragColor;
  
in vec3 vertexColor; // the input variable from the vertex shader (same name and same type)  

uniform float variance; // we set this variable in the OpenGL code.

void main()
{

    FragColor = vec4(vertexColor.x, vertexColor.y, vertexColor.z, 1.0f)*variance;
}