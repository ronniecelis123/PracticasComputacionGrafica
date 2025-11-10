// Practica 12
// Celis Hernández Ronie
// 9 de Noviembre del 2025
// 318143093

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

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

//Load Models
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

void LoadAnimationFromTXT(const std::string& filename);
void SaveAnimationToTXT(const std::string& filename);
void InitDefaultMuertito();
void resetElements();
void interpolation();
void saveFrame();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
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

//Anim
float rotBall = 0.0f;
float rotDog = 0.0f;
int   dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;

//KeyFrames controlados por animación
float dogPosX = 0.0f, dogPosY = 0.0f, dogPosZ = 0.0f;
float bodyRoll = 0.0f;   // para que el perro se tire de lado

#define MAX_FRAMES 9
int i_max_steps = 600;   // velocidad de transición entre keyframes
int i_curr_steps = 0;

typedef struct _frame {
    float rotDog;
    float rotDogInc;

    float dogPosX;
    float dogPosY;
    float dogPosZ;
    float incX;
    float incY;
    float incZ;

    float bodyRoll;
    float bodyRollInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;     // número de keyframes grabados
bool play = false;
int  playIndex = 0;

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// ================== TXT ==================

void LoadAnimationFromTXT(const std::string& filename)
{
    std::ifstream in(filename.c_str());
    if (!in.is_open()) {
        std::cout << "No se pudo abrir " << filename << ". Se iniciara sin anim guardada.\n";
        FrameIndex = 0;
        return;
    }

    int numFrames = 0;
    in >> i_max_steps;
    in >> numFrames;

    if (!in.good()) {
        std::cout << "Error leyendo " << filename << ".\n";
        FrameIndex = 0;
        return;
    }

    if (numFrames > MAX_FRAMES)
        numFrames = MAX_FRAMES;

    FrameIndex = numFrames;

    for (int i = 0; i < FrameIndex; ++i) {
        in >> KeyFrame[i].dogPosX
            >> KeyFrame[i].dogPosY
            >> KeyFrame[i].dogPosZ
            >> KeyFrame[i].rotDog
            >> KeyFrame[i].bodyRoll;

        KeyFrame[i].incX = KeyFrame[i].incY = KeyFrame[i].incZ = 0.0f;
        KeyFrame[i].rotDogInc = KeyFrame[i].bodyRollInc = 0.0f;
    }

    if (FrameIndex > 0) {
        dogPosX = KeyFrame[0].dogPosX;
        dogPosY = KeyFrame[0].dogPosY;
        dogPosZ = KeyFrame[0].dogPosZ;
        rotDog = KeyFrame[0].rotDog;
        bodyRoll = KeyFrame[0].bodyRoll;
    }

    std::cout << "Cargados " << FrameIndex << " keyframes desde "
        << filename << ", i_max_steps = " << i_max_steps << "\n";
}

void SaveAnimationToTXT(const std::string& filename)
{
    std::ofstream out(filename.c_str());
    if (!out.is_open()) {
        std::cout << "No se pudo guardar en " << filename << "\n";
        return;
    }

    out << i_max_steps << "\n";
    out << FrameIndex << "\n";

    for (int i = 0; i < FrameIndex; ++i) {
        out << KeyFrame[i].dogPosX << " "
            << KeyFrame[i].dogPosY << " "
            << KeyFrame[i].dogPosZ << " "
            << KeyFrame[i].rotDog << " "
            << KeyFrame[i].bodyRoll << "\n";
    }

    std::cout << "Animacion guardada en " << filename << "\n";
}

// Animación por defecto: perrito haciéndose el muertito
void InitDefaultMuertito()
{
    FrameIndex = 4;
    i_max_steps = 600;

    // Frame 0: de pie
    KeyFrame[0].dogPosX = 0.0f;
    KeyFrame[0].dogPosY = 0.0f;
    KeyFrame[0].dogPosZ = 0.0f;
    KeyFrame[0].rotDog = 0.0f;
    KeyFrame[0].bodyRoll = 0.0f;

    // Frame 1: cae al piso, de lado
    KeyFrame[1].dogPosX = 0.0f;
    KeyFrame[1].dogPosY = -0.18f;
    KeyFrame[1].dogPosZ = 0.0f;
    KeyFrame[1].rotDog = 0.0f;
    KeyFrame[1].bodyRoll = 90.0f;

    // Frame 2: se queda "muerto"
    KeyFrame[2].dogPosX = 0.0f;
    KeyFrame[2].dogPosY = -0.18f;
    KeyFrame[2].dogPosZ = 0.0f;
    KeyFrame[2].rotDog = 0.0f;
    KeyFrame[2].bodyRoll = 90.0f;

    // Frame 3: se levanta
    KeyFrame[3].dogPosX = 0.0f;
    KeyFrame[3].dogPosY = 0.0f;
    KeyFrame[3].dogPosZ = 0.0f;
    KeyFrame[3].rotDog = 0.0f;
    KeyFrame[3].bodyRoll = 0.0f;

    for (int i = 0; i < FrameIndex; ++i) {
        KeyFrame[i].incX = 0.0f;
        KeyFrame[i].incY = 0.0f;
        KeyFrame[i].incZ = 0.0f;
        KeyFrame[i].rotDogInc = 0.0f;
        KeyFrame[i].bodyRollInc = 0.0f;
    }

    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    rotDog = KeyFrame[0].rotDog;
    bodyRoll = KeyFrame[0].bodyRoll;

    std::cout << "Animacion por defecto (MUERTITO) creada.\n";
}

// ================== Keyframes ==================

void saveFrame(void)
{
    if (FrameIndex >= MAX_FRAMES) {
        std::cout << "MAX_FRAMES alcanzado\n";
        return;
    }

    std::cout << "Guardando keyframe " << FrameIndex << "\n";

    KeyFrame[FrameIndex].dogPosX = dogPosX;
    KeyFrame[FrameIndex].dogPosY = dogPosY;
    KeyFrame[FrameIndex].dogPosZ = dogPosZ;
    KeyFrame[FrameIndex].rotDog = rotDog;
    KeyFrame[FrameIndex].bodyRoll = bodyRoll;

    KeyFrame[FrameIndex].incX = 0.0f;
    KeyFrame[FrameIndex].incY = 0.0f;
    KeyFrame[FrameIndex].incZ = 0.0f;
    KeyFrame[FrameIndex].rotDogInc = 0.0f;
    KeyFrame[FrameIndex].bodyRollInc = 0.0f;

    FrameIndex++;

    // Persistente: cada vez que guardas un keyframe, se escribe al TXT
    SaveAnimationToTXT("dog_animation.txt");
}

void resetElements(void)
{
    if (FrameIndex <= 0) return;

    dogPosX = KeyFrame[0].dogPosX;
    dogPosY = KeyFrame[0].dogPosY;
    dogPosZ = KeyFrame[0].dogPosZ;
    rotDog = KeyFrame[0].rotDog;
    bodyRoll = KeyFrame[0].bodyRoll;
}

void interpolation(void)
{
    if (playIndex >= FrameIndex - 1) return;

    KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
    KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
    KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;

    KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;
    KeyFrame[playIndex].bodyRollInc = (KeyFrame[playIndex + 1].bodyRoll - KeyFrame[playIndex].bodyRoll) / i_max_steps;
}

// ================== main ==================

int main()
{
    // Init GLFW
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 12. Ronie Celis", nullptr, nullptr);

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

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
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

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");


    //models
    Model DogBody((char*)"Models/DogBody.obj");
    Model HeadDog((char*)"Models/HeadDog.obj");
    Model DogTail((char*)"Models/TailDog.obj");
    Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
    Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
    Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
    Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
    Model Piso((char*)"Models/piso.obj");
    Model Ball((char*)"Models/ball.obj");

    // Inicializa KeyFrame en 0
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        KeyFrame[i].dogPosX = 0;
        KeyFrame[i].dogPosY = 0;
        KeyFrame[i].dogPosZ = 0;
        KeyFrame[i].incX = 0;
        KeyFrame[i].incY = 0;
        KeyFrame[i].incZ = 0;
        KeyFrame[i].rotDog = 0;
        KeyFrame[i].rotDogInc = 0;
        KeyFrame[i].bodyRoll = 0;
        KeyFrame[i].bodyRollInc = 0;
    }

    // 1) Intentar cargar animación desde TXT
    LoadAnimationFromTXT("dog_animation.txt");

    // 2) Si no había nada guardado, crear muertito y guardarlo
    if (FrameIndex == 0)
    {
        InitDefaultMuertito();
        SaveAnimationToTXT("dog_animation.txt");
    }

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO, EBO;
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

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);


    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {

        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        DoMovement();
        Animation();

        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // OpenGL options
        glEnable(GL_DEPTH_TEST);


        glm::mat4 modelTemp = glm::mat4(1.0f); //Temp


        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


        // Point light 1
        glm::vec3 lightColor;
        lightColor.x = abs(sin(glfwGetTime() * Light1.x));
        lightColor.y = abs(sin(glfwGetTime() * Light1.y));
        lightColor.z = sin(glfwGetTime() * Light1.z);


        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glm::mat4 model(1);


        //Carga de modelo 
        view = camera.GetViewMatrix();
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Piso.Draw(lightingShader);

        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        //Body
        modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
        modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f)); // yaw
        modelTemp = model = glm::rotate(model, glm::radians(bodyRoll), glm::vec3(0.0f, 0.0f, 1.0f)); // roll (muertito)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogBody.Draw(lightingShader);

        //Head
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
        model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        HeadDog.Draw(lightingShader);

        //Tail 
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
        model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        DogTail.Draw(lightingShader);

        //Front Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_LeftLeg.Draw(lightingShader);

        //Front Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
        model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        F_RightLeg.Draw(lightingShader);

        //Back Left Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_LeftLeg.Draw(lightingShader);

        //Back Right Leg
        model = modelTemp;
        model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
        model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        B_RightLeg.Draw(lightingShader);


        model = glm::mat4(1);
        glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
        model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Ball.Draw(lightingShader);
        glDisable(GL_BLEND);  //Desactiva el canal alfa 
        glBindVertexArray(0);


        // Also draw the lamp object, again binding the appropriate shader
        lampShader.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        model = glm::mat4(1);
        model = glm::translate(model, pointLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);


        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// ================== Movimiento y animación ==================

