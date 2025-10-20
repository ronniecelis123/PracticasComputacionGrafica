//Ronie Celis Hernández
//318143093
//19 de Octubre del 2023


#include <iostream>
#include <cmath>
#include <cstdio>

// GLEW / GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// stb / SOIL2
#include "stb_image.h"
#include "SOIL2/SOIL2.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Proyecto
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// --- Prototipos ---
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// --- Ventana ---
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// --- Cámara ---
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH * 0.5f, lastY = HEIGHT * 0.5f;
bool keys[1024]{};
bool firstMouse = true;

// --- Tiempo ---
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// --- Brillo global (si quieres escalar todas las luces) ---
float g_GlobalLightBoost = 1.0f;

// =====================
//  BLENDER -> OPENGL
// =====================
// Conversión de posiciones/direcciones Blender(Z-up) -> OpenGL(Y-up): (x,y,z) -> (x,z,-y)
static inline glm::vec3 FromBlender(const glm::vec3& b) {
    return glm::vec3(b.x, b.z, -b.y);
}

struct BLPoint { glm::vec3 pos; float energy; };
static const BLPoint kBlenderPoints[] = {
    { {  0.0f, -3.8673f, 1.8027f }, 1000.0f },
    { {  0.0f,  1.4359f, 1.3330f }, 1000.0f }, 
};

struct BLDir { glm::vec3 dir; float intensity; };
static const BLDir kBlenderDirs[] = {
    { { -0.2f, -1.0f, -0.3f }, 1.0f },
    { {  0.4f, -0.7f,  0.2f }, 0.6f }  
};


struct BLSpot { glm::vec3 pos; glm::vec3 dir; float innerDeg, outerDeg; float intensity; glm::vec3 color; };
static const BLSpot kBlenderSpots[] = {

    { { -5.5106f, 0.59733f, 6.7701f },      // pos
      {  0.31275f, 0.00000f, -0.94984f },   // dir 
      12.5f, 17.5f,                         // inner / outer (grados)
      2.0f,                                 // intensidad
      { 1.0f, 1.0f, 1.0f } },               // color (blanco)

      { {  1.8585f, -4.0835f, 0.45771f },       // pos
        {  0.772468f, -0.083688f, -0.629515f }, // dir 
        12.5f, 17.5f,
        1.5f,
        { 1.0f, 1.0f, 1.0f } },                 // blanco

        // === SPOT 3 (nuevo, rojo) ===
        { {  1.1009f, -3.4955f, 0.45771f },     // pos 
          { -0.66079f, 0.05403f, -0.74908f },   // dir 
          12.5f, 17.5f,
          2.0f,
          { 1.0f, 0.0f, 0.0f } }                // ROJO
};

// ======== Cantidades ========
static const int NUM_POINT_LIGHTS =
(int)(sizeof(kBlenderPoints) / sizeof(kBlenderPoints[0]));
static const int NUM_DIR_LIGHTS =
(int)(sizeof(kBlenderDirs) / sizeof(kBlenderDirs[0]));
static const int NUM_SPOT_LIGHTS =
(int)(sizeof(kBlenderSpots) / sizeof(kBlenderSpots[0]));

// ======== Buffers ya convertidos a coordenadas OpenGL ========
glm::vec3 gPointPos[NUM_POINT_LIGHTS];
float     gPointEnergy[NUM_POINT_LIGHTS];

glm::vec3 gDirDir[NUM_DIR_LIGHTS];
float     gDirIntensity[NUM_DIR_LIGHTS];

glm::vec3 gSpotPos[NUM_SPOT_LIGHTS];
glm::vec3 gSpotDir[NUM_SPOT_LIGHTS];
float     gSpotInnerCos[NUM_SPOT_LIGHTS];
float     gSpotOuterCos[NUM_SPOT_LIGHTS];
float     gSpotIntensity[NUM_SPOT_LIGHTS];
glm::vec3 gSpotColor[NUM_SPOT_LIGHTS];   // <<<< NECESARIO para color por luz

