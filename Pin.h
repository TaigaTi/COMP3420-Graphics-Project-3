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

class Pin
{

public:
	const float SCALE = 23.0f;
	const float FALL_SPEED = 14.0f;

	glm::vec3 position;
	Shader shader;
	Model model;
	Rect rect;

	Pin()
		: position(glm::vec3(0.0f, 0.0f, 0.0f)),
		shader("bowlingPinsVertexShader.glsl", "bowlingPinsFragmentShader.glsl"),
		model((GLchar*)"bowling_pins.obj"),
		rect(position, glm::vec3(SCALE, SCALE, SCALE))
	{}

	void setPosition(glm::vec3 newPosition) {
		this->position = newPosition;
		this->rect.pos = (this->position);
	}

	void move(glm::vec3 offset) {
		this->position.x += offset.x;
		this->position.y += offset.y;
		this->position.z += offset.z;

		this->rect.pos = (this->position);
	}

	void fall(float deltaTime) {
		// If pin is in the air, make it fall!

		if (this->position.y >= 0.2f) {
			move(glm::vec3(0, -FALL_SPEED, 0) * deltaTime);
		}
	}

	void setProjection(glm::mat4 projection) {
		this->shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}

	void draw(Camera camera) {
		this->shader.Use();

		// Setup View Matrix
		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));

		// Setup Model Matrix

		glm::mat4 model = glm::mat4(1);

		model = glm::translate(model, this->position * SCALE); // Adjust Z so it's behind wall
		model = glm::scale(model, glm::vec3(SCALE));
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, -580.0f)); // Adjust Z so it's behind wall

		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Textures
		glUniform1i(glGetUniformLocation(this->shader.Program, "texture_diffuse1"), 0);

		this->model.Draw(this->shader);
	}

};
