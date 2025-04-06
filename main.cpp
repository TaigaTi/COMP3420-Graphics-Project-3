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

void processInput(GLFWwindow* window);


// Display Window
GLFWwindow* window;
GLuint sWidth = 1920, sHeight = 1080;
GLfloat aspectRatio = (float)sWidth / (float)sHeight;

// Function Prototypes
GLuint loadCubeMap(vector<std::string>);

// Camera
Camera camera(glm::vec3(0.0f, 200.0f, 1000.0f));
// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly

bool firstMouse = true;
float lastX = sWidth / 2.0f;
float lastY = sHeight / 2.0f;

void camera_view_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
};


// Ball Angle
GLfloat ballAngle = 0.0;

void init() {
	// Initialize the resources - set window, etc.
	if (!glfwInit())
	{
		cout << "\nFailed to Initialize GLFW...";
		exit(EXIT_FAILURE);
	}

	// Set the configuration info
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create the display window
	window = glfwCreateWindow(sWidth, sHeight, "Bowling skybox", NULL, NULL);


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

	// Set viewport size
	// This makes the bottom left be 0, 0
	glViewport(0, 0, sWidth, sHeight);
}

int main(int argc, char* argv[])
{
	// Initialize resources
	init();

	//Camera movement via mouse callback set
	glfwSetCursorPosCallback(window, camera_view_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Setup and compile shaders
	Shader ballShader("ballVertexShader.glsl", "ballFragmentShader.glsl");
	Shader skyboxShader("skyboxVertexShader.glsl", "skyboxFragmentShader.glsl");
	Shader cubeShader("cubeMapVertexShader.glsl", "cubeMapFragmentShader.glsl");
	Shader bowlingPinsShader("bowlingPinsVertexShader.glsl", "bowlingPinsFragmentShader.glsl");
	Shader platformShader("platformVertexShader.glsl", "platformFragmentShader.glsl");

	// Load the bowling ball model object
	Model bowlingBall((GLchar*)"bowling_ball.obj");
	Model platform((GLchar*)"platform.obj");
	Model bowlingPins((GLchar*)"High Poly.obj");

	// Define bowling skybox vertices
	GLfloat skyboxVertices[] =
	{
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// =======================================================================
	//  Setup skybox VAO
	// =======================================================================
	GLuint skyboxVAO, skyboxVBO;

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);

	// Gather images for cube map for the skybox and load them in
	std::vector<std::string> faces;
	faces.push_back("right.jpg");
	faces.push_back("left.jpg");
	faces.push_back("top.jpg");
	faces.push_back("bottom.jpg");
	faces.push_back("back.jpg");
	faces.push_back("front.jpg");

	GLuint skyboxTexture = loadCubeMap(faces);


	// =======================================================================
	//  Create the projection matrix
	// =======================================================================
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)sWidth / (GLfloat)sHeight, 0.1f, 10000.0f);
	ballShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(ballShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	platformShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(platformShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	// Keep displaying the window until we have shut it down
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);										// Input Processing
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Flush the color buffer
	
		// =======================================================================
		//  Setup the scene
		// =======================================================================
		cubeShader.Use();
		glm::mat4 model = glm::mat4(1.0f); // Start with clean matrix
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1000.0f));
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

		glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "model"),
			1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "view"),
			1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));

		glUniform3f(glGetUniformLocation(cubeShader.Program, "cameraPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		// Create the view matrices
		ballShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(ballShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		platformShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(platformShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		// =======================================================================
		//  Create ball model matrix
		// =======================================================================
		ballShader.Use();
		glm::mat4 ballModel = glm::mat4(1);

		ballModel = glm::scale(ballModel, glm::vec3(10.0f));
		ballModel = glm::translate(ballModel, glm::vec3(0.0f, 0.0f, 0.0f));

		// Pass the ball model matrix to the shader as "model"
		glUniformMatrix4fv(glGetUniformLocation(ballShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(ballModel));
		glUniform1i(glGetUniformLocation(ballShader.Program, "texture_diffuse1"), 0);

		// Draw the ball object
		bowlingBall.Draw(ballShader);

		// Draw bowling pins
		bowlingPinsShader.Use();

		// Set view & projection matrices for the pins
		glUniformMatrix4fv(glGetUniformLocation(bowlingPinsShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(bowlingPinsShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Create a model matrix for the pins
		glm::mat4 pinsModel = glm::mat4(1.0f);
		pinsModel = glm::scale(pinsModel, glm::vec3(50.0f));
		pinsModel = glm::translate(pinsModel, glm::vec3(0.0f, 0.0f, 5.0f)); // adjust Z so it’s behind the ball

		glUniformMatrix4fv(glGetUniformLocation(bowlingPinsShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(pinsModel));
		glUniform1i(glGetUniformLocation(bowlingPinsShader.Program, "texture_diffuse1"), 0);

		// Draw pins
		bowlingPins.Draw(bowlingPinsShader);


		// Setup the skybox with its matrices
		// =======================================================================
		//  Create platform model matrix
		// =======================================================================
		platformShader.Use();
		glm::mat4 platformModel = glm::mat4(1);

		platformModel = glm::scale(platformModel, glm::vec3(300.0f));
		platformModel = glm::translate(platformModel, glm::vec3(0.0f, -10.0f, -10.0f));

		// Pass the platform model matrix to the shader as "model"
		glUniformMatrix4fv(glGetUniformLocation(platformShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(platformModel));
		glUniform1i(glGetUniformLocation(platformShader.Program, "texture_diffuse1"), 0);

		// Draw the platorm object
		platform.Draw(platformShader);

		// Zoom the camera
		camera.Position = glm::vec3(camera.Position.x, camera.Position.y, camera.Position.z + 1);

		// =======================================================================
		//  Setup the skybox with its matrices
		// =======================================================================
		// Change the depth function so depth test passes when values are equal to depth buffer's content
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();

		// Reset the view matrix to accommodate camera movements
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		// Pass view and projection matrices to the shaders
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"),
			1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"),
			1, GL_FALSE, glm::value_ptr(projection));

		// Draw the skybox cube map
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// Reset the depth function back to its default
		glDepthFunc(GL_LESS);

		// Listen for other events (Should be done at the end of the frame please.)
		glfwPollEvents();

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

// Loads a cubemap texture from 6 individual texture faces
GLuint loadCubeMap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void processInput(GLFWwindow* window)
{
	// We can handle input in here
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}