#pragma once

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Local Includes
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "rect.h"
#include "sound.h"

class Pin
{
public:
	const float SCALE = 23.0f;
	const float GRAVITY = 9.8f;
	const float BOUNCE_DAMPING = 0.6f;
	float fallRotationAngle = 0.0f;
	bool hasFallen = false;
	bool isLaunched = false;

	glm::vec3 position;
	glm::vec3 originalPosition;
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f, -GRAVITY, 0.0f);
	glm::vec3 fallDirection = glm::vec3(1.0f, 0.0f, 0.0f);

	Shader shader;
	Model model;
	Rect rect;
	Sound sound;

	Pin(Sound sound)
		: position(glm::vec3(0.0f, 0.0f, 0.0f)),
		originalPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
		shader("bowlingPinsVertexShader.glsl", "bowlingPinsFragmentShader.glsl"),
		model((GLchar*)"bowling_pins.obj"),
		rect(position, glm::vec3(SCALE, SCALE, SCALE)),
		sound(sound)
	{
	}

	void setPosition(glm::vec3 newPosition) {
		this->position = newPosition;
		this->originalPosition = newPosition;
		this->rect.pos = this->position;
		this->sound.setPosition(newPosition);
	}

	void launch(glm::vec3 hitDirection) {
		this->fallRotationAngle = 0.0f;
		this->hasFallen = false;
		this->isLaunched = true;

		// Normalize the hit direction
		glm::vec3 normalizedHit = glm::normalize(hitDirection);

		// Compute fall rotation axis (cross with up vector to get perpendicular)
		this->fallDirection = normalizedHit; // fall direction aligns with hit direction

		// Launch velocity includes upward + hit direction
		this->velocity = normalizedHit * 20.0f + glm::vec3(0.0f, 1.0f, 0.0f);

		this->sound.play();
	}


	void move(glm::vec3 offset) {
		this->position += offset;
		this->rect.pos = this->position;
		this->sound.setPosition(position);
	}

	bool isFalling() {
		return this->isLaunched && !hasFallen;
	}

	void fall(float deltaTime) {
		if (!isFalling()) return;

		velocity += acceleration * deltaTime;
		move(velocity * deltaTime);

		// bounce check
		if (position.y <= 0.0f) {
			position.y = 0.0f;
			velocity.y *= -BOUNCE_DAMPING;

			// if bounce is too small, stop
			if (abs(velocity.y) < 0.5f) {
				velocity = glm::vec3(0.0f);
				hasFallen = true;

				// Auto-reset to original position after fall
				resetToOriginalPosition();
			}
		}

		// Slowly increase fall rotation (max ~90 degrees)
		if (fallRotationAngle < glm::radians(90.0f)) {
			fallRotationAngle += glm::radians(300.0f) * deltaTime; // 60 degrees/sec
		}
		else {
			fallRotationAngle = glm::radians(90.0f);
		}
	}

	void resetToOriginalPosition() {
		this->position = originalPosition;
		this->rect.pos = originalPosition;
		this->sound.setPosition(originalPosition);
		this->fallRotationAngle = 0.0f;
		this->velocity = glm::vec3(0.0f);
		this->isLaunched = false;
		this->hasFallen = false;
	}

	void setProjection(glm::mat4 projection) {
		this->shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}

	void draw(Camera camera) {
		this->shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		glm::mat4 model = glm::mat4(1);
		model = glm::translate(model, this->position * SCALE);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f * SCALE, 0.0f));

		if (isFalling()) {
			model = glm::rotate(model, fallRotationAngle, fallDirection);
		}
		else {
			model = glm::rotate(model, (float)glfwGetTime() * 5, glm::vec3(0.05f, 1.0f, 0.0f));
		}

		model = glm::translate(model, glm::vec3(0.0f, 0.5f * SCALE, 0.0f));
		model = glm::scale(model, glm::vec3(SCALE));

		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(this->shader.Program, "texture_diffuse1"), 0);
		this->model.Draw(this->shader);
	}
};
