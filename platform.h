#pragma once
#include "util.h"
#include "camera.h"

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
	}

public:
	// �����߼�����Ⱦ�Ƿ����
	CollisionShape shape;  // �����߼�, ���ڼ����ײ

	IMAGE* img = nullptr; // ��Ⱦ�߼�, ������Ⱦƽ̨ͼ��
	POINT render_position = { 0 };
private:

};

