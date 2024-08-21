#ifndef __TOUCH_H
#define __TOUCH_H

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define CLICK 4
int click[2];

typedef struct {
    int x1, y1; // 左上角坐标
    int x2, y2; // 右下角坐标
} Button;


int getDirection(void);
int detectButton(Button buttons[], int size);

#endif
