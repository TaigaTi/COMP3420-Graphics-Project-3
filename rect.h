#pragma once
// This class will represent a rectangular collision box.

#include <glm/glm.hpp>

class Rect {
public:
    glm::vec3 pos;   // Position vector (x, y, z)
    glm::vec3 size;  // Size vector (width, height, depth)

    Rect() = default;

    // Constructor with position and size
    Rect(const glm::vec3& position, glm::vec3 size)
        : pos(position), size(size) 
    {}

    bool intersects(const Rect& other) const {
        return (pos.x < other.pos.x + other.size.x &&
            pos.x + size.x > other.pos.x &&
            pos.y < other.pos.y + other.size.y &&
            pos.y + size.y > other.pos.y &&
            pos.z < other.pos.z + other.size.z &&
            pos.z + size.z > other.pos.z);
    }

    void display() {
        std::cout << "Rect Details:" << std::endl;
        std::cout << "  Position: ("
            << pos.x << ", "
            << pos.y << ", "
            << pos.z << ")" << std::endl;
        std::cout << "  Size: ("
            << size.x << ", "
            << size.y << ", "
            << size.z << ")" << std::endl;

    }
};