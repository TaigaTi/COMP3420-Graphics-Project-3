#pragma once

#include <SFML/Audio.hpp>

// This class will be responsible for managing sound and music 
class Sound {
public:
	char* path;
	float volume;
	glm::vec3 position;
	sf::SoundBuffer buffer;
	sf::Sound sound;

	Sound(const char* path, float volume, glm::vec3 position, bool loop=false) {
		this->buffer.loadFromFile(path);

		sound.setBuffer(buffer);

		sound.setVolume(volume);
		sound.setPosition(position.x, position.y, position.z);
		sound.setLoop(loop);
	}

	void setPosition(glm::vec3 newPosition) {
		this->position = newPosition;
		sound.setPosition(position.x, position.y, position.z);
	}

	void offsetPosition(glm::vec3 offset) {
		this->position.x += offset.x;
		this->position.y += offset.y;
		this->position.z += offset.z;
	}

	void play() {
		this->sound.play();
	}

	void displaySoundInfo() {
		std::cout << "Sound Status: " << sound.getStatus() << std::endl;
		std::cout << "Volume: " << sound.getVolume() << std::endl;
		std::cout << "Position: (" << sound.getPosition().x << sound.getPosition().y << sound.getPosition().z << ")" << std::endl;
		std::cout << "Looping: " << sound.getLoop() << std::endl;
		std::cout << "Status: " << sound.getStatus() << std::endl;
	}

};