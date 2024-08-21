#include "main.h"
#include <pthread.h>
team *Ateam, *Bteam; // 两支球队
Time *Countdown;

/**
 * 函  数: 初始化球队信息
 * 参  数: 无
 * 返回值: 无
 */
void dataInit(void)
{
    Ateam = (team *)malloc(sizeof(team));
    Bteam = (team *)malloc(sizeof(team));
    if (Ateam == NULL || Bteam == NULL)
    {
        HANDLE_ERROR(strerror(errno));
        return;
    }

    // 球队名字
    strcpy(Ateam->name, "广东队");
    strcpy(Bteam->name, "江西队");

    // 球队得分
    Ateam->scores = 0;
    Bteam->scores = 0;

    // 球队犯规
    Ateam->foul = 0;
    Bteam->foul = 0;

    // 剩余暂停数
    Ateam->pause = 5;
    Bteam->pause = 5;

    // 初始化广东队球员
    strcpy(Ateam->players[0].name, "周琦");
    Ateam->players[0].num = 0;
    Ateam->players[0].score = 0;

    strcpy(Ateam->players[1].name, "胡金秋");
    Ateam->players[1].num = 3;
    Ateam->players[1].score = 0;

    strcpy(Ateam->players[2].name, "赵继伟");
    Ateam->players[2].num = 5;
    Ateam->players[2].score = 0;

    strcpy(Ateam->players[3].name, "张镇麟");
    Ateam->players[3].num = 7;
    Ateam->players[3].score = 0;

    strcpy(Ateam->players[4].name, "赵睿");
    Ateam->players[4].num = 8;
    Ateam->players[4].score = 0;

    strcpy(Ateam->players[5].name, "胡明轩");
    Ateam->players[5].num = 9;
    Ateam->players[5].score = 0;

    strcpy(Ateam->players[6].name, "王哲林");
    Ateam->players[6].num = 10;
    Ateam->players[6].score = 0;

    strcpy(Ateam->players[7].name, "崔永熙");
    Ateam->players[7].num = 11;
    Ateam->players[7].score = 0;

    strcpy(Ateam->players[8].name, "孙铭徽");
    Ateam->players[8].num = 20;
    Ateam->players[8].score = 0;

    strcpy(Ateam->players[9].name, "吴前");
    Ateam->players[9].num = 22;
    Ateam->players[9].score = 0;

    strcpy(Ateam->players[10].name, "郭艾伦");
    Ateam->players[10].num = 32;
    Ateam->players[10].score = 0;

    strcpy(Ateam->players[11].name, "沈梓捷");
    Ateam->players[11].num = 33;
    Ateam->players[11].score = 0;

    // 初始化江西队球员
    strcpy(Bteam->players[0].name, "易建联");
    Bteam->players[0].num = 1;
    Bteam->players[0].score = 0;

    strcpy(Bteam->players[1].name, "周鹏");
    Bteam->players[1].num = 3;
    Bteam->players[1].score = 0;

    strcpy(Bteam->players[2].name, "丁彦雨航");
    Bteam->players[2].num = 5;
    Bteam->players[2].score = 0;

    strcpy(Bteam->players[3].name, "阿不都沙");
    Bteam->players[3].num = 6;
    Bteam->players[3].score = 0;

    strcpy(Bteam->players[4].name, "李根");
    Bteam->players[4].num = 7;
    Bteam->players[4].score = 0;

    strcpy(Bteam->players[5].name, "方硕");
    Bteam->players[5].num = 8;
    Bteam->players[5].score = 0;

    strcpy(Bteam->players[6].name, "陈林坚");
    Bteam->players[6].num = 9;
    Bteam->players[6].score = 0;

    strcpy(Bteam->players[7].name, "翟晓川");
    Bteam->players[7].num = 10;
    Bteam->players[7].score = 0;

    strcpy(Bteam->players[8].name, "郭昊文");
    Bteam->players[8].num = 11;
    Bteam->players[8].score = 0;

    strcpy(Bteam->players[9].name, "韩德君");
    Bteam->players[9].num = 21;
    Bteam->players[9].score = 0;

    strcpy(Bteam->players[10].name, "可兰白克");
    Bteam->players[10].num = 23;
    Bteam->players[10].score = 0;

    strcpy(Bteam->players[11].name, "赵岩昊");
    Bteam->players[11].num = 32;
    Bteam->players[11].score = 0;
}

/**
 * 函  数: 打印初始信息
 * 参  数: 无
 * 返回值: 无
 */
