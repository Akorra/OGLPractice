#pragma once

#include <vector>

struct Mesh
{
    std::vector<float>      vertices;
    std::vector<float>      colors;
    std::vector<float>      normals;
    std::vector<float>      st;
    std::vector<float>      cache;
    std::vector<uint32_t>   indices;

    size_t getStride()        const { return 3 + (colors.empty() ? 0 : 3) + (normals.empty() ? 0 : 3) + (st.empty() ? 0 : 2); }
    size_t getColorOffset()   const { return 3; }
    size_t getNormalsOffset() const { return 3 + (colors.empty() ? 0 : 3); }
    size_t getStOffset()      const { return 3 + (colors.empty() ? 0 : 3) + (normals.empty() ? 0 : 3);}
    
    void update() 
    {
        cache.clear();
        
        if(vertices.empty()) return;

        size_t stride = getStride();
        size_t vertexCount = vertices.size() / 3;

        cache.resize(vertexCount * stride);
        for(size_t i=0; i<vertexCount; ++i)
        {
            size_t j=0, offset=i*stride;

            cache[offset + j++] = vertices[i*3];  
            cache[offset + j++] = vertices[i*3+1];
            cache[offset + j++] = vertices[i*3+2];

            if(!colors.empty())
            {
                cache[offset + j++] = colors[i*3];  
                cache[offset + j++] = colors[i*3+1];
                cache[offset + j++] = colors[i*3+2];
            }

            if(!normals.empty())
            {
                cache[offset + j++] = colors[i*3];  
                cache[offset + j++] = colors[i*3+1];
                cache[offset + j++] = colors[i*3+2];
            }

            if(!st.empty())
            {
                cache[offset + j++] = st[i*2]; 
                cache[offset + j]   = st[i*2+1]; 
            }
        }
    }
};

Mesh generateTriangle(bool colors=true, bool normals=true, bool st=true)
{
    Mesh triangle;
    triangle.vertices = {
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f,  // top 
    };

    if(colors)
        triangle.colors = {
            1.0f, 0.0f, 0.0f,  // bottom right
            0.0f, 1.0f, 0.0f,  // bottom left
            0.0f, 0.0f, 1.0f   // top 
        };

    if(normals)
        triangle.normals = {
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f
        };

    if(st)
        triangle.st = {
            0.0f, 0.0f,  // lower-left corner  
            1.0f, 0.0f,  // lower-right corner
            0.5f, 1.0f   // top-center corner
        };

    triangle.update();

    triangle.indices = { 0, 1, 2 };
    
    return triangle;
}

Mesh generateRectangle(bool colors=true, bool normals=true, bool st=true)
{
    Mesh rectangle;
    rectangle.vertices = {
        0.5f,  0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    
    if(colors)
        rectangle.colors = {
            1.0f, 0.0f, 0.0f, // top right    (red)
            0.0f, 1.0f, 0.0f, // bottom right (green)
            0.0f, 0.0f, 1.0f, // bottom left  (blue)
            1.0f, 1.0f, 0.0f  // top left     (yellow)
        };

    if(normals)
        rectangle.normals = {
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f
        };

    if(st)
        rectangle.st = {
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f
        };

    rectangle.update();

    rectangle.indices = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    return rectangle;
}