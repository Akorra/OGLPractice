
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//#include "model.hpp"
#include "camera.hpp"
#include "utils.h"

#include <GLFW/glfw3.h>

#include <vector>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
uint32_t loadTexture(char const *path, bool flip);

// settings
const uint32_t SCR_WIDTH  = 800;
const uint32_t SCR_HEIGHT = 600;
const float        NEAR_PLANE = 0.1f;
const float        FAR_PLANE  = 100.0f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float  lastX = SCR_WIDTH/2.0f;  // center x
float  lastY = SCR_HEIGHT/2.0f; // center y

// mouse io
bool mouseDown  = false;
bool firstMouse = true;

//time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/**
 * Example uniform block alignment and offset
 * 
 * layout (std140) uniform ExampleBlock
 * {                       // base alingnment  // aligned offset
 *      float value;        // 4                // 0
 *      vec3  vector;       // 16               // 16   (offset must be multiple of 16) - offset is allways a multiple of base alignement
 *      mat4  matrix;       // 16               // 32   - col 0 
 *                          // 16               // 48   - col 1 
 *                          // 16               // 64   - col 2
 *                          // 16               // 80   - col 3
 *      float values[3];    // 16               // 96   - val 0
 *                          // 16               // 112  - val 1
 *                          // 16               // 128  - val 2
 *      bool  boolean;      // 4                // 144
 *      int   integer;      // 4                // 148
 * };
 */

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
    glfwSetMouseButtonCallback(window,     mouse_button_callback);
    glfwSetCursorPosCallback(window,       mouse_callback);
    glfwSetScrollCallback(window,          scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGL(glfwGetProcAddress)) 
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_PROGRAM_POINT_SIZE);

    Shader redShader("./shaders/advancedogl/8/point.vs.glsl", "./shaders/advancedogl/8/red.fs.glsl");
    Shader greenShader("./shaders/advancedogl/8/point.vs.glsl", "./shaders/advancedogl/8/green.fs.glsl");
    Shader blueShader("./shaders/advancedogl/8/point.vs.glsl", "./shaders/advancedogl/8/blue.fs.glsl");
    Shader yellowShader("./shaders/advancedogl/8/point.vs.glsl", "./shaders/advancedogl/8/yellow.fs.glsl");
    //float points[] = { -0.5f, -0.5f, 0.0f, /**/ -0.5f,  0.5f, 0.0f, /**/ 0.0f,  0.0f, 0.0f, /**/ 0.5f, -0.5f, 0.0f, /**/ 0.5f,  0.5f, 0.0f, };

    // box:
    uint32_t VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // gl_FrontFacing test:
    //uint32_t front = loadTexture("./resources/textures/container2.png", true);
    //uint32_t back  = loadTexture("./resources/textures/container2_specular.png", true);
    //pointsShader.use();
    //pointsShader.setInt("frontTexture", 0);
    //pointsShader.setInt("backTexture", 1);

    // set the uniform block of the vertex shaders equal to binding point 0
    // get the relevant block indices
    uint32_t uniformBlockIndexRed    = glGetUniformBlockIndex(redShader.ID, "Matrices");
    uint32_t uniformBlockIndexGreen  = glGetUniformBlockIndex(greenShader.ID, "Matrices");
    uint32_t uniformBlockIndexBlue   = glGetUniformBlockIndex(blueShader.ID, "Matrices");
    uint32_t uniformBlockIndexYellow = glGetUniformBlockIndex(yellowShader.ID, "Matrices");
    // link each shader's uniform block to this uniform binding point
    glUniformBlockBinding(redShader.ID, uniformBlockIndexRed, 0);
    glUniformBlockBinding(greenShader.ID, uniformBlockIndexGreen, 0);
    glUniformBlockBinding(blueShader.ID, uniformBlockIndexBlue, 0);
    glUniformBlockBinding(yellowShader.ID, uniformBlockIndexYellow, 0);

    // create the actual uniform buffer object and bind that buffer to binding point 0
    uint32_t uboMatrices;
    glGenBuffers(1, &uboMatrices);
    // allocate data, leave it empty:
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); 
    // define the range of the buffer that links to a uniform binding point
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);
    glm::mat4 view(1.0f), model(1.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection)); //< fill projection matrix
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // draw in wireframe, will show only 2 tryangles since we're sampling the texture attachement in a quad
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //update view matreix in uniform buffer
        view = camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindVertexArray(VAO);

        redShader.use();
        redShader.setMat4("model", glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, cubeVertCount);
        greenShader.use();
        greenShader.setMat4("model", glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, cubeVertCount);
        blueShader.use();
        blueShader.setMat4("model", glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, cubeVertCount);
        yellowShader.use();
        yellowShader.setMat4("model", glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)));
        glDrawArrays(GL_TRIANGLES, 0, cubeVertCount);
        
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, front);
        //glActiveTexture(GL_TEXTURE0+1);
        //glBindTexture(GL_TEXTURE_2D, back);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    camera.movementSpeed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) ? SPEED*2 : SPEED;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mouseDown = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
    if(mouseDown) firstMouse = true;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if(!mouseDown) return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

uint32_t loadTexture(char const *path, bool flip)
{
    uint32_t textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(flip);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        std::cout << "Texture did not fail to load at path: " << path << std::endl;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        auto texParam = (format == GL_RGBA) ? GL_CLAMP_TO_EDGE : GL_REPEAT;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texParam);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texParam);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}