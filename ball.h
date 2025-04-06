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

class Ball
{

public:
	const float SCALE = 25.0f;
	const float SPEED = 29.0f;

	glm::vec3 position;
	Shader shader;
	Model model;

	Ball() {
		this->position = glm::vec3(0.0f, 0.0f, 0.0f);
		this->shader = Shader("ballVertexShader.glsl", "ballFragmentShader.glsl");
		this->model = Model((GLchar*)"bowling_ball.obj");
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

};
