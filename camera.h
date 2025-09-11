#pragma once

#include "vector2.h"
#include "timer.h"

class Camera
{
public:
	Camera() {
		timer_shake.set_one_shot(true);
		// 定时器的作用就是用来终止摄像机的抖动的
		timer_shake.set_callback([&]() {
			is_shaking = false;
			reset();
			});
	}
	~Camera() = default;

	const Vector2& get_position() const {
		return position;
	}

	void reset() {
		position.x = 0;
		position.y = 0;
	}

	void shake(float strength, int duration) {
		is_shaking = true;
		shaking_strength = strength;

		timer_shake.set_wait_time(duration);
		timer_shake.restart();
	}

	void on_update(int delta) {
		timer_shake.on_update(delta);
		if (is_shaking) {
			position.x = (-25 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-25 + rand() % 100) / 50.0f * shaking_strength;
		}
	}
private:
	Vector2 position;
	Timer timer_shake; // 摄像机抖动定时器
	bool is_shaking = false;
	float shaking_strength = 0;
};

