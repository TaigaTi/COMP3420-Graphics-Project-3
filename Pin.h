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

class Pin
{

public:
	const float SCALE = 25.0f;
	const float SPEED = 29.0f;

	glm::vec3 position;
	Shader shader;
	Model model;

	Pin(glm::vec3 position) {
		this->position = position;
		this->shader = Shader("bowlingPinsVertexShader.glsl", "bowlingPinsFragmentShader.glsl");
		this->model = Model((GLchar*)"bowling_pins.obj");
	}

	Pin() {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->shader = Shader("bowlingPinsVertexShader.glsl", "bowlingPinsFragmentShader.glsl");
		this->model = Model((GLchar*)"bowling_pins.obj");
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

		model = glm::scale(model, glm::vec3(20.0f));
		model = glm::translate(model, this->position); // Adjust Z so it's behind wall
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, -580.0f)); // Adjust Z so it's behind wall

		glUniformMatrix4fv(glGetUniformLocation(this->shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Textures
		glUniform1i(glGetUniformLocation(this->shader.Program, "texture_diffuse1"), 0);

		this->model.Draw(this->shader);
	}

};
