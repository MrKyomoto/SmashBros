#pragma once

#include "scene.h"
#include "scene_manager.h"
#include "atlas.h"
#include "animation.h"
#include "camera.h"

#include <iostream>

extern Atlas atlas_peashooter_run_right;

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() {
		std::cout << "Come to the Main Menu" << std::endl;
		animation_peashooter_run_right.set_atlas(&atlas_peashooter_run_right);
		animation_peashooter_run_right.set_interval(75);
		animation_peashooter_run_right.set_loop(true);
		/*animation_peashooter_run_right.set_callback(
			[]() {
				scene_manager.switch_scene(SceneManager::SceneType::Game);
			}
		);*/
	}

	void on_update(int delta) {
		std::cout << "Running main menu" << std::endl;
		camera.on_update(delta);
		animation_peashooter_run_right.on_update(delta);
	}

	void on_draw() {
		outtextxy(10, 10, _T("draw in main menu"));
		const Vector2& pos_camera = camera.get_position();
		// (100,100)是世界坐标,需要将其渲染在窗口坐标中,窗口坐标 = 世界坐标 - 摄像机坐标
		animation_peashooter_run_right.on_draw((int)100 - pos_camera.x, 100 - pos_camera.y);
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
	Animation animation_peashooter_run_right;
	Camera camera;
};

