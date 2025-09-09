#pragma once

#include "scene.h"
#include "scene_manager.h"
#include "atlas.h"
#include "animation.h"
#include "camera.h"
#include "timer.h"

#include <iostream>

extern IMAGE img_menu_background;

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() {
		//std::cout << "Come to the Main Menu" << std::endl;
		mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);
	}

	void on_update(int delta) {
		//std::cout << "Running main menu" << std::endl;
	}

	void on_draw(const Camera& camera) {
		outtextxy(10, 10, _T("draw in main menu"));
		putimage(0, 0, &img_menu_background);
	}

	void on_input(const ExMessage& msg){
		// 按键抬起时才进入选择界面
		if (msg.message == WM_KEYUP) {
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
			scene_manager.switch_scene(SceneManager::SceneType::Selector);
		}
	}

	void on_exit() {
		std::cout << "Quit main menu" << std::endl;
	}
private:
};

