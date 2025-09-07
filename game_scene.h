#pragma once
#include "scene.h"
#include "scene_manager.h"
#include <iostream>

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() {
		std::cout << "Come to the Game Menu" << std::endl;
	}

	void on_update() {
		std::cout << "Running game menu" << std::endl;
	}

	void on_draw() {
		outtextxy(10, 10, _T("draw in game menu"));
	}

	void on_input(const ExMessage& msg){
		if (msg.message == WM_KEYDOWN) {
			scene_manager.switch_scene(SceneManager::SceneType::Menu);
		}
	}

	void on_exit() {
		std::cout << "Quit game menu" << std::endl;
	}

private:

};

