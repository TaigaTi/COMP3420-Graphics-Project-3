/*==============================================================================
	Project 3 - Group 4
		Tarika Birch
		Isaiah Carrington
		Janae Fitzgerald
		Kiara Fredrick
		Jonathan King

	Resource Citations
	- Background Music: Pajama Party by Jeremy Korpas - YouTube Audio Library
	- Pins Falling Sound: SieuAmThanh (Pixabay) - https://pixabay.com/sound-effects/bowling-strike-40456/
	- Pin Object: Trevor Powers (TurboSquid) - https://www.turbosquid.com/3d-models/bowling-pins-c4d-free/1114818
	- Platform Textures: Tarika Birch
	- Skybox Textures: Tarika Birch
	- Ball Texture: Pramote Lertnitivanit (Vecteezy) - https://www.vecteezy.com/vector-art/3127954-abstract-template-blue-background-white-squares
	- Platform Object: Isaiah Carrington
	- Ball Object: Isaiah Carrington

	Packages
	- Install-Package glew_dynamic
	- Install-Package glfw
	- Install-Package GLMathematics
	- Install-Package SFML-Audio
	- Install-Package Soil
	- Install-Package Assimp -version 3.0.0

	Controls
	Move Ball: Left and Right Arrows
	Launch Ball: Up Arrow
	Move Camera: CTRL & Left and CTRL & Right
	Move Camera & Ball:  SHIFT & Left and SHIFT & Right
	Look Around:  Right Click & Drag
	Music Volume: CTRL & + or CTRL & -

=================================================================================*/


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <iostream>
#include <SFML/Audio.hpp>
#include <algorithm>

#include "camera.h"
#include "shader.h"
#include "model.h"
#include "ball.h"
#include "pin.h"
#include "pins.h"
#include "boundary.h"
#include "sound.h"
#include "text_writer.h"

using namespace std;

void processInput(GLFWwindow*);
bool checkForCollisions(Ball*, Pins*);
void renderAllText(TextWriter);

// Display Window
GLFWwindow* window;
GLuint sWidth = 1920, sHeight = 1080;
GLfloat aspectRatio = (float)sWidth / (float)sHeight;

// Function Prototypes
GLuint loadCubeMap(vector<std::string>);

// Camera
Camera camera(glm::vec3(0.0f, 1000.0f, 1500.0f)); 

// SCORE!

GLuint score = 0;

// Movement Flags and Speed
bool isRightMouseDown = false;
bool keys[1024];
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float movementSpeed = 750.0f; // Adjust this for faster/slower movement

// Music Volume

float musicVolume = 50.0f;
bool musicCanChange = true;

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


// ball and camera movement
void do_movement(Ball* ball)
{
	if (keys[GLFW_KEY_UP]) {
		ball->isRolling = true;
	}

	if (keys[GLFW_KEY_LEFT_SHIFT] && keys[GLFW_KEY_RIGHT] && ball->isRolling == false) {
		ball->move(1, 0, deltaTime);
		camera.ProcessKeyboard(RIGHT, deltaTime * movementSpeed);
	}
	if (keys[GLFW_KEY_LEFT_SHIFT] && keys[GLFW_KEY_LEFT] && ball->isRolling == false) {
		ball->move(-1, 0, deltaTime);
		camera.ProcessKeyboard(LEFT, deltaTime * movementSpeed);
	}

	if (keys[GLFW_KEY_LEFT] && !keys[GLFW_KEY_LEFT_CONTROL] && !keys[GLFW_KEY_LEFT_SHIFT] && ball->isRolling == false) {
		ball->move(-1, 0, deltaTime);
	}
	if (keys[GLFW_KEY_RIGHT] && !keys[GLFW_KEY_LEFT_CONTROL] && !keys[GLFW_KEY_LEFT_SHIFT] && ball->isRolling == false) {
		ball->move(1, 0, deltaTime);
	}
	if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_LEFT] /*&& ball->isRolling == false*/)
		camera.ProcessKeyboard(LEFT, deltaTime * movementSpeed);
	if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_RIGHT] /*&& ball->isRolling == false*/)
		camera.ProcessKeyboard(RIGHT, deltaTime * movementSpeed);

	if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_MINUS])
		musicVolume = std::max(0.0f, musicVolume - (20.0f * deltaTime));
		musicCanChange = false;
	if (keys[GLFW_KEY_LEFT_CONTROL] && keys[GLFW_KEY_EQUAL])
		musicVolume = std::min(100.0f, musicVolume + (20.0f * deltaTime));
		musicCanChange = false;
}

