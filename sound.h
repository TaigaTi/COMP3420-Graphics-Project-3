#pragma once

#include <SFML/Audio.hpp>

// This class will be responsible for managing sound and music 
class Sound {

	char* path;
	float volume;
	glm::vec3 position;
	sf::Sound sound;

	Sound(char* path, float volume, glm::vec3 position, bool loop=false) {
		sf::SoundBuffer buffer;
		buffer.loadFromFile(path);

		sound.setBuffer(buffer);

		sound.setVolume(volume);
		sound.setPosition(position.x, position.y, position.z);
		sound.setLoop(loop);
	}

	void setPosition(glm::vec3 newPosition) {
		this->position = newPosition;
	}

	void offsetPosition(glm::vec3 offset) {
		this->position.x += offset.x;
		this->position.y += offset.y;
		this->position.z += offset.z;
	}

	void play() {
		this->sound.play();
	}

};