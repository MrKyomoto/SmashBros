#pragma once
#include "player.h"
#include "util.h"

class StatusBar
{
public:
	StatusBar() = default;
	~StatusBar() = default;

	void set_avatar(IMAGE* img) {
		img_avatar = img;
	}

	void set_position(int x, int y) {
		position.x = x;
		position.y = y;
	}

	// 状态栏不需要依据摄像机进行渲染因此可以不要camera参数
	void on_draw() {
		putimage_alpha(position.x, position.y, img_avatar);

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3 * 2, position.y + 36, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3 * 2, position.y + 71, 8, 8);

		setfillcolor(RGB(67, 67, 67));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3, position.y + 33, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3, position.y + 68, 8, 8);

		float hp_bar_width = width * max(0, hp) / 100.0f;
		float mp_bar_width = width * min(100, mp) / 100.0f;
		setfillcolor(RGB(197, 61, 67));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + (int)hp_bar_width + 3, position.y + 33, 8, 8);
		setfillcolor(RGB(83, 131, 195));
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + (int)mp_bar_width + 3, position.y + 68, 8, 8);

	}

	void on_update(int delta) {
		set_hp();
		set_mp();
	}

	void bind_player(const Player* player) {
		this->player = player;
	}
private:
	const int width = 275;

private:
	POINT position = { 0 };
	int hp = 0;
	int mp = 0;
	const Player* player;
	IMAGE* img_avatar = nullptr;

private:
	void set_hp() {
		hp = player->get_hp();
	}

	void set_mp() {
		mp = player->get_mp();
	}
};

