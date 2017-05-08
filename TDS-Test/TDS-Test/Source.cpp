#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "ResourceManager.h"
#include "Windows.h"

// Controller IDE
#include <Xinput.h>
#include <ctime>

// Settings
GLuint screenWidth = SCREEN_WIDTH;
GLuint screenHeight = SCREEN_HEIGHT;

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

Game TDS(screenWidth, screenHeight);

int main(int argc, char *argv[]) {
	srand(time(NULL));

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window;
#ifdef FULLSCREEN
	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	screenWidth = glfwGetVideoMode(primary)->width;
	screenHeight = glfwGetVideoMode(primary)->height;
	window = glfwCreateWindow(screenWidth, screenHeight, "TDS", primary, nullptr);
#else //FULLSCREEN
	window = glfwCreateWindow(screenWidth, screenHeight, "TDS", nullptr, nullptr);
#endif
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	glfwSetKeyCallback(window, key_callback);

	// OpenGL configuration
	glViewport(0, 0, screenWidth, screenHeight);
	TDS.Init();

	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;


	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//deltaTime = 0.001f;
		// Manage user input
		TDS.ProcessInput(deltaTime);

		// Update Game state
		TDS.Update(deltaTime);

		// Render
		glClearColor(1.0, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		TDS.Render();
		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			TDS.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			TDS.Keys[key] = GL_FALSE;
	}
}

