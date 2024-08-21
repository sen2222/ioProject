#ifndef __IMG_H
#define __IMG_H

#include "main.h"

#define LCD_WIDTH 800	   // 液晶屏宽
#define LCD_HEIGHT 480	   // 液晶屏高
#define BMP_HEADER_SIZE 14 // 图片文件头大小
#define BMP_INFO_SIZE 40   // 图片信息头大小
struct bitmap_header
{
	int16_t type;
	int32_t size; // 图像文件大小
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits; // bmp图像数据偏移量
} __attribute__((packed));

struct bitmap_info
{
	int32_t size;	// 本结构大小
	int32_t width;	// 图像宽
	int32_t height; // 图像高
	int16_t planes;

	int16_t bit_count; // 色深
	int32_t compression;
	int32_t size_img; // bmp数据大小，必须是4的整数倍
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
} __attribute__((packed));

typedef struct
{
	int width;	// 宽
	int height; // 高
	int bpp;	// 色深
	int size;
	int bmpType; // 图片类型，0为bmp，1为rgb
	unsigned char *bm;
} img;

// 图片节点
typedef struct
{
	img *imgObject;
	struct list_head list;
} imgNode;

// 管理结构体
typedef struct
{
	imgNode *head; // 指向队头的指针
	int num;	   // 图片数目
} imgList;

// bmp图片方法
img *BMPObject(char *address);
void showBMP(char *p, int x, int y, int bmph, int bmpw, int stoph, img *bmp);
void LCDclear(char *p, int x, int y);
void freeBMP(img *p);
img *resizeBMP(img *src, int target_width, int target_height);
void showGallery(void);

#endif
