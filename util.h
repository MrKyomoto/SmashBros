#pragma once
#include <graphics.h>
#include "camera.h"

#pragma comment(lib,"MSIMG32.LIB")

inline void flip_image(IMAGE* src, IMAGE* dst) {
	int width = src->getwidth();
	int height = src->getheight();
	Resize(dst, width, height);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int idx_src = y * width + x;
			int idx_dst = y * width + width - x - 1;
			dst_buffer[idx_dst] = src_buffer[idx_src];
		}
	}
}

inline void putimage_alpha(int dst_x, int dst_y, IMAGE* img) {
	int width = img->getwidth();
	int height = img->getheight();
	// ������������putimage�����ص�,ֻ������������Ƶ��Ǵ�alphaͨ����
	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, width, height,
		GetImageHDC(img), 0, 0, width, height, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

inline void putimage_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img) {
	int width = img->getwidth();
	int height = img->getheight();
	const Vector2& pos_camera = camera.get_position();
	// ������������putimage�����ص�,ֻ������������Ƶ��Ǵ�alphaͨ����
	AlphaBlend(GetImageHDC(GetWorkingImage()), (int)dst_x - pos_camera.x, (int)dst_y - pos_camera.y, width, height,
		GetImageHDC(img), 0, 0, width, height, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
