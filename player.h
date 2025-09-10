#pragma once
#include "camera.h"
#include <graphics.h>
#include "vector2.h"
#include "animation.h"
#include "player_id.h"
#include "platform.h"
#include "timer.h"

#include <iostream>

extern std::vector<Platform> platform_list;

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
	}
	~Player() = default;

	virtual void on_update(int delta) {

		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0) {
			is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			float distance = direction * run_velocity * delta;
			on_run(distance);
		}
		else {
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
		}

		move_and_collide(delta);

		current_animation->on_update(delta);

		timer_attack_cd.on_update(delta);
 
	}

	virtual void on_draw(const Camera& camera){
		current_animation->on_draw(camera, (int)position.x, (int)position.y);

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
			// ֻҪ��ɫx��ƽ̨�غ��ҽ�ɫ�ŵ�y��ƽ̨��ͬ,����ζ���Ѿ����, ����������Ծ����
			bool is_collide_y = shape.y == position.y + size.y;
			if (is_collide_x && is_collide_y) {
				jump_time = 2;
			}
		}
		if (velocity.y <= -0.50 || jump_time == 0) {
			return;
		}

		// TODO �����Ժ�Ҫ���Ǽ�һ��һ����֮���ɫ���ɫ,��ʾ��Ҵ�ʱ�������Ƕ�����(�ο�ε��)
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

	// �����߼�
	void move_and_collide(int delta) {
		velocity.y += gravity * delta;
		position += velocity * (float)delta;

		// ����ƽ̨�ǵ���ɴ�����(���±߿��������ϱ�),�������ֻ��Ҫ������ٶ�����(v.y > 0)ʱ����Ƿ�����ƽ̨
		if (velocity.y > 0) {
			for (const Platform& platform : platform_list) {
				const Platform::CollisionShape& shape = platform.get_shape();
				// Xmax - Xmin <= ���ο�� + �߶γ��� ����ζ������������x�����غϲ���
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left) <= size.x + (shape.right - shape.left));
				bool is_collide_y = (position.y <= shape.y && shape.y <= position.y + size.y);

				if (is_collide_x && is_collide_y) {
					// ���ܼ򵥵Ľ����Ƿ���x y��ײ�˾��ж���Ҫ�ѽ�ɫͣ��ƽ����,
					// ��Ϊ���ǵĽ�ɫ���Դ��·�����ƽ̨��, �����������: ��Ұ�����Ծ
					// �ڽ������ļ�֡��, ��ɫ�Ĳ�����ײ�俪ʼ��ƽ̨��ײ�䷢���غ�, ����������ж��Ƿ���ײ�Ļ�, ���ʱ�����Ͻ�ɫ��˲�䱻�Ƶ���ƽ̨��, �ⲻ�������ǵ�����
					// ���������Ҫ����һ����һ֡�Ų���y����, ȷ��ֻ�е�y������ƽ̨�Ϸ���ʱ����ý�ɫͣ��ƽ̨��
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
	}

protected:
	int mp = 0;
	int hp = 100;

	Vector2 position;
	Vector2 size; // ��ɫ�ߴ�
	Vector2 velocity;
	int jump_time = 2; // ��������������

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

	bool can_attack = true; // �Ƿ������ͨ����
	int attack_cd = 500;
	Timer timer_attack_cd;

	bool is_attacking_ex = false; // �Ƿ��������⹥��(�����bool����Ϊ����ϣ���Ŵ��е�ʱ���ɫ����������ƶ�)
};

