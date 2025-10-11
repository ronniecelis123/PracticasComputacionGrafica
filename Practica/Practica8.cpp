// Previo 8
// Celis Hernández Ronie
// 30 de septiembre del 2025
// 318143093

// Std. Includes
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath> // sin, cos, fmod

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

// === LUCES EN PLANOS VERTICALES (ELIPSES COMPLETAS, CONTROL MANUAL) ===
// Centros base (ajústalos a tu escena). y ? 0 es “horizonte”.
glm::vec3 centerWhite(0.5f, 0.0f, 0.0f);   // Blanca: plano YZ (sur?norte en Z)
glm::vec3 centerBlue(0.0f, 0.0f, -0.5f);   // Azul:   plano YX (este?oeste en X)

// Semiejes de cada elipse (verticales)
// BLANCA (plano YZ): domina Y (altura) y recorre Z (S?N)
float ayWhite = 7.5f;  // radio en Y (qué tan alto/bajo “sol”)
float bzWhite = 10.5f;  // radio en Z (sur-norte)

// AZUL (plano YX): domina Y (altura) y recorre X (E?O)
float ayBlue = 7.5f;  // radio en Y
float axBlue = 10.5f;  // radio en X (este-oeste)

// Ángulos (avanzan/retroceden con teclas)
float tWhite = 0.0f;   // O / L
float tBlue = 0.0f;   // I / K
float stepAngle = 0.02f;

