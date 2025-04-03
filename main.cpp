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
#include <cstdlib>
#include <iostream>

using namespace std;


// ADD GLOBAL VARIABLES HERE
GLFWwindow* window;


int main(int argc, char* argv[])
{
    // Initialize the resources - set window, etc.
    if (!glfwInit())
    {
        cout << "\nFailed to Initialize GLFW...";
        exit(EXIT_FAILURE);
    }


    // Create the display window
    window = glfwCreateWindow(640, 480, "Bowling Alley", NULL, NULL);


    //If window fails creation, then shut down the whole thing
    if (!window)
    {
        cout << "\nFailed to open display window...";
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    //If window succeeded then make it active on the display device
    glfwMakeContextCurrent(window);


    // ******  ABSOLUTELY required otherwise glGenVertexArrays will FAIL! *************
    glewExperimental = GL_TRUE;


    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        cout << "\nFailed to initialize GLEW...";
        exit(EXIT_FAILURE);
    }


    //==========================================================================//
    // At this stage... All systems are OK, proceed with window and view setup   //
    //==========================================================================//

    //Set the window's background colour (to RED)
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);


    //Keep displaying the window until we have shut it down
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);       // Flush the color buffer
        glfwSwapBuffers(window);            // Swap the front and back buffers
        glfwPollEvents();                   // Listen for other events
    }


    //Close the display window
    glfwDestroyWindow(window);

    //Release ALL the other resources
    glfwTerminate();

    // Shut down the program gracefully
    exit(EXIT_SUCCESS);
}