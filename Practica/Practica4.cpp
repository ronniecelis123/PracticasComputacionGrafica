
//Práctica 4
//Celis Hernández Ronie
//07 de septiembre del 2025
//318143093


#include<iostream>

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 04. Ronie Celis", nullptr, nullptr);
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);


    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Estado de OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Info del contexto
    std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // --- Vértices (Cubo color Pluto) ---
    float vertices[] = {
        // Cara frontal
        -0.5f, -0.5f,  0.5f,  0.98f, 0.62f, 0.17f,
         0.5f, -0.5f,  0.5f,  0.98f, 0.62f, 0.17f,
         0.5f,  0.5f,  0.5f,  0.98f, 0.62f, 0.17f,
         0.5f,  0.5f,  0.5f,  0.98f, 0.62f, 0.17f,
        -0.5f,  0.5f,  0.5f,  0.98f, 0.62f, 0.17f,
        -0.5f, -0.5f,  0.5f,  0.98f, 0.62f, 0.17f,

        // Cara trasera
        -0.5f, -0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
         0.5f, -0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
         0.5f,  0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
         0.5f,  0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
        -0.5f,  0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
        -0.5f, -0.5f, -0.5f,  0.98f, 0.72f, 0.20f,

        // Cara derecha (con sombra)
         0.5f, -0.5f,  0.5f,  0.88f, 0.72f, 0.20f,
         0.5f, -0.5f, -0.5f,  0.88f, 0.72f, 0.20f,
         0.5f,  0.5f, -0.5f,  0.88f, 0.72f, 0.20f,
         0.5f,  0.5f, -0.5f,  0.88f, 0.72f, 0.20f,
         0.5f,  0.5f,  0.5f,  0.88f, 0.72f, 0.20f,
         0.5f, -0.5f,  0.5f,  0.88f, 0.72f, 0.20f,

         // Cara izquierda ----
         -0.5f,  0.5f,  0.5f,  0.98f, 0.72f, 0.20f,
         -0.5f,  0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
         -0.5f, -0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
         -0.5f, -0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
         -0.5f, -0.5f,  0.5f,  0.98f, 0.72f, 0.20f,
         -0.5f,  0.5f,  0.5f,  0.98f, 0.72f, 0.20f,

         // Cara inferior ....-
         -0.5f, -0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
          0.5f, -0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
          0.5f, -0.5f,  0.5f,  0.98f, 0.72f, 0.20f,
          0.5f, -0.5f,  0.5f,  0.98f, 0.72f, 0.20f,
         -0.5f, -0.5f,  0.5f,  0.98f, 0.72f, 0.20f,
         -0.5f, -0.5f, -0.5f,  0.98f, 0.72f, 0.20f,

         // Cara superior
         -0.5f,  0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
          0.5f,  0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
          0.5f,  0.5f,  0.5f,  0.98f, 0.72f, 0.20f,
          0.5f,  0.5f,  0.5f,  0.98f, 0.72f, 0.20f,
         -0.5f,  0.5f,  0.5f,  0.98f, 0.72f, 0.20f,
         -0.5f,  0.5f, -0.5f,  0.98f, 0.72f, 0.20f,
    };


    // --- Vértices (Cubo de color verde) ---
    // --- Vértices (Cubo de color verde oscuro más claro) ---
    float verticesGreen[] = {
        // Cara frontal
        -0.5f, -0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.55f, 0.0f,

         0.5f,  0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.55f, 0.0f,

        // Cara trasera
        -0.5f, -0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.55f, 0.0f,

         0.5f,  0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.55f, 0.0f,

        // Cara derecha
         0.5f, -0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.55f, 0.0f,

         0.5f,  0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.55f, 0.0f,

         // Cara izquierda
         -0.5f,  0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 0.55f, 0.0f,

         -0.5f, -0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 0.55f, 0.0f,

         // Cara inferior
         -0.5f, -0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 0.55f, 0.0f,

          0.5f, -0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 0.55f, 0.0f,

         // Cara superior
         -0.5f,  0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
          0.5f,  0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
          0.5f,  0.5f,  0.5f,  0.0f, 0.55f, 0.0f,

          0.5f,  0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 0.55f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 0.55f, 0.0f,
    };

    // --- Vértices (Cubo de color negro) ---
    float verticesBlack[] = {
        // Cara frontal
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

        // Cara trasera
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

        // Cara derecha
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

         // Cara izquierda
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

         // Cara inferior
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

          0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

         // Cara superior
         -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
          0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
          0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

          0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
    };

    // --- Vértices (Cubo de color blanco) ---
    float verticesWhite[] = {
        // Cara frontal
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

        // Cara trasera
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

        // Cara derecha
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

         // Cara izquierda
         -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

         // Cara inferior
         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

          0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

         // Cara superior
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,

          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
    };


    float verticesPink[] = {
        // Cara frontal
        -0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,

        // Cara trasera
        -0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.6f,

         0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.6f,

        // Cara derecha
         0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.6f,

         0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,

         // Cara izquierda
         -0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         -0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
         -0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.6f,

         -0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
         -0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         -0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,

         // Cara inferior
         -0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
          0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,

          0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         -0.5f, -0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         -0.5f, -0.5f, -0.5f,  1.0f, 0.6f, 0.6f,

         // Cara superior
         -0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
          0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,

          0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         -0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.6f,
         -0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.6f,
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Crear VAO/VBO para el cubo verde
    GLuint VBOgreen, VAOgreen;
    glGenVertexArrays(1, &VAOgreen);
    glGenBuffers(1, &VBOgreen);

    glBindVertexArray(VAOgreen);
    glBindBuffer(GL_ARRAY_BUFFER, VBOgreen);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesGreen), verticesGreen, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //Para el color negro 
    GLuint VBOblack, VAOblack;
    glGenVertexArrays(1, &VAOblack);
    glGenBuffers(1, &VBOblack);

    glBindVertexArray(VAOblack);
    glBindBuffer(GL_ARRAY_BUFFER, VBOblack);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBlack), verticesBlack, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    //Para el color blanco

    GLuint VBOwhite, VAOwhite;
    glGenVertexArrays(1, &VAOwhite);
    glGenBuffers(1, &VBOwhite);

    glBindVertexArray(VAOwhite);
    glBindBuffer(GL_ARRAY_BUFFER, VBOwhite);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWhite), verticesWhite, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //para el color rosa
    GLuint VBOpink, VAOpink;
    glGenVertexArrays(1, &VAOpink);
    glGenBuffers(1, &VBOpink);

    glBindVertexArray(VAOpink);
    glBindBuffer(GL_ARRAY_BUFFER, VBOpink);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPink), verticesPink, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        Inputs(window);
        glfwPollEvents();

        glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // azul cielo

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 view = glm::mat4(1);
        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // --- CUBO 1: Escalado ---
        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::scale(model1, glm::vec3(1.2f, 1.2f, 1.3f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model1));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // --- CUBO 2: Sin escala pero desplazado ---
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(1.0f, 0.0f, 0.0f)); // mueve a la derecha
        model2 = glm::scale(model2, glm::vec3(1.4f, 1.2f, 0.8f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Parte final del torso
        glm::mat4 model3 = glm::mat4(1.0f);
        model3 = glm::translate(model3, glm::vec3(1.8f, 0.0f, 0.0f)); // mueve a la derecha
        model3 = glm::scale(model3, glm::vec3(0.5f, 1.2f, 1.3f)); //1.5 lo que le da el ancho al torso

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Se modelan las patas
        glm::mat4 model4 = glm::mat4(1.0f);
        model4 = glm::translate(model4, glm::vec3(1.8f, -1.0f, -0.4f));
        model4 = glm::scale(model4, glm::vec3(0.4f, 1.7f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //parte posterior de la pata
        glm::mat4 model5 = glm::mat4(1.0f);
        model5 = glm::translate(model5, glm::vec3(1.4f, -1.7f, -0.4f));
        model5 = glm::scale(model5, glm::vec3(0.4f, 0.3f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Se modela la segunda pata
        glm::mat4 model6 = glm::mat4(1.0f);
        model6 = glm::translate(model6, glm::vec3(1.8f, -1.0f, 0.4f));
        model6 = glm::scale(model6, glm::vec3(0.4f, 1.7f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //parte posterior de la pata
        glm::mat4 model7 = glm::mat4(1.0f);
        model7 = glm::translate(model7, glm::vec3(1.4f, -1.7f, 0.4f));
        model7 = glm::scale(model7, glm::vec3(0.4f, 0.3f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model7));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Se modela la tercera pata
        glm::mat4 model8 = glm::mat4(1.0f);
        model8 = glm::translate(model8, glm::vec3(-0.3f, -1.0f, 0.4f));
        model8 = glm::scale(model8, glm::vec3(0.4f, 1.7f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model8));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //parte posterior de la pata
        glm::mat4 model9 = glm::mat4(1.0f);
        model9 = glm::translate(model9, glm::vec3(-0.7f, -1.7f, 0.4f));
        model9 = glm::scale(model9, glm::vec3(0.4f, 0.3f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model9));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Se modela la tercera pata
        glm::mat4 model10 = glm::mat4(1.0f);
        model10 = glm::translate(model10, glm::vec3(-0.3f, -1.0f, -0.4f));
        model10 = glm::scale(model10, glm::vec3(0.4f, 1.7f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model10));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //parte posterior de la pata
        glm::mat4 model11 = glm::mat4(1.0f);
        model11 = glm::translate(model11, glm::vec3(-0.7f, -1.7f, -0.4f));
        model11 = glm::scale(model11, glm::vec3(0.4f, 0.3f, 0.4));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model11));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Se modela el collar verde
        // --- DIBUJA EL CUBO VERDE ---
        glm::mat4 modelCollar = glm::mat4(1.0f);
        modelCollar = glm::translate(modelCollar, glm::vec3(-0.1f, 0.73f, 0.0f)); // mueve el cubo a la derecha
        modelCollar = glm::scale(modelCollar, glm::vec3(1.0f, 0.25f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCollar));
        glBindVertexArray(VAOgreen);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Se modela la parte de la cara inferior
        glm::mat4 model12 = glm::mat4(1.0f);
        model12 = glm::translate(model12, glm::vec3(-0.4f, 0.95f, 0.0f));
        model12 = glm::scale(model12, glm::vec3(1.1f, 0.2f, 0.9f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model12));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Se modela el contorno de atras de la cara
        glm::mat4 model13 = glm::mat4(1.0f);
        model13 = glm::translate(model13, glm::vec3(-0.15f, 1.2f, 0.0f));
        model13 = glm::scale(model13, glm::vec3(0.6f, 0.3f, 0.9f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model13));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Se modela la parte de la cara superior
        glm::mat4 model14 = glm::mat4(1.0f);
        model14 = glm::translate(model14, glm::vec3(-0.4f, 1.4f, 0.0f));
        model14 = glm::scale(model14, glm::vec3(1.1f, 0.3f, 0.9f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model14));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Se modela la cara que va a lo alto
        glm::mat4 model15 = glm::mat4(1.0f);
        model15 = glm::translate(model15, glm::vec3(-0.14f, 1.8f, 0.0f));
        model15 = glm::scale(model15, glm::vec3(0.6f, 0.6f, 0.9f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model15));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        //Empezamos con los ojos partes de los extremos
        glm::mat4 model16 = glm::mat4(1.0f);
        model16 = glm::translate(model16, glm::vec3(-0.48f, 1.75f, 0.4f)); // posición
        model16 = glm::scale(model16, glm::vec3(0.06f, 0.4f, 0.1f));     // tamaño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model16));

        glBindVertexArray(VAOwhite);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glm::mat4 model17 = glm::mat4(1.0f);
        model17 = glm::translate(model17, glm::vec3(-0.48f, 1.75f, -0.4f)); // posición
        model17 = glm::scale(model17, glm::vec3(0.06f, 0.4f, 0.1f));     // tamaño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model17));
        glDrawArrays(GL_TRIANGLES, 0, 36);



        //Se crean las uniones con los ojos
        glm::mat4 model18 = glm::mat4(1.0f);
        model18 = glm::translate(model18, glm::vec3(-0.50f, 1.75f, 0.3f)); // posición
        model18 = glm::scale(model18, glm::vec3(0.06f, 0.4f, 0.3f));     // tamaño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model18));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        glm::mat4 model19 = glm::mat4(1.0f);
        model19 = glm::translate(model19, glm::vec3(-0.50f, 1.75f, -0.3f)); // posición
        model19 = glm::scale(model19, glm::vec3(0.06f, 0.4f, 0.3f));     // tamaño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model19));
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //UNION ENTRE AMBOS
        glm::mat4 model20 = glm::mat4(1.0f);
        model20 = glm::translate(model20, glm::vec3(-0.50f, 1.70f, 0.0f)); // posición
        model20 = glm::scale(model20, glm::vec3(0.06f, 0.3f, 0.6f));     // tamaño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model20));
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //Parte de la cabeza restante
        glm::mat4 model21 = glm::mat4(1.0f);
        model21 = glm::translate(model21, glm::vec3(-0.5f, 2.04f, 0.0f));
        model21 = glm::scale(model21, glm::vec3(0.08f, 0.14f, 0.9f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model21));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //Partes negras del modelo 
        //los ojos
        glm::mat4 model22 = glm::mat4(1.0f);
        model22 = glm::translate(model22, glm::vec3(-0.5f, 1.65f, 0.2f)); // posición
        model22 = glm::scale(model22, glm::vec3(0.08f, 0.25f, 0.2f));     // tamaño pequeño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model22));
        glBindVertexArray(VAOblack);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glm::mat4 model23 = glm::mat4(1.0f);
        model23 = glm::translate(model23, glm::vec3(-0.5f, 1.65f, -0.2f)); // posición
        model23 = glm::scale(model23, glm::vec3(0.08f, 0.25f, 0.20f));     // tamaño pequeño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model23));
        glBindVertexArray(VAOblack);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //la nariz
        glm::mat4 model24 = glm::mat4(1.0f);
        model24 = glm::translate(model24, glm::vec3(-0.9f, 1.50f, 0.0f)); // posición
        model24 = glm::scale(model24, glm::vec3(0.3f, 0.3f, 0.3f));     // tamaño pequeño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model24));
        glBindVertexArray(VAOblack);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //la cola
        glm::mat4 model25 = glm::mat4(1.0f);
        model25 = glm::translate(model25, glm::vec3(2.2f, 0.95f, 0.0f)); // posición
        model25 = glm::scale(model25, glm::vec3(0.3f, 1.4f, 0.3f));     // tamaño pequeño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model25));
        glBindVertexArray(VAOblack);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model26 = glm::mat4(1.0f);
        model26 = glm::translate(model26, glm::vec3(2.5f, 1.50f, 0.0f)); // posición
        model26 = glm::scale(model26, glm::vec3(0.3f, 0.3f, 0.3f));     // tamaño pequeño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model26));
        glBindVertexArray(VAOblack);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

         //Siguen las orejas
        glm::mat4 model27 = glm::mat4(1.0f);
        model27= glm::translate(model27, glm::vec3(-0.15f, 1.75f, 0.5f));
        model27 = glm::scale(model27, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model27));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        //la parte negra de la oreja
        //oreja derecha
        glm::mat4 model29 = glm::mat4(1.0f);
        model29 = glm::translate(model29, glm::vec3(-0.15f, 2.1f, 0.5f)); // posición
        model29 = glm::scale(model29, glm::vec3(0.15f, 0.8f, 0.13f));     // tamaño pequeño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model29));
        glBindVertexArray(VAOblack);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model30 = glm::mat4(1.0f);
        model30 = glm::translate(model30, glm::vec3(-0.2f, 2.5f, 0.5f)); // posición
        model30 = glm::scale(model30, glm::vec3(0.24f, 0.15f, 0.13f));     // tamaño pequeño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model30));
        glBindVertexArray(VAOblack);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //oreja izquierda
        glm::mat4 model33 = glm::mat4(1.0f);
        model33 = glm::translate(model33, glm::vec3(-0.15f, 1.75f, -0.5f));
        model33 = glm::scale(model33, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model33));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model31 = glm::mat4(1.0f);
        model31 = glm::translate(model31, glm::vec3(-0.15f, 2.1f, -0.5f)); // posición
        model31 = glm::scale(model31, glm::vec3(0.15f, 0.8f, 0.13f));     // tamaño pequeño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model31));
        glBindVertexArray(VAOblack);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model32 = glm::mat4(1.0f);
        model32 = glm::translate(model32, glm::vec3(-0.2f, 2.5f, -0.5f)); // posición
        model32 = glm::scale(model32, glm::vec3(0.24f, 0.15f, 0.13f));     // tamaño pequeño
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model32));
        glBindVertexArray(VAOblack);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      

        //cubo de la parte superior de la cabeza
        glm::mat4 model34 = glm::mat4(1.0f);
        model34 = glm::translate(model34, glm::vec3(-0.2f, 2.2f, -0.0f));
        model34 = glm::scale(model34, glm::vec3(0.45f, 0.25f, 0.45f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model34));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        //la lengua
        glm::mat4 model35 = glm::mat4(1.0f);
        model35 = glm::translate(model35, glm::vec3(-0.8f, 1.1f, 0.0f)); 
        model35 = glm::scale(model35, glm::vec3(0.6f, 0.15f, 0.35f));    
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model35));
        glBindVertexArray(VAOpink);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model36 = glm::mat4(1.0f);
        model36 = glm::translate(model36, glm::vec3(-1.1f, 1.0f, 0.0f)); // delante de la boca
        model36 = glm::scale(model36, glm::vec3(0.1f, 0.35f, 0.35f));    // tamaño de la lengua
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model36));
        glBindVertexArray(VAOpink);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        glfwSwapBuffers(window);
        glBindVertexArray(0);

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX += 0.03f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movX -= 0.03f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        movY += 0.02f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        movY -= 0.02f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movZ -= 0.03f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movZ += 0.03f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot += 0.3f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot -= 0.3f;
}
