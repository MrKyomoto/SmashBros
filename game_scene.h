#pragma once
#include "scene.h"
#include "scene_manager.h"
#include "camera.h"
#include "util.h"
#include "platform.h"
#include "player.h"
#include "bullet.h"
#include "status_bar.h"

#include <iostream>

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern IMAGE img_1P_winner;
extern IMAGE img_2P_winner;
extern IMAGE img_winner_bar;

extern bool is_debug;

extern Camera main_camera;
extern std::vector<Platform> platform_list;

extern SceneManager scene_manager;

extern Player* player_1;
extern Player* player_2;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern std::vector<Bullet*> bullet_list;

class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() {
		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);

		is_game_over = false;
		is_slide_out_started = false;

		pos_img_winner_bar.x = -img_winner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

		pos_img_winner_text.x = -img_winner_bar.getwidth();
		pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_wait_time(2500);
		timer_winner_slide_in.set_one_shot(true);
		timer_winner_slide_in.set_callback([&]() {
			is_slide_out_started = true;
			});

		timer_winner_slide_out.restart();
		timer_winner_slide_out.set_wait_time(1000);
		timer_winner_slide_out.set_one_shot(true);
		timer_winner_slide_out.set_callback([&]() {
			scene_manager.switch_scene(SceneManager::SceneType::Menu);
			});

		status_bar_1P.bind_player(player_1);
		status_bar_1P.set_avatar(img_player_1_avatar);
		status_bar_1P.set_position(235,625);
		status_bar_2P.bind_player(player_2);
		status_bar_2P.set_avatar(img_player_2_avatar);
		status_bar_2P.set_position(675,625);

		player_1->set_position(200, 50);
		player_2->set_position(975, 50);

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

		main_camera.on_update(delta);

		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet) {
				bool deletable = bullet->check_removable();
				if (deletable) delete bullet;
				return deletable;
			}),
			bullet_list.end()
		);
		for (Bullet* bullet : bullet_list) {
			bullet->on_update(delta);
		}

		const Vector2& position_player_1 = player_1->get_position();
		const Vector2& position_player_2 = player_2->get_position();
		if (position_player_1.y >= getheight() && position_player_1.y <= getheight() + 50) {
			player_1->set_hp(0);
			mciSendString(_T("play manba_out from 0"), NULL, 0, NULL);
			main_camera.shake(50, 250);
		}
		if (position_player_2.y == getheight() && position_player_2.y <= getheight() + 50) {
			player_2->set_hp(0);
			mciSendString(_T("play manba_out from 0"), NULL, 0, NULL);
			main_camera.shake(50, 250);
		}
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0) {
			if (!is_game_over) {
				mciSendString(_T("stop bgm_game"),NULL,0,NULL);
				mciSendString(_T("play ui_win from 0"),NULL,0,NULL);
			}

			is_game_over = true;
		}

		status_bar_1P.on_update(delta);
		status_bar_2P.on_update(delta);

		if (is_game_over) {
			pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
			pos_img_winner_text.x += (int)(speed_winner_text * delta);

			if (!is_slide_out_started) {
				timer_winner_slide_in.on_update(delta);
				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst) {
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				}
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst) {
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
				}
			}
			else {
				timer_winner_slide_out.on_update(delta);
			}

		}

	}

	void on_draw(const Camera& camera) {
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

		for (const Platform& platform : platform_list) {
			platform.on_draw(camera);
		}

		player_1->on_draw(camera);
		player_2->on_draw(camera);

		if (is_debug) {
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("Dubug mode ON, press <Q> to quit"));
		}

		for (const Bullet* bullet : bullet_list) {
			bullet->on_draw(camera);
		}

		if (!is_game_over) {
			status_bar_1P.on_draw();
			status_bar_2P.on_draw();
		}
		else {
			putimage_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y, player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}
	}

	void on_input(const ExMessage& msg){
		player_1->on_input(msg);
		player_2->on_input(msg);

		switch (msg.message)
		{
		case WM_KEYUP:
			// 'Q'
			if (msg.vkcode == 0x51) {
				is_debug = !is_debug;
		//pos_dash_sketch[1].x = pos_dash_sketch[0].x + (position.x - pos_dash_sketch[0].x) / 2;
			}
		default:
			break;
		}
	}

	void on_exit() {
		std::cout << "Quit game menu" << std::endl;
		delete player_1;
		player_1 = nullptr;
		delete player_2;
		player_2 = nullptr;

		is_debug = false;

		bullet_list.clear();
		main_camera.reset();
		mciSendString(_T("stop bgm_game"),NULL,0,NULL);
	}

private:
	POINT pos_img_sky = { 0 };
	POINT pos_img_hills = { 0 };

	StatusBar status_bar_1P;
	StatusBar status_bar_2P;

	bool is_game_over = false;

	POINT pos_img_winner_bar = { 0 };
	POINT pos_img_winner_text = { 0 };
	int pos_x_img_winner_bar_dst = 0; // 结算动效背景移动的目标位置
	int pos_x_img_winner_text_dst = 0;
	Timer timer_winner_slide_in;
	Timer timer_winner_slide_out;
	bool is_slide_out_started = false;

private:
	const float speed_winner_bar = 3.0f;
	const float speed_winner_text = 1.5f;

};

