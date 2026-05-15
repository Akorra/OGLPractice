#pragma once

#include <vector>

struct Mesh
{
    std::vector<float>      vertices;
    std::vector<float>      colors;
    std::vector<float>      normals;
    std::vector<float>      st;
    std::vector<uint32_t>   indices;
};

Mesh& generateTriangle() 
{
    Mesh triangle;
    triangle.vertices = {
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        0.0f,  0.5f, 0.0f,  // top 
    };

    triangle.colors = {
        1.0f, 0.0f, 0.0f,  // bottom right
        0.0f, 1.0f, 0.0f,  // bottom left
        0.0f, 0.0f, 1.0f   // top 
    };

    triangle.normals = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    triangle.st = {
        0.0f, 0.0f,  // lower-left corner  
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner
    };

    triangle.indices = { 0, 1, 2 };
    
    return triangle;
}

Mesh& generateRectangle()
{
    Mesh rectangle;
    rectangle.vertices = {
        0.5f,  0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };
    
    rectangle.colors = {
        1.0f, 0.0f, 0.0f, // top right    (red)
        0.0f, 1.0f, 0.0f, // bottom right (green)
        0.0f, 0.0f, 1.0f, // bottom left  (blue)
        1.0f, 1.0f, 0.0f  // top left     (yellow)
    };

    rectangle.normals = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    rectangle.st = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    rectangle.indices = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    return rectangle;
}