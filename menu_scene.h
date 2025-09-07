#pragma once

#include "scene.h"
#include "menu_scene.h"
#include "scene_manager.h"

#include <iostream>

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() {
		std::cout << "Come to the Main Menu" << std::endl;
	}

	void on_update() {
		std::cout << "Running main menu" << std::endl;
	}

	void on_draw() {
		outtextxy(10, 10, _T("draw in main menu"));
	}

	void on_input(const ExMessage& msg){
		if (msg.message == WM_KEYDOWN) {
			scene_manager.switch_scene(SceneManager::SceneType::Game);
		}
	}

	void on_exit() {
		std::cout << "Quit main menu" << std::endl;
	}
private:

};

