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
#include "ball.h"
#include "pin.h"
#include "pins.h"
#include "boundary.h"

using namespace std;

void processInput(GLFWwindow* window);
bool checkForCollisions(Ball*, Pins*);

// Display Window
GLFWwindow* window;
GLuint sWidth = 1920, sHeight = 1080;
GLfloat aspectRatio = (float)sWidth / (float)sHeight;

// Function Prototypes
GLuint loadCubeMap(vector<std::string>);

// Camera
Camera camera(glm::vec3(0.0f, 1000.0f, 1500.0f)); 

// Ball Properties
const float BALL_SCALE = 25.0f;
const float BALL_BASE_SPEED = 29.0f;
bool isRolling = false;
glm::vec3 ballPosition = glm::vec3(0.0f, 0.0f, 200.0f);

// Movement Flags and Speed
bool keys[1024];
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float movementSpeed = 220.0f; // Adjust this for faster/slower movement

// Keyboard callback to track key presses
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Register key presses
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement(Ball* ball)
{
	if (keys[GLFW_KEY_UP]) {
		ball->isRolling = true;
	}
	if (keys[GLFW_KEY_DOWN]) {
		ball->isRolling = false;
	}
	if (keys[GLFW_KEY_LEFT]) {
		ball->move(-1, 0, deltaTime);
	}
	if (keys[GLFW_KEY_RIGHT]) {
		ball->move(1, 0, deltaTime);
	}
	// Camera movement
	if (keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime * movementSpeed);
	if (keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime * movementSpeed);
	if (keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime * movementSpeed);
	if (keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime * movementSpeed);
}

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

	// Set viewport size
	glViewport(0, 0, sWidth, sHeight);
}

int main(int argc, char* argv[])
{
	// Initialize resources
	init();


	glm::vec3 cameraOffset = camera.Position - ballPosition;
	//Camera movement via mouse callback set
	glfwSetCursorPosCallback(window, camera_view_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);

	// Setup and compile shaders
	Shader skyboxShader("skyboxVertexShader.glsl", "skyboxFragmentShader.glsl");
	Shader cubeShader("cubeMapVertexShader.glsl", "cubeMapFragmentShader.glsl");
	Shader platformShader("platformVertexShader.glsl", "platformFragmentShader.glsl");

	// Load the platform model object
	Model platform((GLchar*)"platform.obj");

	Ball ball = Ball();
	Pins pins = Pins();

	pins.initializeDefaultPins();

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
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)sWidth / (GLfloat)sHeight, 0.1f, 50000.0f);
	ball.setProjection(projection);
	platformShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(platformShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	pins.setProjection(projection);


	// Keep displaying the window until we have shut it down
	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// Flush the color buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		do_movement(&ball); // Input Processing
	
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

		// =======================================================================
		//  Create view matrices
		// =======================================================================
		platformShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(platformShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));


		// =======================================================================
		//  Draw the bowling ball and pins
		// =======================================================================
		if (ball.isRolling == true) {
			ball.move(0, -1, deltaTime);
		}

		ball.draw(camera);
		pins.draw(camera);


		// =======================================================================
		//  Create platform model matrix
		// =======================================================================
		platformShader.Use();
		glm::mat4 platformModel = glm::mat4(1);

		float platformScale = 500.0f;
		float platformOffset = 15.0f;

		float backwall = -(platformScale + platformOffset);
		float side = platformScale / 2.0f;

		Boundary boundary = Boundary(-side, side - ball.SCALE, 0, backwall);

		platformModel = glm::scale(platformModel, glm::vec3(platformScale));
		platformModel = glm::translate(platformModel, glm::vec3(0.0f, 0.0f, platformOffset));


		// Pass the platform model matrix to the shader as "model"
		glUniformMatrix4fv(glGetUniformLocation(platformShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(platformModel));
		glUniform1i(glGetUniformLocation(platformShader.Program, "texture_diffuse1"), 0);

		// Draw the platform object
		platform.Draw(platformShader);

		// =======================================================================
		//  Check for collisions
		// =======================================================================
		checkForCollisions(&ball, &pins);
		ball.boundsCheck(boundary);

		// Reset the pins
		pins.fall(deltaTime);

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

// Processes mouse and keyboard input
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Checks for collisions between the ball and pins
bool checkForCollisions(Ball* ball, Pins* pins) {
	bool collisionOccurred = false;

	for (Pin& pin : pins->pins) {
		if (ball->checkCollision(pin.rect) == true) {
			pin.move(glm::vec3(0, 50, 0));
			return true;
		}
	}

	return false;
}