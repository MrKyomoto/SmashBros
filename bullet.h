#pragma once
#include "vector2.h"
#include <functional>
#include "camera.h"

#include "player_id.h"
#include <graphics.h>

class Bullet
{
public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int val) {
		damage = val;
	}
	
	int get_damage() {
		return damage;
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

	void set_velocity(float x, float y) {
		velocity.x = x;
		velocity.y = y;
	}

	void set_collide_target(PlayerID target) {
		target_id = target;
	}

	PlayerID get_collide_target() const {
		return target_id;
	}

	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}

	void set_valid(bool flag) {
		valid = flag;
	}
	
	bool get_valid() const {
		return valid;
	}

	bool check_removable() const {
		return removable;
	}

	virtual void on_collide() {
		if (callback) {
			callback();
		}
	}
	virtual bool check_collision(const Vector2& position, const Vector2& size) {
		// 基类中的判断条件不是很严格,只是判断了子弹中心点是否位于玩家矩形内部
		return this->position.x + this->size.x / 2 >= position.x 
			&& this->position.x + this->size.x / 2 <= position.x + size.x 
			&& this->position.y + this->size.y / 2 >= position.y 
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}
	virtual void on_update(int delta){ }
	virtual void on_draw(const Camera& camera) const { }

protected:
	Vector2 size;
	Vector2 position;
	Vector2 velocity;
	int damage = 10;
	/// 子弹一共3个阶段: 正常状态 -> 无效状态 -> 可移除状态
	/// 正常状态: 每一帧都要检测和玩家的碰撞
	/// 无效状态: 不再检测和玩家的碰撞,并从正常动画变为破碎动画
	/// 可移除状态: 破碎动画播放完毕后进入可移除状态, 可以销毁这个子弹实例
	bool valid = true; // 子弹是否有效
	bool removable = false; // 子弹是否可以被移除
	std::function<void()> callback; // 子弹碰撞后的回调函数
	PlayerID target_id = PlayerID::P1; // 子弹碰撞目标玩家的ID

protected:
	bool check_if_out_of_screen() {
		return (position.x + size.x <= 0 || position.x >= getwidth() || position.y + size.y <= 0 || position.y >= getheight());
	}
};

