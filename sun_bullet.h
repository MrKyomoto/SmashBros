#pragma once
#include "bullet.h"
#include "animation.h"
#include "camera.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;
extern Camera main_camera;

class SunBullet : public Bullet
{
public:
	SunBullet() {
		size.x = 96;
		size.y = 96;

		damage = 20;

		animation_idle.set_atlas(&atlas_sun);
		animation_idle.set_interval(50);

		animation_explode.set_atlas(&atlas_sun_explode);
		animation_explode.set_interval(50);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() {removable = true; });

		IMAGE* frame_idle = animation_idle.get_image_frame();
		IMAGE* frame_explode = animation_explode.get_image_frame();
		explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
		explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;
	}
	~SunBullet() = default;

	void on_collide() {
		Bullet::on_collide();

		main_camera.shake(5, 250);

		mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
	}

	void on_update(int delta) {
		if (valid) {
			velocity.y += gravity * delta;
			position += velocity * (float)delta;
		}

		if (!valid) {
			animation_explode.on_update(delta);
		}
		else {
			animation_idle.on_update(delta);
		}

		if (check_if_out_of_screen()) {
			removable = true;
		}
	}

	void on_draw(const Camera& camera) const {
		if (valid) {
			animation_idle.on_draw(camera,(int)position.x,(int)position.y);
		}
		else {
			animation_explode.on_draw(camera, (int)(position.x + explode_render_offset.x), (int)(position.y + explode_render_offset.y));
		}
	}
private:
	const float gravity = 1e-3f;
private:
	Animation animation_idle;
	Animation animation_explode;
	Vector2 explode_render_offset; // ±¬Õ¨¶¯»­äÖÈ¾Æ«ÒÆ

};

