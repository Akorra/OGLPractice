#include <iostream>

#include "shader.hpp"
#include "mesh.hpp"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void getVertexAttribCount(int& count);

// settings
constexpr unsigned int SCR_WIDTH  = 800;
constexpr unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGL(glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // build and compile our shader program
    Shader ourShader("./shaders/simple.vs.glsl", "./shaders/simple.fs.glsl"); // you can name your shader files however you like

    // adding color attribute the rasterizer performs fragment interpolation in fragment shader
    Mesh mesh = generateTriangle(true, false, false);

    unsigned int vbo, vao, ebo;
    // vao stores:
    //  - calls to glEnableVertexAttribArray or glDisableVertexAttribArray
    //  - Vertex attribute configurations via glVertexAttribPointer
    //  - Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.
    glGenVertexArrays(1, &vao); //< generate vao (stores vertex attribute calls)
    glGenBuffers(1, &vbo); //< generate object buffer
    glGenBuffers(1, &ebo); //< element bufferobject, stores indices that OpenGL uses to decide what vertices to draw -indexed drawing
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //< bind it to GL_ARRAY_BUFFER target (vertex buffer object)

    // From here any calls we make (on GL_ARRAY_BUFFER target) will be used to configure current bound buffer (vbo)

    // glBufferData:
    //  args: type of buffer, size of data (bytes), data, how to manage data
    //  GL_STREAM_DRAW  -> data set once, used a few times.
    //  GL_STATIC_DRAW  -> data set once, used many times.
    //  GL_DYNAMIC_DRAW -> data changes and is used many times. 
    glBufferData(GL_ARRAY_BUFFER, mesh.cache.size() * sizeof(float), mesh.cache.data(), GL_STATIC_DRAW); //< copy vertex data to bound buffer memory 

    // now for ebo
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
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

    size_t stride = mesh.getStride();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(0); // enable vertex attribute at layout 0

    // color attribute - same as before but at layout 1 with offset acounting for (x,y,z)
    size_t clrOffset = mesh.getColorOffset();
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(clrOffset * sizeof(float)));
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // wireframe (incomment):
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop ------------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.use();

        // set uniforms ------------------------------------------------------------------------------------------------------------------
        // update the uniform color - uniforms allow us to pass data from our application on the CPU to the shaders on the GPU
        float timeValue = glfwGetTime();
        float variance  = sin(timeValue) / 2.0f + 0.5f;
        ourShader.setFloat("variance", variance);

        // draw -------------------------------------------------------------------------------------------------------------------------
        glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6 /* indices */, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void getVertexAttribCount(int& count) 
{
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &count);
    std::cout << "Maximum nr of vertex attributes supported: " << count << std::endl; // minimum 16 4 component attributes
}