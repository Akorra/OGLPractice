#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
// layout (location = 2) in vec2 aOffset; //< for instance vertex attrib

out vec3 fColor;

// learnopen gl creates a uniform array of offsets
vec2 getOffset(int index)
{
    int row = index/10;
    int col = index%10;

    int x = -10 + col*2;
    int y = -10 + row*2;

    return vec2(float(x) / 10.0 + 0.1, float(y) / 10.0 + 0.1);
}

void main()
{
    gl_Position = vec4((aPos*(gl_InstanceID / 100.0)) + getOffset(gl_InstanceID), 0.0, 1.0); 
    fColor = aColor;
}