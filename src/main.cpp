#include <iostream>

#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void getVertexAttribCount(int& count);

// settings
constexpr unsigned int SCR_WIDTH  = 800;
constexpr unsigned int SCR_HEIGHT = 600;

// stores how much we're seeing of either texture
float mixValue = 0.2f;

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
    Mesh mesh = generateRectangle(0.0f, 1.0f, true, false, true);
    mesh.generateBufers();
    mesh.updateBufferData();

    Texture texture1("./textures/container.jpg");
    Texture texture2("./textures/awesomeface.png", GL_RGBA);

    // wireframe (incomment):
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    ourShader.use();
    ourShader.setInt("texture2", 1); //< tell ogl to wich texture unit each shader sampler belongs to. texture one goes to 0 by default

    // render loop ------------------------------------------------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ogl provides atleast 16 texture units
        texture1.bind(GL_TEXTURE0);
        texture2.bind(GL_TEXTURE0 + 1); // or GL_TEXTURE1

        ourShader.use();
        // set uniforms ------------------------------------------------------------------------------------------------------------------
        // update the uniform color - uniforms allow us to pass data from our application on the CPU to the shaders on the GPU
        //float timeValue = glfwGetTime();
        float variance = mixValue; //sin(timeValue) / 2.0f + 0.5f;
        ourShader.setFloat("variance", variance);
        
        mesh.draw();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
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