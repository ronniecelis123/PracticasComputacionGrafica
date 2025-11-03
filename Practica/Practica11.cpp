// Ronie Celis Hernández
// 318143093
// 03 de Noviembre del 2025
// Practica 11

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.0f,2.0f, 0.0f),
    glm::vec3(0.0f,0.0f, 0.0f),
    glm::vec3(0.0f,0.0f,  0.0f),
    glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
    // posiciones        // normales
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 Light1 = glm::vec3(0);

// Anim
float rotBall = 0.0f;
bool AnimBall = false;
bool AnimDog = false;
float rotDog = 0.0f;

// Estados del perro
// 0=Idle, 1=C1, 2=G1, 3=C2, 4=G2, 5=C3(L), 6=giro der corto, 7=avance corto, 8=apuntar origen, 9=volver origen, 10=avance corto final
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
glm::vec3 dogPos(0.0f, 0.0f, 0.0f);
float dogRot = 0.0f; // grados yaw
bool step = false;

// Caminata del perro
float distanciaTotal = 2.35f;
float distanciaCubierta = 0.0f;
float velocidad = 0.001f;

// Tramo final largo
float distanciaFinal = distanciaTotal * 1.8f;

// Distancia corta hacia adelante (nuevo estado 10)
float distanciaCortaAdelante = 0.40f;

// Giros
float anguloTotal = 90.0f;
float anguloCubierto = 0.0f;
float baseVelocidadGiro = 0.4f; // baja a 0.3f si quieres más suavidad
float velocidadGiro = 0.4f;

// Metas “hacia adentro” y regreso
float anguloObjetivo = 0.0f;     // grados
float distanciaObjetivo = 0.0f;  // unidades

// Mantener sentido del último giro (+1 izq, -1 der)
int lastTurnSign = +1;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    // Init GLFW
    glfwInit();

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 11. Ronie Celis", nullptr, nullptr);
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLEW
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

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Models
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // VAO/VBO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Delta
        const float t = static_cast<float>(glfwGetTime());
        deltaTime = t - lastFrame;
        lastFrame = t;

        glfwPollEvents();
        DoMovement();
        Animation();

        // Clear
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        // Lighting shader
        lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Dir light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);

        // Point light 1 (animada)
        glm::vec3 lightColor;
        lightColor.x = std::abs(static_cast<float>(std::sin(t * Light1.x)));
        lightColor.y = std::abs(static_cast<float>(std::sin(t * Light1.y)));
        lightColor.z = static_cast<float>(std::sin(t * Light1.z));

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
            lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

        // Spotlight (cámara)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

        // Material
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        // View/projection
        glm::mat4 view = camera.GetViewMatrix();
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // ---------- OJO: model y modelTemp se declaran UNA sola vez ----------
        glm::mat4 model(1.0f), modelTemp(1.0f);

        // Piso
        view = camera.GetViewMatrix();
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        // Perro
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        // Body
        modelTemp = model = glm::translate(model, dogPos);
        modelTemp = model = glm::rotate(model, glm::radians(dogRot), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        // Head
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        // Tail
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        // Front Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        // Front Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        // Back Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046f, -0.218f));
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        // Back Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);


        glBindVertexArray(0);

        // Lamp shader
        lampShader.Use();
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 lamp(1.0f);
        lamp = glm::translate(lamp, lightPos);
        lamp = glm::scale(lamp, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lamp));

        lamp = glm::mat4(1.0f);
        lamp = glm::translate(lamp, pointLightPositions[0]);
        lamp = glm::scale(lamp, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lamp));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Camera movement
void DoMovement()
{
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])  camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])  camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) camera.ProcessKeyboard(RIGHT, deltaTime);

    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;

    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;

    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

// Key callback
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)      keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    if (keys[GLFW_KEY_SPACE])
    {
        active = !active;
        if (active) Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
        else        Light1 = glm::vec3(0.0f);
    }

    if (keys[GLFW_KEY_N]) { AnimBall = !AnimBall; }

    // Inicia la SECUENCIA automática
    if (keys[GLFW_KEY_B]) {
        if (dogAnim == 0) {
            dogAnim = 1;
            distanciaCubierta = 0.0f;
            dogPos = glm::vec3(0.0f);  // arranque desde origen
            dogRot = 0.0f;
            step = false;
        }
    }
}

