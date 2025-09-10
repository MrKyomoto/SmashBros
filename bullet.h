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
		// �����е��ж��������Ǻ��ϸ�,ֻ���ж����ӵ����ĵ��Ƿ�λ����Ҿ����ڲ�
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
	/// �ӵ�һ��3���׶�: ����״̬ -> ��Ч״̬ -> ���Ƴ�״̬
	/// ����״̬: ÿһ֡��Ҫ������ҵ���ײ
	/// ��Ч״̬: ���ټ�����ҵ���ײ,��������������Ϊ���鶯��
	/// ���Ƴ�״̬: ���鶯��������Ϻ������Ƴ�״̬, ������������ӵ�ʵ��
	bool valid = true; // �ӵ��Ƿ���Ч
	bool removable = false; // �ӵ��Ƿ���Ա��Ƴ�
	std::function<void()> callback; // �ӵ���ײ��Ļص�����
	PlayerID target_id = PlayerID::P1; // �ӵ���ײĿ����ҵ�ID

protected:
	bool check_if_out_of_screen() {
		return (position.x + size.x <= 0 || position.x >= getwidth() || position.y + size.y <= 0 || position.y >= getheight());
	}
};

