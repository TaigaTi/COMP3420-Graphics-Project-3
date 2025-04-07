#pragma once

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Our Includes
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "rect.h"
#include "boundary.h"

class Ball
{

public:
	const float SCALE = 25.0f;
	const float SPEED = 90.0f;

	glm::vec3 position;
	Shader shader;
	Model model;
	Rect rect;

	bool isRolling;

	Ball()
		: position(glm::vec3(0.0f, 0.0f, 0.0f)), // Initialize position
		shader("ballVertexShader.glsl", "ballFragmentShader.glsl"), // Initialize shader
		model((GLchar*)"bowling_ball.obj"), // Initialize model
		rect(position, glm::vec3(SCALE, SCALE, SCALE)), // Initialize Rect with position and size
		isRolling(false) // Set the rolling value to false
	{}

	void setProjection(glm::mat4 projection) {
		this->shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}

	float getBallSpeed(float deltaTime) {
		return SPEED * deltaTime;
	}

	void draw(Camera camera) {
		this->shader.Use();

		// Setup View Matrix
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		
		// Setup Model Matrix

		glm::mat4 model = glm::mat4(1);

		model = glm::translate(model, this->position * SCALE);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, (float)glfwGetTime() * 10, glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(SCALE));

		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		
		// Textures
		glUniform1i(glGetUniformLocation(this->shader.Program, "texture_diffuse1"), 0);

		this->model.Draw(this->shader);
	}

	void move(int right = 0, int forward = 0, float deltaTime=0.0f) {
		this->position.x += this->getBallSpeed(deltaTime) * right;
		this->position.z += this->getBallSpeed(deltaTime) * forward;
		this->rect.pos = this->position;
	}

	void setPosition(float x, float y, float z) {
		this->position = glm::vec3(x, y, z);
		this->rect.pos = this->position;
	}

	void boundsCheck(Boundary boundary) {
		if (this->position.z < boundary.back) {
			this->isRolling = false;
			setPosition(0, 0, 0);
		}
		if (this->position.x < boundary.left) {
			this->position.x = boundary.left;
		}
		if (this->position.x > boundary.right) {
			this->position.x = boundary.right;
		}
	}

	bool checkCollision(Rect rect) {
		return this->rect.intersects(rect);
	}

};
