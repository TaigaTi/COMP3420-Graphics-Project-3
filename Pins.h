#pragma once

// This class will store a collection of Pins!
#include <vector>
#include "pin.h"

class Pins {
public:
    std::vector<Pin> pins; // Vector to store Pin objects

    // Constructor
    Pins() {}

    // Add a Pin to the collection
    void addPin(const Pin& pin) {
        pins.push_back(pin);
    }

    // Set Projection Matrix for All Pins
    void setProjection(glm::mat4 projection) {
        for (Pin& pin : pins) {
            pin.setProjection(projection);
        }
    }

    // Draw All Pins
    void draw(Camera camera) {
        for (Pin& pin : pins) {
            pin.draw(camera);
        }
    }

    // Initialize a Default Pin Layout (optional, to create a bowling formation)
    void initializeDefaultPins() {
        float baseX = 0.0f; // Base X position
        float baseZ = -580.0f; // Base Z position
        float spacing = 5.0f; // Spacing between pins
        float floor = -2.0f;

        glm::vec3 pinPositions[10] = {
            glm::vec3(0.0f, floor, -350.0f),  // First pin (front row, center)
            glm::vec3(-10.0f, floor, -360.0f), // Second pin (left of first)
            glm::vec3(10.0f, floor, -360.0f),  // Third pin (right of first)

            glm::vec3(-20.0f, floor, -380.0f), // Fourth pin (left of second)
            glm::vec3(0.0f, floor, -380.0f),   // Fifth pin (center, second row)
            glm::vec3(20.0f, floor, -380.0f),  // Sixth pin (right of second)

            glm::vec3(floor, floor, -400.0f), // Seventh pin (left of third)
            glm::vec3(-10.0f, floor, -400.0f), // Eighth pin (left of center)
            glm::vec3(10.0f, floor, -400.0f),  // Ninth pin (right of center)
            glm::vec3(30.0f, floor, -400.0f),  // Tenth pin (right of third)
        };


        // Create a triangular formation

        // Loop through all the pins and draw them
        for (int i = 0; i < 10; ++i) {
            pins.push_back(Pin(pinPositions[i])); // 1st row
        }
            //pinModel = glm::scale(pinModel, glm::vec3(23.0f)); // Scale the pin uniformly
    }
};