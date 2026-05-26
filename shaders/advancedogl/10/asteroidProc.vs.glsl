#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform int  instanceCount;

const float radius = 150.0;
const float offset = 25.0f;

float random(float seed)
{
    return fract(sin(seed) * 43758.5453123);
}

mat4 translate(vec3 t)
{
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        t.x, t.y, t.z, 1.0
    );
}

mat4 scale(vec3 s)
{
    return mat4(
        s.x, 0.0, 0.0, 0.0,
        0.0, s.y, 0.0, 0.0,
        0.0, 0.0, s.z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );
}


mat4 rotationY(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return mat4(
           c,  0.0,  -s,  0.0,
         0.0,  1.0, 0.0,  0.0,
           s,  0.0,   c,  0.0,
         0.0,  0.0, 0.0,  1.0
    );
}

mat4 transformAsteroid()
{
    float id = float(gl_InstanceID);

    float angle = id / float(instanceCount) * 6.28318530718;

    float dx = random(id * 12.9898) * 2.0 * offset - offset;
    float dy = random(id * 78.233) * 2.0 * offset - offset;
    float dz = random(id * 39.425) * 2.0 * offset - offset;

    float x = sin(angle) * radius + dx;
    float y = dy * 0.4;
    float z = cos(angle) * radius + dz;

    float s = random(id * 15.137) * 0.2 + 0.05;
    float rot = random(id * 91.731) * 6.28318530718;

    mat4 model = translate(vec3(x, y, z));
    model *= rotationY(rot);
    model *= scale(vec3(s));

    return model;
}

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * transformAsteroid() * vec4(aPos, 1.0f); 
}