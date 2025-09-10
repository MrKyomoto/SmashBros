#pragma once
#include "util.h"
#include "camera.h"

extern bool is_debug;

class Platform
{
public:
	struct CollisionShape{
		// 2Dƽ̨,��ҿ��Դ��·�ֱ�Ӵ���ƽ̨��������,��˼򵥵İ�ƽ̨����Ϊһ��ˮƽ�߼���,��ʱ�������ƽ̨����ײ��״����һ��y���� + ���ұ�Ե����
		float y;
		float left, right;
	};
public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) const {
		putimage_alpha(camera, render_position.x, render_position.y, img);

		if (is_debug) {
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);

		}
	}

	void set_shape(float y, float left, float right) {
		shape.y = y;
		shape.left = left;
		shape.right = right;
	}

	const POINT& get_position() const {
		return render_position;
	}

	void set_position(int x, int y) {
		render_position.x = x;
		render_position.y = y;
	}

	void set_img(IMAGE* img_platform) {
		img = img_platform;
	}

	const IMAGE* get_img() const {
		return img;
	}

private:
	// �����߼�����Ⱦ�Ƿ����
	CollisionShape shape = { 0,0,0 };  // �����߼�, ���ڼ����ײ

	IMAGE* img = nullptr; // ��Ⱦ�߼�, ������Ⱦƽ̨ͼ��
	POINT render_position = { 0 };
private:

};

