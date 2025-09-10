#pragma once
#include "util.h"
#include "camera.h"

class Platform
{
public:
	struct CollisionShape{
		// 2D平台,玩家可以从下方直接穿过平台跳到上面,因此简单的把平台抽象为一条水平线即可,此时描述这个平台的碰撞形状就是一个y坐标 + 左右边缘即可
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
	// 数据逻辑和渲染是分离的
	CollisionShape shape;  // 数据逻辑, 用于检测碰撞

	IMAGE* img = nullptr; // 渲染逻辑, 用于渲染平台图像
	POINT render_position = { 0 };
private:

};