void showData(void)
{
    Lcd_ShowFont(280, 40, 0, 45, getColor(0, 255, 255, 255), 0, 0, 255, 30, "2024年 CBA常规赛");
    Lcd_ShowFont(100, 40, 0, 45, getColor(0, 0, 0, 238), 0, 0, 255, 90, Ateam->name);
    Lcd_ShowFont(100, 40, 0, 45, getColor(0, 238, 0, 0), 0, 0, 425, 90, Bteam->name);
    int i = 0;
    char strtmp[30];
    for (; i < 12; i++)
    {
        // 打印球员得分、犯规数
        // 名字
        snprintf(strtmp, 30, " %d  %s", Ateam->players[i].num, Ateam->players[i].name);
        Lcd_ShowFont(150, 35, 0, 40, getColor(0, 0, 0, 238), 0, 0, 0, i * 40, strtmp);
        // 分数
        snprintf(strtmp, 5, " %u", Ateam->players[i].score);
        Lcd_ShowFont(50, 30, 0, 40, getColor(0, 255, 255, 255), 0, 0, 150, i * 40, strtmp);

        // 名字
        Lcd_ShowFont(180, 35, 0, 40, getColor(0, 238, 0, 0), 0, 0, 630, i * 40, Bteam->players[i].name);
        snprintf(strtmp, 5, " %u", Ateam->players[i].num);
        Lcd_ShowFont(50, 30, 0, 40, getColor(0, 238, 0, 0), 0, 0, 760, i * 40, strtmp);
        // 分数
        snprintf(strtmp, 30, " %u", Bteam->players[i].score);
        Lcd_ShowFont(50, 30, 0, 40, getColor(0, 255, 255, 255), 0, 0, 575, i * 40, strtmp);
    }
    // 比分
    snprintf(strtmp, 5, " %u", Ateam->scores);
    Lcd_ShowFont(100, 80, 0, 100, getColor(0, 255, 48, 48), -15, -10, 265, 160, strtmp);
    snprintf(strtmp, 5, " %u", Bteam->scores);
    Lcd_ShowFont(100, 80, 0, 100, getColor(0, 255, 48, 48), -15, -10, 425, 160, strtmp);

    // 时间
    Lcd_ShowFont(200, 100, 0, 100, getColor(0, 255, 255, 255), 0, 0, 290, 230, "12:00");
    Lcd_ShowFont(130, 60, 0, 60, getColor(0, 0, 255, 0), 0, 0, 320, 330, "第一节");

    // 犯规数
    snprintf(strtmp, 5, " %u", Ateam->foul);
    Lcd_ShowFont(80, 50, 0, 60, getColor(0, 255, 255, 0), 0, 0, 275, 380, strtmp);
    snprintf(strtmp, 5, " %u", Bteam->foul);
    Lcd_ShowFont(80, 50, 0, 60, getColor(0, 255, 255, 0), 0, 0, 455, 380, strtmp);

    // 犯规字样
    bitmap *bm = createBitmapWithInit(80, 50, 4, 0);
    fontSetSize(f, 40);
    fontPrint(f, bm, 0, 0, "犯规", getColor(0, 255, 255, 0), 0);
    show_font_to_lcd(LCD_Font->mp, 455, 430, bm);
    show_font_to_lcd(LCD_Font->mp, 275, 430, bm);
    destroyBitmap(bm);
}
/**
 * 函  数: 更新倒计时信息
 * 参  数: minutes 分钟
 * 返回值: 无
 */
void updateTime(int minutes)
{
    Countdown->minutes = minutes;
    Countdown->seconds = 0;
}
/**
 * 函  数: 倒计时线程函数
 * 参  数: 无
 * 返回值: 无
 */
bool timeFlag;
void *countdownTime(void *arg)
{
    char timestr[15]; // 用于存储格式化的时间字符串
    char *str[] = {"一", "二", "三", "四"};
    while (timeFlag)
    {
        while (Countdown->stop && timeFlag)
            usleep(10000);

        sleep(1); // 等待一秒

        if (Countdown->seconds > 0)
        {
            Countdown->seconds--; // 减少秒数
        }
        else if (Countdown->minutes > 0)
        {
            Countdown->minutes--; // 减少分钟数并重置秒数
            Countdown->seconds = 59;
        }
        else
        {
            updateTime(12);
            Countdown->section++;
            // 四节结束，结束倒计时
            if (Countdown->section > 4)
            {
                *((int *)arg) = 0;
                img *imgtmp = BMPObject("../rec/img/win.bmp");
                showBMP(LCD, 240, 160, 0, 0, 480, imgtmp);
                freeBMP(imgtmp);
                snprintf(timestr, sizeof(timestr), "%s", (Ateam->scores > Bteam->scores) ? Ateam->name : Bteam->name);
                Lcd_ShowFont(160, 80, getColor(0, 255, 255, 255), 70, getColor(0, 255, 0, 0), 0, 0, 310, 160, timestr);
                break;
            }
            // 下一节
            snprintf(timestr, sizeof(timestr), "第%s节", str[Countdown->section - 1]);
            Lcd_ShowFont(130, 60, 0, 60, getColor(0, 0, 255, 0), 0, 0, 320, 330, timestr);
            Countdown->stop = 1;
        }
        // 格式化并显示时间
        snprintf(timestr, sizeof(timestr), "%02d:%02d", Countdown->minutes, Countdown->seconds);
        Lcd_ShowFont(200, 100, 0, 100, getColor(0, 255, 255, 255), 0, 0, 290, 230, timestr);
    }

    pthread_exit(NULL); // 线程结束时调用
}

/**
 * 函  数: 更新得分信息
 * 参  数: flag 1代表A队, 0代表B队
 * 参  数: index 得分的球员下标
 * 返回值: 无
 */
