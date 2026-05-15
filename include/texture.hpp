#pragma once

#include <glad/gl.h> // include glad to get all the required OpenGL headers

// stb_image.h - single header image loading library by Sean Barrett
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include <iostream>

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

class Texture
{
public:
    Texture(const std::string& filePath)
    {
        data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
        if(data == nullptr)
        {
            std::cout << "Failed to load texture" << std::endl;
            return;
        }
        
        glGenTextures(1, &ID); // hoe many textures to generate and store in int array (2nd arg)
        glBindTexture(GL_TEXTURE_2D, ID); // bind texture to target

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // generate texture:
        // args:
        //  - texture target
        //  - mipmap level at wich to create the texture for
        //  - desired format
        //  - width & height of resulting texture
        //  - 0 (legacy stuff??)
        //  - format and datatype of source image
        //  - actual image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //< generate mipmaps
        
        // free imafe data:
        stbi_image_free(data);
    }

    bool bind() { glBindTexture(GL_TEXTURE_2D, ID); }

private:
    int width=0, height=0, channels=0;
    unsigned int  ID    = 0;
    unsigned char *data = nullptr;
};

 