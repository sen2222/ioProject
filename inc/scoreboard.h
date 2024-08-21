#ifndef __SCOREBOARD_H
#define __SCOREBOARD_H

// 球员结构体
typedef struct player
{
    char name[30];
    unsigned char num;   // 号码
    unsigned char score; // 得分
} player;

// 球队结构体
typedef struct team
{
    char name[30];         // 球队名称
    unsigned char scores;     // 球队得分
    unsigned char foul;    // 球队犯规数
    unsigned char pause;   // 剩余暂停数
    player players[12]; // 球队成员
}team;

// 控制倒计时线程的状态
typedef struct {
    int minutes;          // 分钟数
    int seconds;          // 秒数
    bool stop;            // 是否停止
    int section;          //节数
}Time;
void scoreBoard(void);
#endif
