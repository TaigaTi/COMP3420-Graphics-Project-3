#pragma once

// This class will store a collection of Pins!
#include <vector>
#include "pin.h"

class Pins {
public:
    const static int PIN_COUNT = 10;
    std::vector<Pin> pins; // Vector to store Pin objects

    // Constructor
    Pins() {}

    // Add a Pin to the collection
    /*void addPin(const Pin& pin) {
        pins.push_back(pin);
    }*/

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

    // Make all pins stay on the ground!

    void fall(float deltaTime) {
        for (Pin& pin : pins) {
            pin.fall(deltaTime);
        }
    }

    // Initialize a Default Pin Layout (optional, to create a bowling formation)
    void initializeDefaultPins(float xOffset, float yOffset, float zOffset) {
        float centerX = 0.0f + xOffset; // Base X position
        float centerZ = -380.0f + zOffset; // Base Z position
        float floor = -2.0f;

        float right1 = 10.0f;
        float right2 = 20.0f;
        float right3 = 30.0f;

        // Create a triangular formation
        glm::vec3 pinPositions[PIN_COUNT] = {
            glm::vec3(centerX, floor, centerZ + 20),  // First pin (front row, center)
            glm::vec3(centerX -right1, floor, centerZ + 20), // Second pin (left of first)
            glm::vec3(centerX + right1, floor, centerZ + 20),  // Third pin (right of first)

            glm::vec3(centerX -right2, floor, centerZ), // Fourth pin (left of second)
            glm::vec3(centerX, floor, centerZ),   // Fifth pin (center, second row)
            glm::vec3(centerX + right2, floor, centerZ),  // Sixth pin (right of second)

            glm::vec3(centerX -right3, floor, centerZ - 20), // Seventh pin (left of third)
            glm::vec3(centerX -right1, floor, centerZ - 20), // Eighth pin (left of center)
            glm::vec3(centerX + right1, floor, centerZ - 20),  // Ninth pin (right of center)
            glm::vec3(centerX + right3, floor, centerZ - 20),  // Tenth pin (right of third)
        };

        // Store the pins
        for (int i = 0; i < PIN_COUNT; ++i) {
            Pin pin = Pin();
            pin.setPosition(pinPositions[i]);
            pins.push_back(pin); // 1st row
        }
    }
};