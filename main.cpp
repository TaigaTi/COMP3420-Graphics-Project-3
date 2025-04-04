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

using namespace std;


// Display Window
GLFWwindow* window;
GLuint sWidth = 1000, sHeight = 800;

// Camera
Camera camera(glm::vec3(0, 0, 0));

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
}

int main(int argc, char* argv[])
{
    // Initialize resources
    init();
    

    // Keep displaying the window until we have shut it down
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);       // Flush the color buffer
        glfwSwapBuffers(window);            // Swap the front and back buffers
        glfwPollEvents();                   // Listen for other events
    }


    // Close the display window
    glfwDestroyWindow(window);

    // Release ALL the other resources
    glfwTerminate();

    // Shut down the program gracefully
    exit(EXIT_SUCCESS);
}