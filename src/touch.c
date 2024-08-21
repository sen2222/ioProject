#include "main.h"

int getDirection(void)
{
    int x1, y1;
    int x2, y2;
    int xPoor, yPoor;
    int level, vertical;

    bool x1done = false;
    bool y1done = false;

    struct input_event buf;
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(tp, &buf, sizeof(buf));

        // get first pos
        if (buf.type == EV_ABS && buf.code == ABS_X && !x1done)
        {
            x2 = x1 = buf.value;
            x1done = true;
        }
        if (buf.type == EV_ABS && buf.code == ABS_Y && !y1done)
        {
            y2 = y1 = buf.value;
            y1done = true;
        }

        if (buf.type == EV_ABS && buf.code == ABS_X)
            x2 = buf.value;
        if (buf.type == EV_ABS && buf.code == ABS_Y)
            y2 = buf.value;

        if (buf.type == EV_KEY && buf.code == BTN_TOUCH && buf.value == 0)
        {
            xPoor = x1 - x2;
            yPoor = y1 - y2;
            if (abs(xPoor) <= 50 && abs(yPoor) <= 50)
            {
                click[0] = x1 * 800 / 1024;
                click[1] = y1 * 480 / 600;
                return CLICK;
            }
            else
            {
                if (xPoor > 0)
                    level = LEFT;
                else
                    level = RIGHT;
                if (yPoor > 0)
                    vertical = UP;
                else
                    vertical = DOWN;

                if (abs(xPoor) > abs(yPoor))
                    return level;
                else
                    return vertical;
            }
        }
    }
}

/**
 * 函  数: 检查点击事件是哪个按钮
 * 参  数: buttons[] 按键位置数组
 * 参  数: 按键数量，也就是数组长度
 * 返回值: 对应位置下标
 */
int detectButton(Button buttons[], int size)
{
    // printf("x = %d, y = %d\n", click[0], click[1]);  
    for (int i = 0; i < size; i++)
    {
        if (click[0] >= buttons[i].x1 && click[0] <= buttons[i].x2 &&
            click[1] >= buttons[i].y1 && click[1] <= buttons[i].y2)
        {
            return i; // 返回按键编号
        }
    }
    return -1; // 没有匹配的按键
}
