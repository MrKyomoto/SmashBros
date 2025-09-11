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

inline void putimage_alpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y) {
	int w = width > 0 ? width : img->getwidth();
	int h = height > 0 ? height : img->getheight();
	// ������������putimage�����ص�,ֻ������������Ƶ��Ǵ�alphaͨ����
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

// ��ӵ� util.h ��
inline void white_overlay_image(IMAGE* src, IMAGE* dst, int alpha = 99) {
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h); 

	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);

	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int index = y * w + x;
			DWORD src_pixel = src_buffer[index];  // Դ���أ���ʽ��0xAARRGGBB��

			// ��ȡԴ���ص� alpha ͨ������8λ��
			BYTE src_alpha = (src_pixel >> 24) & 0xFF;
			if (src_alpha == 0) {
				// ��ȫ͸��������ֱ�ӱ�����������
				dst_buffer[index] = src_pixel;
				continue;
			}

			// ������Ӻ����ɫ����ɫ (255,255,255) ��ԭͼ��ɫ���
			// ��ʽ������ɫ = ��ɫ * ����͸���� + ԭͼ��ɫ * (1 - ����͸����)
			BYTE red = 255 * alpha / 255 + ((src_pixel >> 16) & 0xFF) * (255 - alpha) / 255;
			BYTE green = 255 * alpha / 255 + ((src_pixel >> 8) & 0xFF) * (255 - alpha) / 255;
			BYTE blue = 255 * alpha / 255 + (src_pixel & 0xFF) * (255 - alpha) / 255;

			// ��������أ�����ԭͼ alpha ͨ����
			dst_buffer[index] = (src_alpha << 24) | (red << 16) | (green << 8) | blue;
		}
	}
}