bool firstMouse = true;
float lastX = sWidth / 2.0f;
float lastY = sHeight / 2.0f;

//camera view callback
void camera_view_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (!isRightMouseDown)
	{
		return;
	}

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

	float sensitivity = 0.2f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.ProcessMouseMovement(xoffset, yoffset);
};

// Right click to allow camera movement
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		isRightMouseDown = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
		isRightMouseDown = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

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

	// OpenGL state
	// ------------
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Depth Test
	glEnable(GL_DEPTH_TEST);

	// Set viewport size
	glViewport(0, 0, sWidth, sHeight);

}

int main(int argc, char* argv[])
{
	// Initialize resources
	init();

	Ball ball = Ball();
	// Text Writer!

	TextWriter textWriter = TextWriter();

	glm::vec3 cameraOffset = camera.Position - ball.position;
	//Camera movement via mouse callback set
	glfwSetCursorPosCallback(window, camera_view_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
	

	// Setup and compile shaders
	Shader skyboxShader("skyboxVertexShader.glsl", "skyboxFragmentShader.glsl");
	Shader cubeShader("cubeMapVertexShader.glsl", "cubeMapFragmentShader.glsl");
	Shader platformShader("platformVertexShader.glsl", "platformFragmentShader.glsl");

	// Load the platform model object
	Model platform((GLchar*)"platform.obj");

	Pins pins = Pins();

	pins.initializeDefaultPins(0.0, 0.0, 0.0);
	pins.initializeDefaultPins(175.0, 0.0, 0.0);
	pins.initializeDefaultPins(-175.0, 0.0, 0.0);

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

	sf::Music music;
	music.openFromFile("music.ogg");

	music.setLoop(true);
	music.setVolume(musicVolume);

	music.play();


	/*sf::Sound sound;
	sf::SoundBuffer buffer;

	buffer.loadFromFile("strike_sound.wav");

	sound.setBuffer(buffer);
	sound.setVolume(100.0f);
	sound.setPosition(0, 0, 0);
	sound.play();*/

	//Sound sound = Sound("strike_sound.wav", 100.0f, glm::vec3(0));
	//sound.play();

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
		if (ball.boundsCheck(boundary)) {
			score = 0;
		}

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

		// Optional Volume Updates:

		if (musicVolume != music.getVolume()) {
			music.setVolume(musicVolume);
			musicCanChange = true;
		}

		// Draw text
		renderAllText(textWriter);

		// Reset the depth function back to its default
		glDepthFunc(GL_LESS);

		// Listen for other events (Should be done at the end of the frame please.)
		glfwPollEvents();

		// Swap the front and back buffers
		glfwSwapBuffers(window);
	}

	music.stop();

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
			glm::vec3 direction = pin.position - ball->position;
			direction.y = 0.0f; // Ignore vertical component
			if (glm::length(direction) != 0.0f) {
				direction = glm::normalize(direction);
			}

			// Apply directional fall
			pin.launch(direction);
			collisionOccurred = true;

			if (pin.canCollide) {
				score += 1;
				pin.canCollide = false;
			}
		}
	}

	return collisionOccurred;
}

void renderAllText(TextWriter textWriter) {

	// Options
	textWriter.drawText("Score: " + std::to_string(score), glm::vec2(10.0, sHeight - 40.0f));
	textWriter.drawText("Current Volume: " + std::to_string((int)musicVolume), glm::vec2(10.0f, 10.0f), 0.5);

	// Do instructions
	float offset;

	offset = 0.01f;
	std::string instructions[] = {
		"Use left and right arrow keys to move the ball.",
		"Use Up arrow to BOWL!",
		"Left Ctrl and left and right arrow keys moves the camera only.",
		"Using Shift and Arrow keys moves both the ball and camera together.",
		"You can use right click to look around.",
		"Just be careful when moving camera after looking around!",
		"Use ctrl and + and - to raise and lower the volume.",
		"Have fun!"
	};

	for (std::string instruction : instructions) {
		textWriter.drawText(instruction, glm::vec2(sWidth * 0.76, sHeight * (0.98 - offset)), 0.3f);
		offset += 0.02f;
	}

	// Do Credits

	std::string names[] = { "Tarika Birch", "Isaiah Carrington", "Janae Fitzgerald", "Kiara Fredrick", "Jonathan King" };
	offset = 0.01f;
	for (std::string name : names) {
		textWriter.drawText(name, glm::vec2(sWidth * 0.92, sHeight * (0.1 - offset) ), 0.4f);
		offset += 0.02f;
	}
}