#pragma once

#include <vector>
#include <glad/gl.h> // include glad to get all the required OpenGL headers

struct Mesh
{
    unsigned int vbo, vao, ebo;

    std::vector<float>      vertices;
    std::vector<float>      colors;
    std::vector<float>      normals;
    std::vector<float>      st;
    std::vector<float>      cache;
    std::vector<uint32_t>   indices;

    size_t getVertexCont()    const { return vertices.size() / 3; }
    size_t getStride()        const { return 3 + (colors.empty() ? 0 : 3) + (normals.empty() ? 0 : 3) + (st.empty() ? 0 : 2); }
    size_t getColorOffset()   const { return 3; }
    size_t getNormalsOffset() const { return 3 + (colors.empty() ? 0 : 3); }
    size_t getStOffset()      const { return 3 + (colors.empty() ? 0 : 3) + (normals.empty() ? 0 : 3);}
    
    Mesh() {}
    ~Mesh() 
    {
        // optional: de-allocate all resources once they've outlived their purpose: -> when
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
    }

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

    void generateBufers() 
    {        
        // vao stores:
        //  - calls to glEnableVertexAttribArray or glDisableVertexAttribArray
        //  - Vertex attribute configurations via glVertexAttribPointer
        //  - Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.
        glGenVertexArrays(1, &vao); //< generate vao (stores vertex attribute calls)
        glGenBuffers(1, &vbo); //< generate object buffer
        glGenBuffers(1, &ebo); //< element bufferobject, stores indices that OpenGL uses to decide what vertices to draw -indexed drawing
    }

    void updateBufferData() 
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo); //< bind it to GL_ARRAY_BUFFER target (vertex buffer object)

        // From here any calls we make (on GL_ARRAY_BUFFER target) will be used to configure current bound buffer (vbo)

        // glBufferData:
        //  args: type of buffer, size of data (bytes), data, how to manage data
        //  GL_STREAM_DRAW  -> data set once, used a few times.
        //  GL_STATIC_DRAW  -> data set once, used many times.
        //  GL_DYNAMIC_DRAW -> data changes and is used many times. 
        glBufferData(GL_ARRAY_BUFFER, cache.size() * sizeof(float), cache.data(), GL_STATIC_DRAW); //< copy vertex data to bound buffer memory 

        // now for ebo
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(size_t), indices.data(), GL_STATIC_DRAW);
        
        // our data is tightly packed, first value at 0, only position data:
        // x1,y1,z1,r1,g1,b1,x2,y2,z2,r2,g2,b2,...,xN,yN,zN,rN,gN,bN 
        //  - each position data is a 32-bit (4 byte) float
        //  - each position is composed of 6 values (x, y, z) + (r, b, b)

        // Args:
        //  - layout position (vertex attribute we want to configure)
        //  - size of vertex attribute (3 values)
        //  - type of data (GL_FLOAT)
        //  - normalize data ???
        //  - stride - space between consecutive vertex attributes -> x,y,z tightly packed -> 3*sizeof(float) or 0 to let opengl determine it (possible for tightly packed data)
        //  - offset - where position data begins in the buffer

        size_t stride = getStride();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0); 
        glEnableVertexAttribArray(0); // enable vertex attribute at layout 0

        // color attribute - same as before but at layout 1 with offset acounting for (x,y,z)
        if(!colors.empty())
        {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(getColorOffset() * sizeof(float)));
            glEnableVertexAttribArray(1);
        }

        //if(!normals.empty())
        //{
        //    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(getNormalsOffset() * sizeof(float)));
        //    glEnableVertexAttribArray(2);
        //}

        if(!st.empty())
        {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(getStOffset() * sizeof(float)));
            glEnableVertexAttribArray(2);
        }

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }

    void draw() {
        // draw -------------------------------------------------------------------------------------------------------------------------
        glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, indices.size() /* indices */, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time
    }
};

Mesh generateTriangle(float stMin=0.0f, float stMax=1.0f, bool colors=true, bool normals=true, bool st=true)
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
            stMin,   stMin,  // lower-left corner  
            stMax,   stMax,  // lower-right corner
            stMax/2, stMax   // top-center corner
        };

    triangle.update();

    triangle.indices = { 0, 1, 2 };
    
    return triangle;
}

Mesh generateRectangle(float stMin=0.0f, float stMax=1.0f, bool colors=true, bool normals=true, bool st=true)
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
            stMax, stMax,
            stMax, stMin,
            stMin, stMin,
            stMin, stMax
        };

    rectangle.update();

    rectangle.indices = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    return rectangle;
}

