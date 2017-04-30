// OpenGl Imports
#define GLEW_STATIC // GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <SOIL/SOIL.h>

// Other Imports
#include <iostream>
#include <vector>
#include "Shader.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Game vatiables
std::vector<float> pos{ 0.0f, 0.0f };
float mixValue = 0.5;


// The MAIN function, from here we start the application and run the game loop
int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader myShader("shader.vs", "shader.frag");

	// Adding vertex data
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left 
	};

	GLuint indices[] = {
		0, 1, 3, // First Triangle
		1, 2, 3 // Second Triangle
	};

	// Creating vertex Buffers
	GLuint VBOs[1], VAOs[1], EBOs[1], Ts[2];

	// VertexArray;
	glGenVertexArrays(1, VAOs);
	glGenBuffers(1, EBOs);
	glGenBuffers(1, VBOs); // generates buffers
	glBindVertexArray(VAOs[0]); // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // Binds buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	// Specifying vertexAttribPointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat))); // Texture Coordinates
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Importing Textures
	glGenTextures(2, Ts); // generating textures
	int imgWidth, imgHeight;
	unsigned char* myImage;

	glBindTexture(GL_TEXTURE_2D, Ts[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	myImage = SOIL_load_image("Textures\\Test_IMG_container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, myImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(myImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, Ts[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	myImage = SOIL_load_image("Textures\\awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, myImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(myImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.1796875f, 0.5078125f, 0.796875f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Update
		glUniform1f(glGetUniformLocation(myShader.Program, "mixValue"), mixValue);
		glUniform2f(glGetUniformLocation(myShader.Program, "positionOffset"), pos[0], pos[1]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Ts[0]);
		glUniform1i(glGetUniformLocation(myShader.Program, "texture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Ts[1]);
		glUniform1i(glGetUniformLocation(myShader.Program, "texture2"), 1);


		// Activates Shaders
		myShader.Use();
		glUniform3f(glGetUniformLocation(myShader.Program, "positionOffset"), 0.0f, 0.0f, 0.0f);

		// Draw first triangle
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, EBOs);
	glDeleteBuffers(1, VBOs);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_UP: if (mixValue < 1) { mixValue += 0.1; }
						  break;
		case GLFW_KEY_DOWN: if (mixValue > 0) { mixValue -= 0.1; }
							break;
		case GLFW_KEY_W: pos[1] += 0.05;
			break;
		case GLFW_KEY_S: pos[1] -= 0.05;
			break;
		case GLFW_KEY_A: pos[0] -= 0.05;
			break;
		case GLFW_KEY_D: pos[0] += 0.05;
			break;
		}
	}
}