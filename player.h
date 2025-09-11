#pragma once
#include "camera.h"
#include <graphics.h>
#include "vector2.h"
#include "animation.h"
#include "player_id.h"
#include "platform.h"
#include "timer.h"
#include "bullet.h";

#include <iostream>

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;
extern bool is_debug;

class Player
{
public:
	Player() {
		current_animation = &animation_idle_right;

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() {
			can_attack = true;
			});

		timer_invulnerable.set_wait_time(750);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]() {
			is_invulnerable = false;
			});

		timer_invulnerable_blink.set_wait_time(75);
		timer_invulnerable_blink.set_callback([&]() {
			is_showing_sketch_frame = !is_showing_sketch_frame;
			});
	}
	~Player() = default;

	virtual void on_update(int delta) {

		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0) {
			if (!is_attacking_ex) {
				is_facing_right = direction > 0;
			}

			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			float distance = direction * run_velocity * delta;
			on_run(distance);
		}
		else {
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
		}

		if (is_attacking_ex) {
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;
		}

		move_and_collide(delta);

		current_animation->on_update(delta);

		timer_attack_cd.on_update(delta);

		timer_invulnerable.on_update(delta);
		timer_invulnerable_blink.on_update(delta);
 
		if (is_showing_sketch_frame) {
			sketch_image(current_animation->get_image_frame(), &img_sketch);
		}
	}

	virtual void on_draw(const Camera& camera){
		if (hp > 0 && is_invulnerable && is_showing_sketch_frame) {
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		}
		else {
			current_animation->on_draw(camera, (int)position.x, (int)position.y);
		}
		if (is_debug) {
			setlinecolor(RGB(0, 125, 255));
			rectangle((int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}

	}

	virtual void on_input(const ExMessage& msg) {
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 'A'
				case 0x41:
					is_left_key_down = true;
					break;
					// 'D'
				case 0x44:
					is_right_key_down = true;
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT:
					is_left_key_down = true;
					break;
				case VK_RIGHT:
					is_right_key_down = true;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (id)
			{
			case PlayerID::P1:
				switch (msg.vkcode)
				{
					// 'A'
				case 0x41:
					is_left_key_down = false;
					break;
					// 'D'
				case 0x44:
					is_right_key_down = false;
					break;
					// 'W'
				case 0x57:
					on_jump();
					break;
					// 'F'
				case 0x46:
					if (can_attack) {
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
					// 'G'
				case 0x47:
					if (mp >= 100) {
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			case PlayerID::P2:
				switch (msg.vkcode)
				{
				case VK_LEFT:
					is_left_key_down = false;
					break;
				case VK_RIGHT:
					is_right_key_down = false;
					break;
				case VK_UP:
					on_jump();
					break;
					// '.'
				case VK_OEM_PERIOD:
					if (can_attack) {
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}
					break;
				case VK_OEM_2:
					if (mp >= 100) {
						on_attack_ex();
						mp = 0;
					}
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	void set_id(PlayerID id) {
		this->id = id;
	}

	void set_position(float x, float y) {
		position.x = x;
		position.y = y;
	}

	const Vector2& get_position() const {
		return position;
	}

	const Vector2& get_size() const {
		return size;
	}

	const int get_hp() const{
		return hp;
	}

	const int get_mp() const{
		return mp;
	}


	virtual void on_run(float distance) {
		if (is_attacking_ex) {
			return;
		}
		position.x += distance;
	}

	virtual void on_jump() {
		if (is_attacking_ex) {
			return;
		}

		for (const Platform& platform : platform_list) {
			const Platform::CollisionShape& shape = platform.get_shape();
			bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left) <= size.x + (shape.right - shape.left));
			// 只要角色x和平台重合且角色脚的y和平台相同,则意味着已经落地, 可以重置跳跃次数
			bool is_collide_y = shape.y == position.y + size.y;
			if (is_collide_x && is_collide_y) {
				jump_time = 2;
			}
		}
		if (velocity.y <= -0.50 || jump_time == 0) {
			return;
		}

		// TODO 可能以后要考虑加一个一段跳之后角色会变色,提示玩家此时再跳就是二段跳(参考蔚蓝)
		switch (jump_time)
		{
		case 2:
			velocity.y += jump_velocity_1;
			break;
		case 1:
			velocity.y += jump_velocity_2;
			break;
		default:
			break;
		}
		jump_time -= 1;
	}

	virtual void on_attack() { }
	virtual void on_attack_ex() { }

protected:
	const float run_velocity = 0.50f;
	const float gravity = 1.6e-3f;
	const float jump_velocity_1 = -0.70f;
	const float jump_velocity_2 = -0.55f;

	// 重力逻辑
	void move_and_collide(int delta) {
		velocity.y += gravity * delta;
		position += velocity * (float)delta;

		// 由于平台是单向可穿过的(从下边可以跳到上边),因此我们只需要在玩家速度向下(v.y > 0)时检测是否触碰到平台
		if (velocity.y > 0) {
			for (const Platform& platform : platform_list) {
				const Platform::CollisionShape& shape = platform.get_shape();
				// Xmax - Xmin <= 矩形宽度 + 线段长度 即意味着两个物体在x上有重合部分
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left) <= size.x + (shape.right - shape.left));
				bool is_collide_y = (position.y <= shape.y && shape.y <= position.y + size.y);

				if (is_collide_x && is_collide_y) {
					// 不能简单的仅从是否在x y碰撞了就判断需要把角色停在平面上,
					// 因为我们的角色可以从下方跳到平台上, 考虑这种情况: 玩家按下跳跃
					// 在接下来的几帧中, 角色的部分碰撞箱开始和平台碰撞箱发生重合, 如果仅仅从判断是否碰撞的话, 这个时候画面上角色就瞬间被移到了平台上, 这不符合我们的需求
					// 因此我们需要计算一下上一帧脚部的y坐标, 确保只有当y坐标在平台上方的时候才让角色停在平台上
					float delta_pos_y = velocity.y * delta;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
					if (last_tick_foot_pos_y <= shape.y) {
						position.y = shape.y - size.y;
						velocity.y = 0;

						break;
					}
				}
			}
		}

		if (!is_invulnerable) {
			for (Bullet* bullet : bullet_list) {
				if (!bullet->get_valid() || bullet->get_collide_target() != id) {
					continue;
				}

				if (bullet->check_collision(position, size)) {
					make_invulnerable();

					bullet->on_collide();
					bullet->set_valid(false);
					hp -= bullet->get_damage();
				}
			}
		}

	}

	void make_invulnerable() {
		is_invulnerable = true;
		timer_invulnerable.restart();
	}



protected:
	int mp = 0;
	int hp = 100;

	Vector2 position;
	Vector2 size; // 角色尺寸
	Vector2 velocity;
	int jump_time = 2; // 能连续跳的数量

	Animation animation_idle_left;
	Animation animation_idle_right;
	Animation animation_run_left;
	Animation animation_run_right;
	Animation animation_attack_ex_left;
	Animation animation_attack_ex_right;

	Animation* current_animation = nullptr;

	PlayerID id = PlayerID::P1;

	bool is_left_key_down = false;
	bool is_right_key_down = false;

	bool is_facing_right = true;

	bool can_attack = true; // 是否可以普通攻击
	int attack_cd = 500;
	Timer timer_attack_cd;

	bool is_attacking_ex = false; // 是否正在特殊攻击(有这个bool是因为我们希望放大招的时候角色不能随意的移动)

	bool is_invulnerable = false; // 角色是否处于无敌状态
	bool is_showing_sketch_frame = false; // 当前帧是否应该显示剪影

	Timer timer_invulnerable;
	Timer timer_invulnerable_blink;

	IMAGE img_sketch;
};

