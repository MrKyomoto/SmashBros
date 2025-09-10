#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "camera.h"
#include "util.h"
#include "platform.h"

#include <iostream>

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platfrom_large;
extern IMAGE img_platfrom_small;

extern Camera main_camera;
extern std::vector<Platform> platfrom_list;

extern SceneManager scene_manager;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() {
		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;
	}

	void on_update(int delta) {

	}

	void on_draw(const Camera& camera) {
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);
	}

	void on_input(const ExMessage& msg){
	}

	void on_exit() {
		std::cout << "Quit game menu" << std::endl;
	}

private:
	POINT pos_img_sky = { 0 };
	POINT pos_img_hills = { 0 };

};

