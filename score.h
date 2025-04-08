#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include <queue>

#include "sound.h"

class Score {

public:
	int score;
	Sound sound1;
	Sound sound2;
	Sound sound3;
	Sound sound4;
	Sound sound5;
	Sound sound6;
	Sound sound7;
	Sound sound8;
	Sound sound9;
	Sound sound10;

    std::queue<Sound*> soundQueue;

    Score()
        : score(0),
        sound1("score_sounds/1.wav", 70.0f, glm::vec3(0)),
        sound2("score_sounds/2.wav", 70.0f, glm::vec3(0)),
        sound3("score_sounds/3.wav", 70.0f, glm::vec3(0)),
        sound4("score_sounds/4.wav", 70.0f, glm::vec3(0)),
        sound5("score_sounds/5.wav", 70.0f, glm::vec3(0)),
        sound6("score_sounds/6.wav", 70.0f, glm::vec3(0)),
        sound7("score_sounds/7.wav", 70.0f, glm::vec3(0)),
        sound8("score_sounds/8.wav", 70.0f, glm::vec3(0)),
        sound9("score_sounds/9.wav", 70.0f, glm::vec3(0)),
        sound10("score_sounds/10.wav", 70.0f, glm::vec3(0))
    {
    }

	void incrScore() {
		score = std::min(score+1, 10);
        cout << score << endl;
        switch (score) {
        case 1: sound1.play(); break;
        case 2: sound2.play(); break;
        case 3: sound3.play(); break;
        case 4: sound4.play(); break;
        case 5: sound5.play(); break;
        case 6: sound6.play(); break;
        case 7: sound7.play(); break;
        case 8: sound8.play(); break;
        case 9: sound9.play(); break;
        case 10: sound10.play(); break;
        default:
            std::cerr << "Error: Invalid score value!" << std::endl;
        }

	}
	
	void resetScore() {
		score = 0;
	}
};