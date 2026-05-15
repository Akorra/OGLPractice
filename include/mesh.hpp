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

/**
 * Texture Wrapping
 * 
 * glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
 * glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
 * 
 * args: 
 *  - texture target (image is 2D)
 *  - target axis
 *  - texture wrapping mode
 * 
 * texture coordinates range (0,0) to (1,1), but may go outside range, so we introduce texture wrapping:
 *  - GL_REPEAT: The default behavior for textures. Repeats the texture image.
 *  - GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat.
 *  - GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
 *  - GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user-specified border color.
 *      - this option also requires color: glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, {r,g,b,a})
 */

 /**
  * Texture Filtering
  * 
  * determine wich texture pixel (texel) a float texture coordinate belongs to.
  * 
  * GL_NEAREST (nearest neightbour or point filtering) select texel with center closest to coordinate (default) - blocky
  * GL_LINEAR  ((bi)linear filtering) interpolated value from coordinates neightboring texels. - smoother
  * 
  * They can be set for scaling up or down independently:
  * glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  * glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  */

  /**
   * Mipmaps
   * 
   * textured objects small/far-away, with few fragments, produce artifacts due to ogl difficulty retrieving color.
   * waste of memory bandwidth
   * 
   * Mipmaps introduce continuously size-halved versions of the image, allowing to pick the best suited one for an object 
   * depending ond size/amount of fragments produced.
   * 
   * glGenerateMipmap - create mipmaps
   * options:
   *  - GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
   *  - GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap level and samples that level using linear interpolation.
   *  - GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples the interpolated level via nearest neighbor interpolation.
   *  - GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps and samples the interpolated level via linear interpolation.
   * 
   * glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
   * glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // mipmap has no effect on magnification
   */

   /**
    * loading and creating textures:
    * 
    * stb_image.h - single header image loading library by Sean Barrett
    *   #define STB_IMAGE_IMPLEMENTATION
    *   #include "stb_image.h"
    */