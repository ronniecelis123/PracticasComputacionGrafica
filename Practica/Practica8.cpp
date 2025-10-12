// Practica 8
// Celis Hernández Ronie
// 12 de octubre del 2025
// 318143093

// Std. Includes
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath> // sin, cos

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


glm::vec3 centerWhite(0.5f, 0.0f, 0.0f);   
glm::vec3 centerBlue(0.0f, 0.0f, -0.5f);   

float ayWhite = 7.5f;   
float bzWhite = 10.5f;  
float ayBlue = 7.5f;   
float axBlue = 10.5f;  

float tWhite = 0.0f;   // O / L
float tBlue = 0.0f;   // I / K
float stepAngle = 0.02f;

const float T_MIN = 0.0f;
const float T_MAX = 3.1415926535f; 

// Otros estados
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
bool activanim = false;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8. Ronie Celis", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Modern function pointers
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // === INFO DEL CONTEXTO OPENGL ===
    {
        auto safe_cstr = [](const GLubyte* s) -> const char* {
            return s ? reinterpret_cast<const char*>(s) : "(null)";
            };
        auto print_gl_str = [&](GLenum pname, const char* label) {
            const GLubyte* s = glGetString(pname);
            std::cout << label << safe_cstr(s) << '\n';
            };
        print_gl_str(GL_VERSION, "> Version: ");
        print_gl_str(GL_VENDOR, "> Vendor: ");
        print_gl_str(GL_RENDERER, "> Renderer: ");
        print_gl_str(GL_SHADING_LANGUAGE_VERSION, "> SL Version: ");
    }

    // Viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");   // (opcional; no lo uso para dibujar)
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Models de escena
    Model dog((char*)"Models/RedDog.obj");
    Model avion((char*)"Models/granja.obj");
    Model grass((char*)"Models/grass.obj");
    Model arbol((char*)"Models/arbol.obj");
    Model molino((char*)"Models/molino.obj");
    Model valla((char*)"Models/valla.obj");

    // Modelo de Blender para lámparas (ESFERA)
    Model lampSphere((char*)"Models/esfera.obj");  

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Textura de ejemplo
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);

    // Loop
    while (!glfwWindowShouldClose(window))
    {
        // Tiempos
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // ? SIN WRAP a 2?: ahora CLAMP a [0, ?] (solo la mitad superior)
        auto clamp01pi = [](float& t) {
            if (t < T_MIN) t = T_MIN;
            if (t > T_MAX) t = T_MAX;
            };
        clamp01pi(tWhite);
        clamp01pi(tBlue);

        // === POSICIONES DE LAS ELIPSES EN PLANOS VERTICALES ===
        // Blanca (plano YZ): y = ay*sin(t), z = bz*cos(t), x fijo
        glm::vec3 whitePos;
        whitePos.x = centerWhite.x;                            
        whitePos.y = centerWhite.y + ayWhite * sin(tWhite);     
        whitePos.z = centerWhite.z + bzWhite * cos(tWhite);    

      
        glm::vec3 bluePos;
        bluePos.z = centerBlue.z;                               // fijo en Z
        bluePos.y = centerBlue.y + ayBlue * sin(tBlue);         // >= 0 por clamp
        bluePos.x = centerBlue.x + axBlue * cos(tBlue);         // este?oeste

        // Clear
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // === LUCES + MATRICES (lightingShader) ===
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, whitePos.x, whitePos.y, whitePos.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Luz blanca (sol)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.3f, 0.3f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);

        // Luz azul (secundaria)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), bluePos.x, bluePos.y, bluePos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.0f, 0.0f, 0.2f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 1.0f);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Material
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

   
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        dog.Draw(lightingShader);
        avion.Draw(lightingShader);
        grass.Draw(lightingShader);
        arbol.Draw(lightingShader);
        molino.Draw(lightingShader);
        valla.Draw(lightingShader);

        // === DIBUJAR LÁMPARAS COMO ESFERAS ===
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Lámpara blanca (esfera)
        glm::mat4 lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, whitePos);
        lampModel = glm::scale(lampModel, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lampModel));
        glUniform3f(glGetUniformLocation(lampshader.Program, "lampColor"), 1.0f, 1.0f, 1.0f);
        lampSphere.Draw(lampshader);

        // Lámpara azul (esfera)
        glm::mat4 lampModel2 = glm::mat4(1.0f);
        lampModel2 = glm::translate(lampModel2, bluePos);
        lampModel2 = glm::scale(lampModel2, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lampModel2));
        glUniform3f(glGetUniformLocation(lampshader.Program, "lampColor"), 0.0f, 0.0f, 1.0f);
        lampSphere.Draw(lampshader);

        // Swap
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);


    if (keys[GLFW_KEY_O]) { tWhite += stepAngle; if (tWhite > T_MAX) tWhite = T_MAX; }
    if (keys[GLFW_KEY_L]) { tWhite -= stepAngle; if (tWhite < T_MIN) tWhite = T_MIN; }

    if (keys[GLFW_KEY_I]) { tBlue += stepAngle; if (tBlue > T_MAX) tBlue = T_MAX; }
    if (keys[GLFW_KEY_K]) { tBlue -= stepAngle; if (tBlue < T_MIN) tBlue = T_MIN; }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = (GLfloat)xPos;
        lastY = (GLfloat)yPos;
        firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos; // Reversed since y-coordinates go from bottom to left

    lastX = (GLfloat)xPos;
    lastY = (GLfloat)yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