// ======== Inicialización de conversiones ========
static void InitConvertedLights() {
    for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
        gPointPos[i] = FromBlender(kBlenderPoints[i].pos);
        gPointEnergy[i] = kBlenderPoints[i].energy;
    }
    for (int i = 0; i < NUM_DIR_LIGHTS; ++i) {
        gDirDir[i] = glm::normalize(FromBlender(kBlenderDirs[i].dir));
        gDirIntensity[i] = kBlenderDirs[i].intensity;
    }
    for (int i = 0; i < NUM_SPOT_LIGHTS; ++i) {
        gSpotPos[i] = FromBlender(kBlenderSpots[i].pos);
        gSpotDir[i] = glm::normalize(FromBlender(kBlenderSpots[i].dir));
        gSpotInnerCos[i] = glm::cos(glm::radians(kBlenderSpots[i].innerDeg));
        gSpotOuterCos[i] = glm::cos(glm::radians(kBlenderSpots[i].outerDeg));
        gSpotIntensity[i] = kBlenderSpots[i].intensity;
        gSpotColor[i] = kBlenderSpots[i].color; // <<<< cargar color
    }
}

int main() {
    // Init GLFW
    if (!glfwInit()) { std::cout << "Failed to init GLFW\n"; return EXIT_FAILURE; }

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9.Ronie Celis", nullptr, nullptr);
    if (!window) { std::cout << "Failed to create GLFW window\n"; glfwTerminate(); return EXIT_FAILURE; }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) { std::cout << "Failed to initialize GLEW\n"; return EXIT_FAILURE; }

    // Viewport y opciones GL
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);
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

    // Shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Modelos
    Model Dog((char*)"Models/EscenarioLuces.obj");

    // Conversión de luces
    InitConvertedLights();

    // Proyección
    glm::mat4 projection = glm::perspective(
        camera.GetZoom(),
        (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT,
        0.5f,   // mejor precisión que 0.1
        600.0f
    );

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        // Tiempo
        GLfloat currentFrame = (GLfloat)glfwGetTime();
        deltaTime = currentFrame - lastFrame; lastFrame = currentFrame;

        // Input
        glfwPollEvents();
        DoMovement();

        // Clear
        glClearColor(0.12f, 0.12f, 0.13f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // === Luces y material ===
        lightingShader.Use();

        // Material (coincide con lighting.frag: "material")
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        // Cámara
        glm::mat4 view = camera.GetViewMatrix();
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // ===== Direccionales (arreglo) =====
        for (int i = 0; i < NUM_DIR_LIGHTS; ++i) {
            char name[64];
            // Escala de intensidades base (en blanco; si quieres color direccional, añade color al BLDir)
            const float ambB = 0.06f * g_GlobalLightBoost * gDirIntensity[i];
            const float difB = 0.12f * g_GlobalLightBoost * gDirIntensity[i];
            const float speB = 0.22f * g_GlobalLightBoost * gDirIntensity[i];

            std::snprintf(name, sizeof(name), "dirLights[%d].direction", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name),
                gDirDir[i].x, gDirDir[i].y, gDirDir[i].z);

            std::snprintf(name, sizeof(name), "dirLights[%d].ambient", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name), ambB, ambB, ambB);

            std::snprintf(name, sizeof(name), "dirLights[%d].diffuse", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name), difB, difB, difB);

            std::snprintf(name, sizeof(name), "dirLights[%d].specular", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name), speB, speB, speB);
        }

        // ===== Puntuales (arreglo) =====
        for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
            char name[64];

            // Energía Blender ? factor
            const float eBoost = gPointEnergy[i] / 1000.0f;

            glm::vec3 amb = glm::vec3(0.04f) * (0.7f * eBoost) * g_GlobalLightBoost;
            glm::vec3 dif = glm::vec3(0.90f) * eBoost * g_GlobalLightBoost;
            glm::vec3 spe = glm::vec3(0.55f) * eBoost * g_GlobalLightBoost;

            // Atenuación base
            float kc = 1.0f, kl = 0.045f, kq = 0.0075f;
            if (eBoost > 1.5f) { kl = 0.028f; kq = 0.0019f; } // menos atenuación si es potente

            // Heurística opcional por profundidad (como en tu Previo)
            if (gPointPos[i].z < 5.0f) { dif *= 0.7f; spe *= 0.5f; kl = 0.060f; kq = 0.0170f; }
            else if (gPointPos[i].z < 15.0f) { dif *= 0.90f; }

            std::snprintf(name, sizeof(name), "pointLights[%d].position", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name),
                gPointPos[i].x, gPointPos[i].y, gPointPos[i].z);

            std::snprintf(name, sizeof(name), "pointLights[%d].constant", i);
            glUniform1f(glGetUniformLocation(lightingShader.Program, name), kc);

            std::snprintf(name, sizeof(name), "pointLights[%d].linear", i);
            glUniform1f(glGetUniformLocation(lightingShader.Program, name), kl);

            std::snprintf(name, sizeof(name), "pointLights[%d].quadratic", i);
            glUniform1f(glGetUniformLocation(lightingShader.Program, name), kq);

            std::snprintf(name, sizeof(name), "pointLights[%d].ambient", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name), amb.x, amb.y, amb.z);

            std::snprintf(name, sizeof(name), "pointLights[%d].diffuse", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name), dif.x, dif.y, dif.z);

            std::snprintf(name, sizeof(name), "pointLights[%d].specular", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name), spe.x, spe.y, spe.z);
        }

        // ===== Spotlights (arreglo) =====
        for (int i = 0; i < NUM_SPOT_LIGHTS; ++i) {
            char name[64];

            // >>> AQUÍ ESTÁ EL COLOR POR LUZ <<<
            // Ganancias base: sube diffuse para que el color “pinte” más.
            glm::vec3 amb = 0.02f * gSpotIntensity[i] * g_GlobalLightBoost * gSpotColor[i];
            glm::vec3 dif = 1.40f * gSpotIntensity[i] * g_GlobalLightBoost * gSpotColor[i];
            glm::vec3 spe = 0.60f * gSpotIntensity[i] * g_GlobalLightBoost * gSpotColor[i];

            const float kc = 1.0f, kl = 0.045f, kq = 0.0075f;

            std::snprintf(name, sizeof(name), "spotLights[%d].position", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name),
                gSpotPos[i].x, gSpotPos[i].y, gSpotPos[i].z);

            std::snprintf(name, sizeof(name), "spotLights[%d].direction", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name),
                gSpotDir[i].x, gSpotDir[i].y, gSpotDir[i].z);

            std::snprintf(name, sizeof(name), "spotLights[%d].cutOff", i);
            glUniform1f(glGetUniformLocation(lightingShader.Program, name), gSpotInnerCos[i]);

            std::snprintf(name, sizeof(name), "spotLights[%d].outerCutOff", i);
            glUniform1f(glGetUniformLocation(lightingShader.Program, name), gSpotOuterCos[i]);

            std::snprintf(name, sizeof(name), "spotLights[%d].constant", i);
            glUniform1f(glGetUniformLocation(lightingShader.Program, name), kc);

            std::snprintf(name, sizeof(name), "spotLights[%d].linear", i);
            glUniform1f(glGetUniformLocation(lightingShader.Program, name), kl);

            std::snprintf(name, sizeof(name), "spotLights[%d].quadratic", i);
            glUniform1f(glGetUniformLocation(lightingShader.Program, name), kq);

            std::snprintf(name, sizeof(name), "spotLights[%d].ambient", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name), amb.x, amb.y, amb.z);

            std::snprintf(name, sizeof(name), "spotLights[%d].diffuse", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name), dif.x, dif.y, dif.z);

            std::snprintf(name, sizeof(name), "spotLights[%d].specular", i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, name), spe.x, spe.y, spe.z);
        }

        // ===== Matrices y draw =====
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
        Dog.Draw(lightingShader);

        // Swap
        glfwSwapBuffers(window);
    }

    // Cleanup
    glfwTerminate();
    return 0;
}

// --- Movimiento cámara y edición en vivo de UNA puntual (índice 0) ---
void DoMovement() {
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    // Mover la point[0] a mano (opcional)
    if (NUM_POINT_LIGHTS > 0) {
        if (keys[GLFW_KEY_T]) gPointPos[0].x += 0.01f;
        if (keys[GLFW_KEY_G]) gPointPos[0].x -= 0.01f;
        if (keys[GLFW_KEY_Y]) gPointPos[0].y += 0.01f;
        if (keys[GLFW_KEY_H]) gPointPos[0].y -= 0.01f;
        if (keys[GLFW_KEY_U]) gPointPos[0].z -= 0.10f;
        if (keys[GLFW_KEY_J]) gPointPos[0].z += 0.01f;
    }
}

// --- Input ---
void KeyCallback(GLFWwindow* window, int key, int, int action, int) {
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) { lastX = (GLfloat)xPos; lastY = (GLfloat)yPos; firstMouse = false; }
    GLfloat xOffset = (GLfloat)xPos - lastX;
    GLfloat yOffset = lastY - (GLfloat)yPos;
    lastX = (GLfloat)xPos; lastY = (GLfloat)yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}
