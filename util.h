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
	// 这个函数本身和putimage作用重叠,只是这个函数绘制的是带alpha通道的
	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, width, height,
		GetImageHDC(img), 0, 0, width, height, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

inline void putimage_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img) {
	int width = img->getwidth();
	int height = img->getheight();
	const Vector2& pos_camera = camera.get_position();
	// 这个函数本身和putimage作用重叠,只是这个函数绘制的是带alpha通道的
	AlphaBlend(GetImageHDC(GetWorkingImage()), (int)dst_x - pos_camera.x, (int)dst_y - pos_camera.y, width, height,
		GetImageHDC(img), 0, 0, width, height, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

inline void putimage_alpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y) {
	int w = width > 0 ? width : img->getwidth();
	int h = height > 0 ? height : img->getheight();
	// 这个函数本身和putimage作用重叠,只是这个函数绘制的是带alpha通道的
	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h,
		GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2) {
	const Vector2& pos_camera = camera.get_position();
	line((int)(x1 - pos_camera.x), (int)(y1 - pos_camera.y), (int)(x2 - pos_camera.x), (int)(y2 - pos_camera.y));
}

inline void sketch_image(IMAGE* src, IMAGE* dst) {
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int index = y * w + x;
			dst_buffer[index] = BGR(RGB(255, 255, 255)) | (src_buffer[index] & 0xFF000000);
		}
	}
}

// 添加到 util.h 中
inline void white_overlay_image(IMAGE* src, IMAGE* dst, int alpha = 99) {
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h); 

	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int index = y * w + x;
			DWORD src_pixel = src_buffer[index];  // 源像素（格式：0xAARRGGBB）

			// 提取源像素的 alpha 通道（高8位）
			BYTE src_alpha = (src_pixel >> 24) & 0xFF;
			if (src_alpha == 0) {
				// 完全透明的像素直接保留（不处理）
				dst_buffer[index] = src_pixel;
				continue;
			}

			// 计算叠加后的颜色：白色 (255,255,255) 与原图颜色混合
			// 公式：新颜色 = 白色 * 叠加透明度 + 原图颜色 * (1 - 叠加透明度)
			BYTE red = 255 * alpha / 255 + ((src_pixel >> 16) & 0xFF) * (255 - alpha) / 255;
			BYTE green = 255 * alpha / 255 + ((src_pixel >> 8) & 0xFF) * (255 - alpha) / 255;
			BYTE blue = 255 * alpha / 255 + (src_pixel & 0xFF) * (255 - alpha) / 255;

			// 组合新像素（保留原图 alpha 通道）
			dst_buffer[index] = (src_alpha << 24) | (red << 16) | (green << 8) | blue;
		}
	}
}
