
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "camera.hpp"
#include "geometryhelper.hpp"
#include "materialhelper.hpp"
#include "lighthelper.hpp"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;
const float        NEAR_PLANE = 0.1f;
const float        FAR_PLANE  = 100.0f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float  lastX = SCR_WIDTH/2.0f;  // center x
float  lastY = SCR_HEIGHT/2.0f; // center y

// window
glm::uvec2 windowSize(SCR_WIDTH, SCR_HEIGHT); 

//time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// mouse io
bool mouseDown  = false;
bool firstMouse = true;

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

    Shader lightingShader("./shaders/2_maps.vs.glsl", "./shaders/2_multiple_lights.fs.glsl");
    Shader lightCubeShader("./shaders/2_light.vs.glsl", "./shaders/2_light.fs.glsl");

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // tex coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glm::mat3 normalMatrix;
    glm::mat4 projection, view, model;
    glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f), lightPosition(1.2f, 1.0f, 2.0f), lightAmbient(0.2f), lightDiffuse(0.5f), lightSpecular(1.0f);

    // load textures
    Material  diffuseMaterial;
    diffuseMaterial.loadTexture(diffuseMaterial.diffuseMapId, "./textures/container2.png");
    diffuseMaterial.loadTexture(diffuseMaterial.specularMapId, "./textures/container2_specular.png");
    diffuseMaterial.loadTexture(diffuseMaterial.emissionMapId, "./textures/matrix.jpg");

    DirectionalLight dirlight; // use defaults
    PointLight       pointlights[4]; // also build defaults

    // change spotlight positions
    for(int i=0; i<4; ++i)
        pointlights[i].position = pointLightPositions[i];

    // shader config
    lightingShader.use();
    lightingShader.setInt("material.diffuse",  0);
    lightingShader.setInt("material.specular", 1);
    lightingShader.setInt("material.emission", 2);

    // render loop
    unsigned int i=0;
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //lightPosition.x = 2.0f*glm::cos(glm::radians(20.0f*glfwGetTime()));
        //lightPosition.y = 2.0f*glm::sin(glm::radians(20.0f*glfwGetTime()));
        //lightPosition.z = 0.0f;

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();

        // light properties
        lightingShader.setFloat3("viewPosition",      camera.position);
        lightingShader.setFloat3("light.position",    camera.position);
        lightingShader.setFloat3("light.direction",   camera.front);
        lightingShader.setFloat("light.cutoff",       glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("light.outerCutoff",  glm::cos(glm::radians(17.5f)));
        lightingShader.setFloat3("light.ambient",     lightAmbient);
        lightingShader.setFloat3("light.diffuse",     lightDiffuse);
        lightingShader.setFloat3("light.specular",    lightSpecular);
        lightingShader.setFloat("light.constant",     1.0f);
        lightingShader.setFloat("light.linear",       0.09f);
        lightingShader.setFloat("light.quadratic",    0.032f);
        lightingShader.setFloat("material.shininess", diffuseMaterial.shininess);
        

        lightingShader.setFloat("mixer", 0.0f);//0.7f + sin(75.f*glm::radians(glfwGetTime()))*0.3f);

        // view/projection/world transformations
        projection   = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, NEAR_PLANE, FAR_PLANE);
        view         = camera.GetViewMatrix();
        model        = glm::mat4(1.0f);
        normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
        lightingShader.setMat4("projection",   projection);
        lightingShader.setMat4("view",         view);
        lightingShader.setMat3("normalMatrix", normalMatrix);

        diffuseMaterial.use();

        // render the cubes
        glBindVertexArray(cubeVAO);
        for(unsigned int i=0; i<10; ++i)
        {
            lightingShader.setMat4("model", 
                glm::rotate(glm::translate(model, cubePositions[i]), glm::radians(20.0f*i), glm::vec3(1.0f, 0.3f, 0.5f)));

            glDrawArrays(GL_TRIANGLES, 0, cubeVertCount);
        }

        /*
        // also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view); 
        lightCubeShader.setFloat3("light.ambient",   lightAmbient);
        lightCubeShader.setFloat3("light.diffuse",   lightDiffuse);
        lightCubeShader.setFloat3("light.specular",  lightSpecular);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, cubeVertCount);
        /**/

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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
    windowSize = {width, height};
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    mouseDown = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
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