// Moves/alters the camera positions based on user input
void DoMovement()
{
    //Dog Controls (solo afectan al frame actual, y si presionas K se guarda)
    if (keys[GLFW_KEY_2]) rotDog += 1.0f;
    if (keys[GLFW_KEY_3]) rotDog -= 1.0f;
    if (keys[GLFW_KEY_H]) dogPosZ += 0.01f;
    if (keys[GLFW_KEY_Y]) dogPosZ -= 0.01f;
    if (keys[GLFW_KEY_G]) dogPosX -= 0.01f;
    if (keys[GLFW_KEY_J]) dogPosX += 0.01f;

    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (keys[GLFW_KEY_T]) pointLightPositions[0].x += 0.01f;
    if (keys[GLFW_KEY_G]) pointLightPositions[0].x -= 0.01f;
    if (keys[GLFW_KEY_Y]) pointLightPositions[0].y += 0.01f;
    if (keys[GLFW_KEY_H]) pointLightPositions[0].y -= 0.01f;
    if (keys[GLFW_KEY_U]) pointLightPositions[0].z -= 0.1f;
    if (keys[GLFW_KEY_J]) pointLightPositions[0].z += 0.01f;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // L: reproduce / pausa la animación
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        if (!play && (FrameIndex > 1))
        {
            resetElements();
            interpolation();
            play = true;
            playIndex = 0;
            i_curr_steps = 0;
            std::cout << "Reproduciendo animacion...\n";
        }
        else
        {
            play = false;
            std::cout << "Animacion detenida.\n";
        }
    }

    // K: guarda keyframe actual (y persiste al TXT)
    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        if (FrameIndex < MAX_FRAMES)
        {
            saveFrame();
        }
    }

    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_SPACE])
    {
        active = !active;
        if (active)
        {
            Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

        }
        else
        {
            Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
        }
    }
}

void Animation() {

    if (!play) return;

    if (i_curr_steps >= i_max_steps) //end of animation between frames?
    {
        playIndex++;
        if (playIndex > FrameIndex - 2) //end of total animation?
        {
            printf("termina anim\n");
            playIndex = 0;
            play = false;
        }
        else //Next frame interpolations
        {
            i_curr_steps = 0; //Reset counter
            //Interpolation
            interpolation();
        }
    }
    else
    {
        //Draw animation
        dogPosX += KeyFrame[playIndex].incX;
        dogPosY += KeyFrame[playIndex].incY;
        dogPosZ += KeyFrame[playIndex].incZ;
        rotDog += KeyFrame[playIndex].rotDogInc;
        bodyRoll += KeyFrame[playIndex].bodyRollInc;

        i_curr_steps++;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
