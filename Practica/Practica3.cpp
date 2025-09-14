//Práctica 3
//Celis Hernández Ronie
//30 de Agosto del 2025
//318143093


#include <iostream>

//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shaders
#include "Shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    // --- Inicialización GLFW ---
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica 3. Ronie Celis", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    // --- Inicialización GLEW ---
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialise GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    glViewport(0, 0, screenWidth, screenHeight);

    // Estado de OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Info del contexto
    std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    // Shaders
    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // --- Geometría de un cubo con color por vértice ---
    float vertices[] = {
        // posiciones          // colores
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  1.0f, 0.2f, 0.5f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.2f, 0.5f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.2f, 0.5f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.2f, 0.5f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.2f, 0.5f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.2f, 0.5f,
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

    // --- Proyección ---
    glm::mat4 projection = glm::perspective(glm::radians(40.0f),
        (GLfloat)screenWidth / (GLfloat)screenHeight,
        0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();

        glm::mat4 model = glm::mat4(1.0f);

        glm::vec3 c1(0.0f, 0.0f, 0.0f);
        glm::vec3 c2(1.0f, 0.6088f, 1.0f);
        glm::vec3 center = 0.5f * (c1 + c2);

        glm::vec3 eyeOffset(-2.1f, 1.2f, 2.0f);
        eyeOffset *= 1.6f;
        glm::vec3 eye = center + eyeOffset;

        glm::mat4 view = glm::lookAt(
            eye,
            center,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);

        // --- Dibujo de los cubos ---
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model2 = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f));
        model2 = glm::rotate(model2, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 1.0f, 0.4f));
        model3 = glm::rotate(model3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model3 = glm::scale(model3, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.8f, 1.0f, 1.0f));
        model4 = glm::rotate(model4, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model4 = glm::scale(model4, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 model5 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 1.6f, 0.7f));
        model5 = glm::rotate(model5, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model5 = glm::scale(model5, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return EXIT_SUCCESS;
}
