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
	const float FALL_SPEED = 14.0f;
	const float LAUNCH_HEIGHT = 50.0f;

	glm::vec3 position;
	Shader shader;
	Model model;
	Rect rect;
	Sound sound;

	Pin(Sound sound)
		: position(glm::vec3(0.0f, 0.0f, 0.0f)),
		shader("bowlingPinsVertexShader.glsl", "bowlingPinsFragmentShader.glsl"),
		model((GLchar*)"bowling_pins.obj"),
		rect(position, glm::vec3(SCALE, SCALE, SCALE)),
		sound(sound)
	{
	}

	void setPosition(glm::vec3 newPosition) {
		this->position = newPosition;
		this->rect.pos = (this->position);
		this->sound.setPosition(newPosition);
	}

	void launch() {
		this->move(glm::vec3(0, this->LAUNCH_HEIGHT, 0));
		this->sound.play();
		//this->sound.displaySoundInfo();
	}

	void move(glm::vec3 offset) {
		this->position.x += offset.x;
		this->position.y += offset.y;
		this->position.z += offset.z;

		this->rect.pos = (this->position);
		this->sound.setPosition(position);

	}

	bool isFalling() {
		return this->position.y >= 0.2f;
	}

	void fall(float deltaTime) {
		// If pin is in the air, make it fall!

		if (isFalling()) {
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
		if (isFalling()) {
			// Do some funky falling animation. :D
			model = glm::rotate(model, (float)glfwGetTime() * 20, glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		else {
			// Play wiggle animation
			model = glm::rotate(model, (float)glfwGetTime() * 5, glm::vec3(0.05f, 1.0f, 0.0f));
		}
		model = glm::scale(model, glm::vec3(SCALE));

		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Add textures
		glUniform1i(glGetUniformLocation(this->shader.Program, "texture_diffuse1"), 0);

		this->model.Draw(this->shader);
	}

};