// Otros estados
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
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

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

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
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Models
  // Load models
    Model dog((char*)"Models/RedDog.obj");
    Model avion((char*)"Models/granja.obj");
    Model grass((char*)"Models/grass.obj");
    Model arbol((char*)"Models/arbol.obj");
    Model molino((char*)"Models/molino.obj");
    Model valla((char*)"Models/valla.obj");


    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    float vertices[] = {
        // positions          // normals
        -0.5f,-0.5f,-0.5f,    0.0f,  0.0f,-1.0f,
         0.5f,-0.5f,-0.5f,    0.0f,  0.0f,-1.0f,
         0.5f, 0.5f,-0.5f,    0.0f,  0.0f,-1.0f,
         0.5f, 0.5f,-0.5f,    0.0f,  0.0f,-1.0f,
        -0.5f, 0.5f,-0.5f,    0.0f,  0.0f,-1.0f,
        -0.5f,-0.5f,-0.5f,    0.0f,  0.0f,-1.0f,

        -0.5f,-0.5f, 0.5f,    0.0f,  0.0f, 1.0f,
         0.5f,-0.5f, 0.5f,    0.0f,  0.0f, 1.0f,
         0.5f, 0.5f, 0.5f,    0.0f,  0.0f, 1.0f,
         0.5f, 0.5f, 0.5f,    0.0f,  0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,    0.0f,  0.0f, 1.0f,
        -0.5f,-0.5f, 0.5f,    0.0f,  0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f,   -1.0f,  0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,   -1.0f,  0.0f, 0.0f,
        -0.5f,-0.5f,-0.5f,   -1.0f,  0.0f, 0.0f,
        -0.5f,-0.5f,-0.5f,   -1.0f,  0.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,   -1.0f,  0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,   -1.0f,  0.0f, 0.0f,

         0.5f, 0.5f, 0.5f,    1.0f,  0.0f, 0.0f,
         0.5f, 0.5f,-0.5f,    1.0f,  0.0f, 0.0f,
         0.5f,-0.5f,-0.5f,    1.0f,  0.0f, 0.0f,
         0.5f,-0.5f,-0.5f,    1.0f,  0.0f, 0.0f,
         0.5f,-0.5f, 0.5f,    1.0f,  0.0f, 0.0f,
         0.5f, 0.5f, 0.5f,    1.0f,  0.0f, 0.0f,

        -0.5f,-0.5f,-0.5f,    0.0f, -1.0f, 0.0f,
         0.5f,-0.5f,-0.5f,    0.0f, -1.0f, 0.0f,
         0.5f,-0.5f, 0.5f,    0.0f, -1.0f, 0.0f,
         0.5f,-0.5f, 0.5f,    0.0f, -1.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,    0.0f, -1.0f, 0.0f,
        -0.5f,-0.5f,-0.5f,    0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f,-0.5f,    0.0f,  1.0f, 0.0f,
         0.5f, 0.5f,-0.5f,    0.0f,  1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,    0.0f,  1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,    0.0f,  1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,    0.0f,  1.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,    0.0f,  1.0f, 0.0f
    };

    // VAO/VBO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Textura
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // MAG no admite mipmaps
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

        // Normaliza ángulos a [0, 2?)
        const float TWO_PI = 6.28318530718f;
        if (tWhite >= TWO_PI) tWhite = fmod(tWhite, TWO_PI);
        if (tWhite < 0.0f)     tWhite += TWO_PI;
        if (tBlue >= TWO_PI) tBlue = fmod(tBlue, TWO_PI);
        if (tBlue < 0.0f)     tBlue += TWO_PI;

        // === POSICIONES DE LAS ELIPSES EN PLANOS VERTICALES ===
        // BLANCA (plano YZ): y = ay*sin(t), z = bz*cos(t), x fijo
        glm::vec3 whitePos;
        whitePos.x = centerWhite.x;                             // fijo en X
        whitePos.y = centerWhite.y + ayWhite * sin(tWhite);     // arriba/abajo (día/noche)
        whitePos.z = centerWhite.z + bzWhite * cos(tWhite);     // sur?norte

        // AZUL (plano YX): y = ay*sin(t), x = ax*cos(t), z fijo
        glm::vec3 bluePos;
        bluePos.z = centerBlue.z;                               // fijo en Z
        bluePos.y = centerBlue.y + ayBlue * sin(tBlue);        // arriba/abajo
        bluePos.x = centerBlue.x + axBlue * cos(tBlue);        // este?oeste

        // Clear
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Luz y materiales
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

        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader);
        avion.Draw(shader);
        grass.Draw(shader);
        arbol.Draw(shader);
        molino.Draw(shader);
        valla.Draw(shader);
        glm::mat4 model3(1.0f);
        glBindVertexArray(0);

        // Lámparas (cubos que muestran dónde están las luces)
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glBindVertexArray(VAO);

        // Lámpara blanca
        glm::mat4 lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, whitePos);
        lampModel = glm::scale(lampModel, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lampModel));
        glUniform3f(glGetUniformLocation(lampshader.Program, "lampColor"), 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Lámpara azul
        glm::mat4 lampModel2 = glm::mat4(1.0f);
        lampModel2 = glm::translate(lampModel2, bluePos);
        lampModel2 = glm::scale(lampModel2, glm::vec3(0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(lampModel2));
        glUniform3f(glGetUniformLocation(lampshader.Program, "lampColor"), 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        // Swap
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
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

    // Control manual de las elipses (MISMAS TECLAS):
    // Luz blanca (plano YZ): O avanza, L retrocede
    if (keys[GLFW_KEY_O]) tWhite += stepAngle;
    if (keys[GLFW_KEY_L]) tWhite -= stepAngle;

    // Luz azul (plano YX): I avanza, K retrocede
    if (keys[GLFW_KEY_I]) tBlue += stepAngle;
    if (keys[GLFW_KEY_K]) tBlue -= stepAngle;

    // (Opcional) animación del molino
    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }
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

    // (Opcional) ajustar radios al vuelo:
    // if (keys[GLFW_KEY_1]) ayWhite += 0.01f;
    // if (keys[GLFW_KEY_2]) bzWhite += 0.01f;
    // if (keys[GLFW_KEY_3]) ayBlue  += 0.01f;
    // if (keys[GLFW_KEY_4]) axBlue  += 0.01f;
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
