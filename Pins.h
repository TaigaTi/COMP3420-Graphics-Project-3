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

        // Create a triangular formation
        pins.push_back(Pin(glm::vec3(baseX, -2.0f, baseZ))); // 1st row
        pins.push_back(Pin(glm::vec3(baseX - spacing, -2.0f, baseZ - spacing)));
        pins.push_back(Pin(glm::vec3(baseX + spacing, -2.0f, baseZ - spacing))); // 2nd row
        pins.push_back(Pin(glm::vec3(baseX - 2 * spacing, -2.0f, baseZ - 2 * spacing)));
        pins.push_back(Pin(glm::vec3(baseX, -2.0f, baseZ - 2 * spacing)));
        pins.push_back(Pin(glm::vec3(baseX + 2 * spacing, -2.0f, baseZ - 2 * spacing))); // 3rd row
        pins.push_back(Pin(glm::vec3(baseX - 3 * spacing, -2.0f, baseZ - 3 * spacing)));
        pins.push_back(Pin(glm::vec3(baseX - spacing, -2.0f, baseZ - 3 * spacing)));
        pins.push_back(Pin(glm::vec3(baseX + spacing, -2.0f, baseZ - 3 * spacing)));
        pins.push_back(Pin(glm::vec3(baseX + 3 * spacing, -2.0f, baseZ - 3 * spacing))); // 4th row
    }
};