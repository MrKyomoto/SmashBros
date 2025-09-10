#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "camera.h"
#include "util.h"
#include "platform.h"
#include "player.h"

#include <iostream>

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;
extern bool is_debug;

extern Camera main_camera;
extern std::vector<Platform> platform_list;

extern SceneManager scene_manager;

extern Player* player_1;
extern Player* player_2;

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

		platform_list.resize(4);
		Platform& large_platform = platform_list[0];
		large_platform.set_img(&img_platform_large);
		large_platform.set_position(122, 455);
		// shape 略小于实际的图片尺寸,符合游戏直觉
		const POINT& large_platform_pos = large_platform.get_position();
		large_platform.set_shape(large_platform_pos.y + 60, large_platform_pos.x + 30, large_platform_pos.x + large_platform.get_img()->getwidth() - 30);

		Platform& small_platform_1 = platform_list[1];
		Platform& small_platform_2 = platform_list[2];
		Platform& small_platform_3 = platform_list[3];
		const POINT& small_platform_1_pos = small_platform_1.get_position();
		const POINT& small_platform_2_pos = small_platform_2.get_position();
		const POINT& small_platform_3_pos = small_platform_3.get_position();

		small_platform_1.set_img(&img_platform_small);
		small_platform_1.set_position(175, 360);
		small_platform_1.set_shape(small_platform_1_pos.y + small_platform_1.get_img()->getheight() / 2,small_platform_1_pos.x + 40, small_platform_1_pos.x + small_platform_1.get_img()->getwidth() - 40);

		small_platform_2.set_img(&img_platform_small);
		small_platform_2.set_position(855, 360);
		small_platform_2.set_shape(small_platform_2_pos.y + small_platform_2.get_img()->getheight() / 2,small_platform_2_pos.x + 40, small_platform_2_pos.x + small_platform_2.get_img()->getwidth() - 40);

		small_platform_3.set_img(&img_platform_small);
		small_platform_3.set_position(515, 225);
		small_platform_3.set_shape(small_platform_3_pos.y + small_platform_3.get_img()->getheight() / 2,small_platform_3_pos.x + 40, small_platform_3_pos.x + small_platform_3.get_img()->getwidth() - 40);
	}

	void on_update(int delta) {
		player_1->on_update(delta);
		player_2->on_update(delta);
	}

	void on_draw(const Camera& camera) {
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

		for (const Platform& platform : platform_list) {
			platform.on_draw(camera);
		}

		if (is_debug) {
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("Dubug mode ON, press <Q> to quit"));
		}
	}

	void on_input(const ExMessage& msg){
		switch (msg.message)
		{
		case WM_KEYUP:
			// 'Q'
			if (msg.vkcode == 0x51) {
				is_debug = !is_debug;
			}
		default:
			break;
		}
	}

	void on_exit() {
		std::cout << "Quit game menu" << std::endl;
	}

private:
	POINT pos_img_sky = { 0 };
	POINT pos_img_hills = { 0 };

};

