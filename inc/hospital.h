#ifndef __HOSPITAL_H
#define __HOSPITAL_H

typedef struct
{
    int doctrIndex;        //对应医生下标
    char name[15];
    char sex[3]; // 性别
} __attribute__((packed)) patient;

typedef struct pitNode
{
    patient pit; // 病人结构体
    struct pitNode *next;
} pitNode;

typedef struct
{
    int size;       // 数量
    pitNode *front; // 队头
    pitNode *rear;  // 队尾
} Queue;

typedef struct
{
    unsigned char id; // 医生id
    char name[15];    // 名字
    char job[20];     // 职称
    char bio[512];    // 简介
    img *dtimg;       // 图片
    Queue *qu;        // 病人队列
    Queue *overqu;    // 过号队列
} doctor;

void Hospital(void);

#endif