// Máquina de estados con regreso al origen y sin “finta” + avance corto final
void Animation()
{
    if (AnimBall) rotBall += 0.4f;
    if (AnimDog)  rotDog -= 0.6f;

    // --- Estado 1: Caminar Tramo 1 ---
    if (dogAnim == 1) {
        if (distanciaCubierta < distanciaTotal) {
            if (!step) { RLegs += 0.03f; FLegs += 0.03f; head += 0.03f; tail += 0.03f; if (RLegs > 15.0f) step = true; }
            else { RLegs -= 0.03f; FLegs -= 0.03f; head -= 0.03f; tail -= 0.03f; if (RLegs < -15.0f) step = false; }
            float radRot = glm::radians(dogRot);
            dogPos.x += std::sin(radRot) * velocidad;
            dogPos.z += std::cos(radRot) * velocidad;
            distanciaCubierta += velocidad;
        }
        else {
            dogAnim = 2;
            anguloCubierto = 0.0f;
            RLegs = FLegs = head = tail = 0.0f;
            velocidadGiro = baseVelocidadGiro;
        }
    }
    // --- Estado 2: Girar 1 (Izquierda) ---
    else if (dogAnim == 2) {
        if (anguloCubierto < anguloTotal) {
            dogRot += velocidadGiro; // izquierda
            lastTurnSign = +1;
            anguloCubierto += velocidadGiro;
            if (!step) { head += 0.03f; tail += 0.03f; if (head > 5.0f) step = true; }
            else { head -= 0.03f; tail -= 0.03f; if (head < -5.0f) step = false; }
        }
        else {
            dogAnim = 3;
            distanciaCubierta = 0.0f;
            RLegs = FLegs = head = tail = 0.0f;
        }
    }
    // --- Estado 3: Caminar Tramo 2 ---
    else if (dogAnim == 3) {
        if (distanciaCubierta < distanciaTotal) {
            if (!step) { RLegs += 0.03f; FLegs += 0.03f; head += 0.03f; tail += 0.03f; if (RLegs > 15.0f) step = true; }
            else { RLegs -= 0.03f; FLegs -= 0.03f; head -= 0.03f; tail -= 0.03f; if (RLegs < -15.0f) step = false; }
            float radRot = glm::radians(dogRot);
            dogPos.x += std::sin(radRot) * velocidad;
            dogPos.z += std::cos(radRot) * velocidad;
            distanciaCubierta += velocidad;
        }
        else {
            dogAnim = 4;
            anguloCubierto = 0.0f;
            RLegs = FLegs = head = tail = 0.0f;
            velocidadGiro = baseVelocidadGiro;
        }
    }
    // --- Estado 4: Girar 2 (Izquierda) ---
    else if (dogAnim == 4) {
        if (anguloCubierto < anguloTotal) {
            dogRot += velocidadGiro; // izquierda
            lastTurnSign = +1;
            anguloCubierto += velocidadGiro;
            if (!step) { head += 0.03f; tail += 0.03f; if (head > 5.0f) step = true; }
            else { head -= 0.03f; tail -= 0.03f; if (head < -5.0f) step = false; }
        }
        else {
            dogAnim = 5;
            distanciaCubierta = 0.0f;
            RLegs = FLegs = head = tail = 0.0f;
        }
    }
    // --- Estado 5: Caminar Tramo 3 (Largo) ---
    else if (dogAnim == 5) {
        if (distanciaCubierta < distanciaFinal) {
            if (!step) { RLegs += 0.03f; FLegs += 0.03f; head += 0.03f; tail += 0.03f; if (RLegs > 15.0f) step = true; }
            else { RLegs -= 0.03f; FLegs -= 0.03f; head -= 0.03f; tail -= 0.03f; if (RLegs < -15.0f) step = false; }
            float radRot = glm::radians(dogRot);
            dogPos.x += std::sin(radRot) * velocidad;
            dogPos.z += std::cos(radRot) * velocidad;
            distanciaCubierta += velocidad;
        }
        else {
            // Preparar "hacia adentro"
            dogAnim = 6;
            anguloCubierto = 0.0f;
            distanciaCubierta = 0.0f;
            RLegs = FLegs = head = tail = 0.0f;
            anguloObjetivo = 20.0f;     // pequeño giro a la derecha
            distanciaObjetivo = 0.40f;  // paso corto
            velocidadGiro = baseVelocidadGiro; // usaremos '-' al aplicar
        }
    }
    // --- Estado 6: pequeño giro a la derecha ---
    else if (dogAnim == 6) {
        if (anguloCubierto < anguloObjetivo) {
            dogRot -= velocidadGiro; // derecha
            lastTurnSign = -1;
            anguloCubierto += velocidadGiro;
            if (!step) { head += 0.03f; tail += 0.03f; if (head > 5.0f) step = true; }
            else { head -= 0.03f; tail -= 0.03f; if (head < -5.0f) step = false; }
        }
        else {
            dogAnim = 7;
            distanciaCubierta = 0.0f;
            RLegs = FLegs = head = tail = 0.0f;
        }
    }
    // --- Estado 7: avance corto hacia adentro ---
    else if (dogAnim == 7) {
        if (distanciaCubierta < distanciaObjetivo) {
            if (!step) { RLegs += 0.03f; FLegs += 0.03f; head += 0.03f; tail += 0.03f; if (RLegs > 15.0f) step = true; }
            else { RLegs -= 0.03f; FLegs -= 0.03f; head -= 0.03f; tail -= 0.03f; if (RLegs < -15.0f) step = false; }
            float radRot = glm::radians(dogRot);
            dogPos.x += std::sin(radRot) * velocidad;
            dogPos.z += std::cos(radRot) * velocidad;
            distanciaCubierta += velocidad;
        }
        else {
            // Alinear rumbo al origen
            dogAnim = 8;
            anguloCubierto = 0.0f;
            RLegs = FLegs = head = tail = 0.0f;

            // Ángulo objetivo para mirar al origen (FLOATS ? sin C4244)
            float objetivoYaw = glm::degrees(static_cast<float>(std::atan2(-dogPos.x, -dogPos.z)));

            auto norm180 = [](float a) {
                while (a > 180.0f)  a -= 360.0f;
                while (a < -180.0f) a += 360.0f;
                return a;
                };

            float diff = norm180(objetivoYaw - dogRot);

            // Mantén el mismo sentido del giro previo (evita “finta”)
            if (diff * lastTurnSign < 0.0f) {
                diff = norm180(diff + 360.0f * lastTurnSign);
            }

            anguloObjetivo = std::fabs(diff);
            velocidadGiro = (diff >= 0.0f) ? baseVelocidadGiro : -baseVelocidadGiro;
        }
    }
    // --- Estado 8: girar para apuntar al origen ---
    else if (dogAnim == 8) {
        if (anguloCubierto < anguloObjetivo) {
            dogRot += velocidadGiro; // usa el signo ya fijado
            lastTurnSign = (velocidadGiro > 0.0f) ? +1 : -1;
            anguloCubierto += std::fabs(velocidadGiro);
            if (!step) { head += 0.03f; tail += 0.03f; if (head > 5.0f) step = true; }
            else { head -= 0.03f; tail -= 0.03f; if (head < -5.0f) step = false; }
        }
        else {
            dogAnim = 9;
            distanciaCubierta = 0.0f;
            RLegs = FLegs = head = tail = 0.0f;
            distanciaObjetivo = std::sqrt(dogPos.x * dogPos.x + dogPos.z * dogPos.z);
            velocidadGiro = baseVelocidadGiro; // restaurar
        }
    }
    else if (dogAnim == 9) {
        if (distanciaCubierta < distanciaObjetivo) {
            if (!step) { RLegs += 0.03f; FLegs += 0.03f; head += 0.03f; tail += 0.03f; if (RLegs > 15.0f) step = true; }
            else { RLegs -= 0.03f; FLegs -= 0.03f; head -= 0.03f; tail -= 0.03f; if (RLegs < -15.0f) step = false; }
            float radRot = glm::radians(dogRot);
            dogPos.x += std::sin(radRot) * velocidad;
            dogPos.z += std::cos(radRot) * velocidad;
            distanciaCubierta += velocidad;
        }
        else {
            dogPos = glm::vec3(0.0f, 0.0f, 0.0f);
            dogRot = 0.0f;
            RLegs = FLegs = head = tail = 0.0f;
            step = false;


            dogAnim = 10;
            distanciaCubierta = 0.0f;
            distanciaObjetivo = distanciaCortaAdelante;
        }
    }
    // --- Estado 10: avance corto final hacia adelante ---
    else if (dogAnim == 10) {
        if (distanciaCubierta < distanciaObjetivo) {
            if (!step) { RLegs += 0.03f; FLegs += 0.03f; head += 0.03f; tail += 0.03f; if (RLegs > 15.0f) step = true; }
            else { RLegs -= 0.03f; FLegs -= 0.03f; head -= 0.03f; tail -= 0.03f; if (RLegs < -15.0f) step = false; }
            float radRot = glm::radians(dogRot);
            dogPos.x += std::sin(radRot) * velocidad;
            dogPos.z += std::cos(radRot) * velocidad;
            distanciaCubierta += velocidad;
        }
        else {
            // Fin de la secuencia: volver a Idle
            RLegs = FLegs = head = tail = 0.0f;
            step = false;
            dogAnim = 0;
            velocidadGiro = baseVelocidadGiro;
            lastTurnSign = +1;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = static_cast<GLfloat>(xPos);
        lastY = static_cast<GLfloat>(yPos);
        firstMouse = false;
    }

    GLfloat xOffset = static_cast<GLfloat>(xPos) - lastX;
    GLfloat yOffset = lastY - static_cast<GLfloat>(yPos);

    lastX = static_cast<GLfloat>(xPos);
    lastY = static_cast<GLfloat>(yPos);

    camera.ProcessMouseMovement(xOffset, yOffset);
}