void updateScore(bool flag, int index)
{
    char strtmp[30];
    if (flag)
    {
        Ateam->scores++;
        Ateam->players[index].score++;
        snprintf(strtmp, 5, " %u", Ateam->scores);
        Lcd_ShowFont(100, 80, 0, 100, getColor(0, 255, 48, 48), -15, -10, 265, 160, strtmp);
        // 分数
        snprintf(strtmp, 5, " %u", Ateam->players[index].score);
        Lcd_ShowFont(50, 30, 0, 40, getColor(0, 255, 255, 255), 0, 0, 150, index * 40, strtmp);
    }
    else
    {
        Bteam->scores++;
        Bteam->players[index].score++;
        snprintf(strtmp, 5, " %u", Bteam->scores);
        Lcd_ShowFont(100, 80, 0, 100, getColor(0, 255, 48, 48), -15, -10, 425, 160, strtmp);
        // 分数
        snprintf(strtmp, 30, " %u", Bteam->players[index].score);
        Lcd_ShowFont(50, 30, 0, 40, getColor(0, 255, 255, 255), 0, 0, 575, index * 40, strtmp);
    }
}
/**
 * 函  数: 释放所有空间
 * 参  数: 无
 * 返回值: 无
 */
void freeTeam(void)
{
    free(Ateam);     // 释放A队
    free(Bteam);     // 释放B队
    free(Countdown); // 释放时间结构体
}
/**
 * 函  数: 计分板模块
 * 参  数: 无
 * 返回值: 无
 */
void scoreBoard(void)
{
    // 初始化球队数据
    dataInit();
    // 按键序列
    Button buttons[] =
        {
            // A队队员按键
            {0, 0, 200, 40},    // 0
            {0, 40, 200, 80},   // 1
            {0, 80, 200, 120},  // 2
            {0, 120, 200, 160}, // 3
            {0, 160, 200, 200}, // 4
            {0, 200, 200, 240}, // 5
            {0, 240, 200, 280}, // 6
            {0, 280, 200, 320}, // 7
            {0, 320, 200, 360}, // 8
            {0, 360, 200, 400}, // 9
            {0, 400, 200, 440}, // 10
            {0, 440, 200, 480}, // 11

            // B队员按键
            {630, 0, 800, 40},    // 0
            {630, 40, 800, 80},   // 1
            {630, 80, 800, 120},  // 2
            {630, 120, 800, 160}, // 3
            {630, 160, 800, 200}, // 4
            {630, 200, 800, 240}, // 5
            {630, 240, 800, 280}, // 6
            {630, 280, 800, 320}, // 7
            {630, 320, 800, 360}, // 8
            {630, 360, 800, 400}, // 9
            {630, 400, 800, 440}, // 10
            {630, 440, 800, 480}, // 11

            {290, 230, 490, 330}, // 时间暂停
            {275, 380, 355, 480}, // A队犯规
            {455, 380, 535, 480}, // B队犯规
            {255, 30, 535, 70},   // 退出
            {320, 330, 450, 390}

        };
    memset(LCD, 0, 800 * 480 * 4); // 涂黑屏幕

    // 时间模块
    Countdown = (Time *)malloc(sizeof(Time));
    Countdown->stop = 1;
    Countdown->section = 1;
    updateTime(12);

    timeFlag = 1;
    pthread_t thread, thread2;
    int arg = 3; // 传递给线程的参数
    tPthread = 1;
    // 创建线程
    if (pthread_create(&thread, NULL, countdownTime, &arg) != 0 || pthread_create(&thread2, NULL, waterThreadFunction, &arg))
        HANDLE_ERROR(strerror(errno));

    // 打印标题
    showData();
    int tmp;
    while (arg)
    {
        // 获取按键输入
        while (getDirection() != CLICK)
            ;
        tmp = detectButton(buttons, 29);
        // A队队员得分
        if (tmp >= 0 && tmp <= 11)
        {
            updateScore(1, tmp);
        }
        else if (tmp >= 12 && tmp <= 23)
        {
            updateScore(0, tmp - 12);
        }
        else if (tmp == 24)
        {
            Countdown->stop = !Countdown->stop;
        }
        else if (tmp == 25) // A队犯规
        {
            char strtmp[4];
            snprintf(strtmp, 5, " %u", ++Ateam->foul);
            Lcd_ShowFont(80, 50, 0, 60, getColor(0, 255, 255, 0), 0, 0, 275, 380, strtmp);
        }
        else if (tmp == 26)
        {
            char strtmp[4];
            snprintf(strtmp, 5, " %u", ++Bteam->foul);
            Lcd_ShowFont(80, 50, 0, 60, getColor(0, 255, 255, 0), 0, 0, 455, 380, strtmp);
        }
        else if (tmp == 27)
            break;
        else if (tmp == 28)
            updateTime(Countdown->minutes - 1);
    }
    timeFlag = 0;
    tPthread = 0;
    pthread_join(thread2, NULL);
    pthread_join(thread, NULL);
    freeTeam();
}