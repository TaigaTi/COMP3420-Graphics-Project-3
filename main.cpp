/*==============================================================================
	Project 3 - Group 4
		Tarika Birch
		Isaiah Carrington
		Janae Fitzgerald
		Kiara Fredrick
		Jonathan King
=================================================================================*/


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <iostream>

#include "camera.h"
#include "shader.h"
#include "model.h"

using namespace std;


// Display Window
GLFWwindow* window;
GLuint sWidth = 1920, sHeight = 1080;
//GLfloat aspectRatio = sWidth / sHeight;

// Camera
Camera camera(glm::vec3(0.0f, 200.0f, 1700.0f));

// Ball Angle
GLfloat ballAngle = 0.0;

void init() {
	// Initialize the resources - set window, etc.
	if (!glfwInit())
	{
		cout << "\nFailed to Initialize GLFW...";
		exit(EXIT_FAILURE);
	}


	// Create the display window
	window = glfwCreateWindow(sWidth, sHeight, "Bowling Alley", NULL, NULL);


	// If window fails creation, then shut down the whole thing
	if (!window)
	{
		cout << "\nFailed to open display window...";
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// If window succeeded then make it active on the display device
	glfwMakeContextCurrent(window);


	// ******  ABSOLUTELY required otherwise glGenVertexArrays will FAIL! *************
	glewExperimental = GL_TRUE;


	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		cout << "\nFailed to initialize GLEW...";
		exit(EXIT_FAILURE);
	}

	// Depth Test
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[])
{
	// Initialize resources
	init();

	// Setup and compile bowling ball shader
	Shader ballShader("vertexShader.glsl", "fragmentShader.glsl");

	// Load the bowling ball model object
	Model bowlingBall((GLchar*)"bowling_ball.obj");

	// Set the color of the window
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)sWidth / (GLfloat)sHeight, 1.0f, 10000.0f);
	ballShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(ballShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	// Keep displaying the window until we have shut it down
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Flush the color buffer
		glfwPollEvents();											// Listen for other events
	
		// Create the view matrix
		ballShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(ballShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		// Create model matrix
		ballShader.Use();
		glm::mat4 ballModel = glm::mat4(1);

		ballModel = glm::scale(ballModel, glm::vec3(20.0f));
		ballModel = glm::translate(ballModel, glm::vec3(0.0f, 0.0f, 0.0f));

		ballAngle += 0.001;
		ballModel = glm::rotate(ballModel, ballAngle, glm::vec3(1.0f, 1.0f, 1.0f));

		
		// Pass the model matrix to the shader as "model"
		glUniformMatrix4fv(glGetUniformLocation(ballShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ballModel));
		glUniform1i(glGetUniformLocation(ballShader.Program, "texture_diffuse1"), 0);
	
		// Draw the object
		bowlingBall.Draw(ballShader);

		// Swap the front and back buffers
		glfwSwapBuffers(window);            
	}


	// Close the display window
	glfwDestroyWindow(window);

	// Release ALL the other resources
	glfwTerminate();

	// Shut down the program gracefully
	exit(EXIT_SUCCESS);
}