#pragma once

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

// Local Includes
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "rect.h"
#include "sound.h"

class Pin
{
public:
	const float SCALE = 23.0f;                  // Uniform scale for the pin
	const float GRAVITY = 9.8f;                 // Gravity acceleration constant
	const float BOUNCE_DAMPING = 0.6f;          // Damping factor for bounce

	float fallRotationAngle = 0.0f;             // Angle for simulating fall rotation
	bool hasFallen = false;                     // Whether the pin has fallen completely
	bool isLaunched = false;                    // Whether the pin has been hit/launched
	bool awaitingReset = false;					// Whether the pin is waiting to be reset
	float resetDelaySeconds = 2.0f;				// Delay in seconds before the pin resets
	bool canCollide = true;						// Whether or not the ball can go BONK

	std::chrono::high_resolution_clock::time_point fallEndTime; // Time when the pin falls

	glm::vec3 position;                         // Current position
	glm::vec3 originalPosition;                 // Original standing position
	glm::vec3 velocity = glm::vec3(0.0f);       // Current velocity
	glm::vec3 acceleration = glm::vec3(0.0f, -GRAVITY, 0.0f); // Downward acceleration
	glm::vec3 fallDirection = glm::vec3(1.0f, 0.0f, 0.0f);    // Axis to rotate around when falling

	Shader shader;                              // Shader program for the pin
	Model model;                                // 3D model of the pin
	Rect rect;                                  // Bounding box or UI interaction helper
	Sound sound;                                // Sound played when pin is hit

	// Constructor
	Pin(Sound sound)
		: position(glm::vec3(0.0f, 0.0f, 0.0f)),
		originalPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
		shader("bowlingPinsVertexShader.glsl", "bowlingPinsFragmentShader.glsl"),
		model((GLchar*)"bowling_pins.obj"),
		rect(position, glm::vec3(SCALE, SCALE, SCALE)),
		sound(sound)
	{
	}

	// Set pin position and update dependent objects
	void setPosition(glm::vec3 newPosition) {
		this->position = newPosition;
		this->originalPosition = newPosition;
		this->rect.pos = this->position;
		this->sound.setPosition(newPosition);
	}

	// Launch the pin with a given direction of force
	void launch(glm::vec3 hitDirection) {
		this->fallRotationAngle = 0.0f;
		this->hasFallen = false;
		this->isLaunched = true;

		glm::vec3 normalizedHit = glm::normalize(hitDirection); // Ensure direction is normalized

		this->fallDirection = normalizedHit; // Direction for rotation during fall

		// Apply initial velocity in hit direction plus a small upward lift
		this->velocity = normalizedHit * 20.0f + glm::vec3(0.0f, 1.0f, 0.0f);

		this->sound.play(); // Play sound effect
	}

	// Move the pin by a specified offset
	void move(glm::vec3 offset) {
		this->position += offset;
		this->rect.pos = this->position;
		this->sound.setPosition(position);
	}

	// Check if the pin is currently falling
	bool isFalling() {
		return this->isLaunched && !hasFallen;
	}

	// Simulate physics and falling over time
	void fall(float deltaTime) {
		if (!isFalling()) return;

		// Apply gravity
		velocity += acceleration * deltaTime;

		// Update position based on velocity
		move(velocity * deltaTime);

		// Check for bounce on ground
		if (position.y <= 0.0f) {
			position.y = 0.0f;
			velocity.y *= -BOUNCE_DAMPING;

			// Stop bouncing if it's too small
			if (abs(velocity.y) < 0.5f) {
				velocity = glm::vec3(0.0f);
				hasFallen = true;

				// Start reset timer to check delay before resetting pin
				awaitingReset = true;
				fallEndTime = std::chrono::high_resolution_clock::now();
			}
		}

		// Increase rotation angle until reaching 90 degrees
		if (fallRotationAngle < glm::radians(90.0f)) {
			fallRotationAngle += glm::radians(300.0f) * deltaTime; // Speed of rotation
		}
		else {
			fallRotationAngle = glm::radians(90.0f); // Cap rotation
		}
	}

	// Checks and performs reset of the pin to original position
	void updateResetTimer() {
		if (awaitingReset) {
			auto now = std::chrono::high_resolution_clock::now();
			float elapsed = std::chrono::duration<float>(now - fallEndTime).count();

			// Reset pin back to original position after delay
			if (elapsed >= resetDelaySeconds) {
				resetToOriginalPosition();
				awaitingReset = false;
			}
		}
	}

	// Reset pin to original state and position
	void resetToOriginalPosition() {
		this->position = originalPosition;
		this->rect.pos = originalPosition;
		this->sound.setPosition(originalPosition);
		this->fallRotationAngle = 0.0f;
		this->velocity = glm::vec3(0.0f);
		this->isLaunched = false;
		this->hasFallen = false;
		this->canCollide = true;
	}

	// Pass projection matrix to the shader
	void setProjection(glm::mat4 projection) {
		this->shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}

	// Draw the pin on screen
	void draw(Camera camera) {
		this->shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		glm::mat4 model = glm::mat4(1);

		// Move model to current position
		model = glm::translate(model, this->position * SCALE);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f * SCALE, 0.0f));

		// Rotate the model if it is falling, otherwise idle animation
		if (isFalling() || awaitingReset) {
			model = glm::rotate(model, fallRotationAngle, fallDirection);
		}
		else {
			model = glm::rotate(model, (float)glfwGetTime() * 5, glm::vec3(0.05f, 1.0f, 0.0f)); // idle spin
		}

		// Final transformations
		model = glm::translate(model, glm::vec3(0.0f, 0.5f * SCALE, 0.0f));
		model = glm::scale(model, glm::vec3(SCALE));

		// Set the model matrix and draw
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(this->shader.Program, "texture_diffuse1"), 0);
		this->model.Draw(this->shader);
	}
};
