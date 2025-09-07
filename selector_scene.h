#pragma once
#include "scene.h"
#include "scene_manager.h"
#include <iostream>

extern SceneManager scene_manager;

class SelectorScene : public Scene
{
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void on_enter() {
		std::cout << "Come to the Selector Menu" << std::endl;
	}

	void on_update() {
		std::cout << "Running Selector menu" << std::endl;
	}

	void on_draw() {
		outtextxy(10, 10, _T("draw in Selector menu"));
	}

	void on_input(const ExMessage& msg){
		if (msg.message == WM_KEYDOWN) {
			scene_manager.switch_scene(SceneManager::SceneType::Menu);
		}
	}

	void on_exit() {
		std::cout << "Quit Selector menu" << std::endl;
	}

private:

};

