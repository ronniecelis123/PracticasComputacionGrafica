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

// Other includes
#include "Shader.h"
#include "Camera.h"


//Practica 07
//Celis Hernández Ronie
//28 de septiembre del 2025
//318143093

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

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
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// The MAIN function, from here we start the application and run the game loop
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 7. Ronie Celis", nullptr, nullptr);

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// === INFO DEL CONTEXTO OPENGL (pegar después de glewInit OK) ===
	{
		auto safe_cstr = [](const GLubyte* s) -> const char* {
			return s ? reinterpret_cast<const char*>(s) : "(null)";
			};

		auto print_gl_str = [&](GLenum pname, const char* label) {
			const GLubyte* s = glGetString(pname);
			std::cout << label << safe_cstr(s) << '\n';
			};

		// Requiere: contexto activo (glfwMakeContextCurrent) + GLEW inicializado correctamente.
		print_gl_str(GL_VERSION, "> Version: ");
		print_gl_str(GL_VENDOR, "> Vendor: ");
		print_gl_str(GL_RENDERER, "> Renderer: ");
		print_gl_str(GL_SHADING_LANGUAGE_VERSION, "> SL Version: ");
	}
	// === FIN INFO DEL CONTEXTO ===

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// cara 1  (tope: Y = +1)  (f 1/1/1 5/2/1 7/3/1 3/4/1)
		 1.0f,  1.0f, -1.0f,  1,1,1,   0.499217f,0.626171f, // v1 vt1
		-1.0f,  1.0f, -1.0f,  1,1,1,   0.499990f,0.876000f, // v5 vt2
		-1.0f,  1.0f,  1.0f,  1,1,1,   0.313470f,0.876030f, // v7 vt3
		 1.0f,  1.0f,  1.0f,  1,1,1,   0.312697f,0.626202f, // v3 vt4


		 // cara 2  (f 4/5/2 3/4/2 7/6/2 8/7/2)
		  1.0f, -1.0f,  1.0f,  1,1,1,   0.311924f,0.376373f, // v4 vt5
		  1.0f,  1.0f,  1.0f,  1,1,1,   0.312697f,0.626202f, // v3 vt4
		 -1.0f,  1.0f,  1.0f,  1,1,1,   0.133118f,0.626318f, // v7 vt6
		 -1.0f, -1.0f,  1.0f,  1,1,1,   0.132345f,0.376489f, // v8 vt7

		 // cara 3  (f 8/8/3 7/9/3 5/10/3 6/11/3)
		 -1.0f, -1.0f,  1.0f,  1,1,1,   0.878424f,0.376368f, // v8 vt8
		 -1.0f,  1.0f,  1.0f,  1,1,1,   0.879197f,0.626197f, // v7 vt9
		 -1.0f,  1.0f, -1.0f,  1,1,1,   0.692678f,0.626227f, // v5 vt10
		 -1.0f, -1.0f, -1.0f,  1,1,1,   0.691904f,0.376399f, // v6 vt11

		 // cara 4  (f 6/12/4 2/13/4 4/5/4 8/14/4)
		 -1.0f, -1.0f, -1.0f,  1,1,1,   0.497670f,0.126515f, // v6 vt12
		  1.0f, -1.0f, -1.0f,  1,1,1,   0.498444f,0.376343f, // v2 vt13
		  1.0f, -1.0f,  1.0f,  1,1,1,   0.311924f,0.376373f, // v4 vt5
		 -1.0f, -1.0f,  1.0f,  1,1,1,   0.311150f,0.126545f, // v8 vt14

		 // cara 5  (f 2/13/5 1/1/5 3/4/5 4/5/5)
		  1.0f, -1.0f, -1.0f,  1,1,1,   0.498444f,0.376343f, // v2 vt13
		  1.0f,  1.0f, -1.0f,  1,1,1,   0.499217f,0.626171f, // v1 vt1
		  1.0f,  1.0f,  1.0f,  1,1,1,   0.312697f,0.626202f, // v3 vt4
		  1.0f, -1.0f,  1.0f,  1,1,1,   0.311924f,0.376373f, // v4 vt5

		  // cara 6  (f 6/11/6 5/10/6 1/1/6 2/13/6)
		  -1.0f, -1.0f, -1.0f,  1,1,1,   0.691904f,0.376399f, // v6 vt11
		  -1.0f,  1.0f, -1.0f,  1,1,1,   0.692678f,0.626227f, // v5 vt10
		   1.0f,  1.0f, -1.0f,  1,1,1,   0.499217f,0.626171f, // v1 vt1
		   1.0f, -1.0f, -1.0f,  1,1,1,   0.498444f,0.376343f, // v2 vt13
	};


	GLuint indices[] = {
		0,1,2,  0,2,3,        // cara 1
		4,5,6,  4,6,7,        // cara 2
		8,9,10, 8,10,11,      // cara 3
		12,13,14, 12,14,15,   // cara 4
		16,17,18, 16,18,19,   // cara 5
		20,21,22, 20,22,23    // cara 6
	};

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Load textures
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// Diffuse map
	image = stbi_load("images/cubo.jpeg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
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

		// Clear the colorbuffer
		glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // azul claro (cielo)

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lampShader.Use();
		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 model(1);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lampShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lampShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
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
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
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
