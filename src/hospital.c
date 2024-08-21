#include "main.h"
#include <pthread.h>

/*************************************************队列处理*************************************************************/
int i = 0;          // 当前医生下标
doctor *doctors[6]; // 医生数组
pitNode *pits[6];   // 医生正在看诊的病人

/**
 * 函  数: 初始化病人队列
 * 参  数: 无
 * 返回值: 队列管理结构体指针
 */
Queue *newQueue(void)
{
    Queue *qu = (Queue *)malloc(sizeof(Queue));
    if (qu != NULL)
    {
        qu->size = 0;
        qu->rear = NULL;
        qu->front = NULL;
    }
    return qu;
}

/**
 * 函  数: 创建病人节点
 * 参  数: pit 病人信息结构体指针
 * 返回值: 病人节点指针
 */
pitNode *newPitNode(patient *pit)
{
    pitNode *newNode = (pitNode *)malloc(sizeof(pitNode));
    if (newNode != NULL)
    {
        strcpy(newNode->pit.name, pit->name);
        strcpy(newNode->pit.sex, pit->sex);
        newNode->next = NULL;
    }
    return newNode;
}
/**
 * 函  数: 病人节点入队操作
 * 参  数: qu 所要入的队列
 * 参  数：newNode要入队的节点
 * 返回值: 无
 */
void enQueue(Queue *qu, pitNode *newNode)
{
    // 队列为空
    if (qu->size == 0)
    {
        qu->front = qu->rear = newNode;
    }
    // 队列不为空，插入到队尾
    else
    {
        qu->rear->next = newNode;
        qu->rear = newNode;
    }
    qu->size++;
}
/**
 * 函  数: 病人节点出队队操作
 * 参  数: qu 所要出的队列
 * 返回值: 出队的节点的指针
 */
pitNode *outQueue(Queue *qu)
{
    pitNode *tmp = NULL;
    // 队列不为空
    if (qu->size != 0)
    {
        tmp = qu->front;
        qu->front = qu->front->next;
        tmp->next = NULL;
        qu->size--;
    }
    return tmp;
}
/**
 * 函  数: 查找从队头数过去第n个节点
 * 参  数: n 要拿的节点
 * 返回值: 拿到的节点
 * 注意 这个函数不会出队，只是单纯的拿某个节点
 */
pitNode *getNode(Queue *qu, int n)
{
    pitNode *tmp = qu->front;
    while (n-- > 0 && tmp != NULL)
        tmp = tmp->next;
    return tmp;
}
/**
 * 函  数: 某个节点出队操作
 * 参  数: n 要拿的节点
 * 返回值: 拿到的节点
 * 注意 这个函数会出队
 */
pitNode *getOutNode(Queue *qu, int n)
{

    pitNode *tmp = qu->front, *outNode;
    if (n == 0)
    {
        qu->front = qu->front->next;
        qu->size--;
        tmp->next = NULL;
        // 只有一个节点的情况，把尾巴指针置空
        if (qu->size == 0)
            qu->rear = NULL;
        return tmp;
    }
    while (n-- > 1 && tmp != NULL)
        tmp = tmp->next;
    outNode = tmp->next;
    tmp->next = outNode->next;
    outNode->next = NULL;
    qu->size--;
    return outNode;
}
/**
 * 函  数: 释放队列
 * 参  数: 队列管理节点
 * 返回值: 无
 */
void freeQueue(Queue *qu)
{
    pitNode *tmp;
    while ((tmp = outQueue(qu)) != NULL)
        free(tmp);
    free(qu);
}

/***************************************************主要功能实现******************************************************/
/**
 * 函  数: 建立医生数组
 * 参  数: 无
 * 返回值: 无
 */
void readDoctorsFromFile(void)
{
    FILE *file = fopen("../docs/doctorData", "r");
    if (file == NULL)
    {
        HANDLE_ERROR("无法打开文件");
        return;
    }

    char buffer[512];
    char imgName[1024] = {0};
    for (unsigned char i = 0; i < 6; i++)
    {
        doctors[i] = (doctor *)malloc(sizeof(doctor));

        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "%hhu", &(doctors[i]->id)); // 读取医生ID

        fgets(doctors[i]->name, sizeof(doctors[i]->name), file);
        doctors[i]->name[strcspn(doctors[i]->name, "\n")] = '\0'; // 去掉换行符

        fgets(doctors[i]->job, sizeof(doctors[i]->job), file);
        doctors[i]->job[strcspn(doctors[i]->job, "\n")] = '\0'; // 去掉换行符

        fgets(doctors[i]->bio, sizeof(doctors[i]->bio), file);
        doctors[i]->bio[strcspn(doctors[i]->bio, "\n")] = '\0'; // 去掉换行符

        snprintf(imgName, sizeof(imgName), "../rec/img/hospital/doctor%d.bmp", i); // 拼接医生图片地址

        doctors[i]->dtimg = BMPObject(imgName);
        doctors[i]->qu = newQueue();
        doctors[i]->overqu = newQueue();
    }

    fclose(file);
}

/**
 * 函  数: 打印医生信息
 * 参  数: 无
 * 返回值: 无
 */
void showDoctors(void)
{
    // 医生图片
    showBMP(LCD, 15, 80, 0, 0, 480, doctors[i]->dtimg);

    // 医生名字
    Lcd_ShowFont(85, 30, getColor(0, 255, 255, 255), 25, getColor(0, 0, 134, 139), 5, 3, 60, 245, doctors[i]->name);

    // 医生所属科
    Lcd_ShowFont(85, 30, getColor(0, 255, 255, 255), 25, getColor(0, 0, 134, 139), 5, 3, 60, 290, doctors[i]->job);

    // 显示医生简介
    bitmap *bm = createBitmapWithInit(450, 80, 4, getColor(0, 255, 255, 255)); // 背景色为白色
    fontSetSize(f, 20);
    fontPrint(f, bm, 5, 5, doctors[i]->bio, getColor(0, 0, 134, 139), 440);
    show_font_to_lcd(LCD_Font->mp, 10, 360, bm);
    destroyBitmap(bm);
}

/**
 * 函  数: 打印医生队列情况
 * 参  数: 无
 * 返回值: 无
 */
void showQueue()
{
    // 没病人的时候打印白板上去
    if (doctors[i]->qu->size == 0)
    {
        Lcd_ShowFont(260, 170, getColor(0, 255, 255, 255), 25, 0, 0, 0, 530, 80, "");
    }
    char datatmp[35] = {0};
    pitNode *tmp = doctors[i]->qu->front;
    int j = 0;

    // 显示前5个病人
    for (; tmp != NULL && j < 5; j++)
    {

        snprintf(datatmp, 30, "%d. %s", j + 1, tmp->pit.name);
        // printf("%s\n", datatmp);
        Lcd_ShowFont(110, 35, getColor(0, 255, 255, 255), 25, 0, 5, 3, 530, 80 + (35 * j), datatmp);
        tmp = tmp->next;
    }

    // 显示接下来的5个病人
    for (int r = 0; tmp != NULL && j < 10; r++, j++)
    {
        snprintf(datatmp, 30, "%d. %s", j + 1, tmp->pit.name);
        // printf("%s\n", datatmp);
        Lcd_ShowFont(110, 35, getColor(0, 255, 255, 255), 25, 0, 5, 3, 675, 80 + (35 * r), datatmp);
        tmp = tmp->next;
    }

    // 清空后续可能的多余显示
    for (int r = doctors[i]->qu->size < 5 ? doctors[i]->qu->size : 5; r < 5; r++)
    {
        Lcd_ShowFont(110, 35, getColor(0, 255, 255, 255), 25, 0, 5, 3, 530, 80 + (35 * r), "");
    }
    for (int r = doctors[i]->qu->size < 10 ? (doctors[i]->qu->size - 5) : 5; r < 5 && doctors[i]->qu->size >= 5; r++)
    {
        Lcd_ShowFont(110, 35, getColor(0, 255, 255, 255), 25, 0, 5, 3, 675, 80 + (35 * r), "");
    }
}
/**
 * 函  数: 打印医生看诊中的病人
 * 参  数: 无
 * 返回值: 无
 */
void showed(void)
{
    char tmp[50];
    unsigned int color;
    for (int j = 0; j < 6; j++)
    {
        color = (i == j) ? getColor(0, 0, 134, 139) : 0;
        if (pits[j] == NULL)
        {
            snprintf(tmp, 50, "%s:  空闲中", doctors[j]->name, pits[j]->pit.name);
            Lcd_ShowFont(285, 30, getColor(0, 255, 255, 255), 25, color, 0, 0, 170, 125 + (30 * j), tmp);
            continue;
        }
        snprintf(tmp, 50, "%s:  %s正在看诊", doctors[j]->name, pits[j]->pit.name);
        Lcd_ShowFont(285, 30, getColor(0, 255, 255, 255), 25, color, 0, 0, 170, 125 + (30 * j), tmp);
    }
}

/**
 * 函  数: 打印医生过号的队列情况
 * 参  数: 无
 * 返回值: 无
 */
void showOver(void)
{
    // 打印左边过号
    int j = 0;
    unsigned int color = getColor(0, 0, 191, 255);
    for (; j < doctors[i]->overqu->size && j < 3; j++)
    {
        Lcd_ShowFont(110, 35, color, 25, 0, 30, 5, 530, 270 + (60 * j),
                     getNode(doctors[i]->overqu, j)->pit.name);
    }
    // 打印右边过号
    for (int r = 0; j < doctors[i]->overqu->size; j++, r++)
    {
        Lcd_ShowFont(110, 35, getColor(0, 0, 191, 255), 25, 0, 30, 5, 670, 270 + (60 * r),
                     getNode(doctors[i]->overqu, j)->pit.name);
    }
}

void testData(void)
{
    patient a1 = {1, "陈森", "男"};
    patient b1 = {1, "陈培鑫", "女"};
    patient c1 = {1, "张涌涛", "男"};
    patient d1 = {1, "张三", "男"};
    patient e1 = {1, "李四", "女"};
    patient f1 = {1, "王五", "男"};

    patient a11 = {2, "赵六", "男"};
    patient b11 = {2, "吴七", "女"};
    patient c11 = {2, "周八", "男"};
    patient d11 = {2, "刘七", "男"};
    patient e11 = {2, "奥特曼", "女"};
    patient f11 = {2, "周杰伦", "男"};

    patient a12 = {2, "盖亚", "男"};
    patient b12 = {2, "迪迦", "女"};
    patient c12 = {2, "赛尔", "男"};
    patient d12 = {2, "曾俊举", "男"};
    patient e12 = {2, "陈景俊", "女"};
    patient f12 = {2, "卜伟好", "男"};
    enQueue(doctors[i]->qu, newPitNode(&a1));
    enQueue(doctors[i]->qu, newPitNode(&b1));
    enQueue(doctors[i]->qu, newPitNode(&c1));
    enQueue(doctors[i]->qu, newPitNode(&d1));
    enQueue(doctors[i]->qu, newPitNode(&e1));
    enQueue(doctors[i]->qu, newPitNode(&f1));

    enQueue(doctors[i + 1]->qu, newPitNode(&a11));
    enQueue(doctors[i + 1]->qu, newPitNode(&b11));
    enQueue(doctors[i + 1]->qu, newPitNode(&c11));
    enQueue(doctors[i + 1]->qu, newPitNode(&d11));
    enQueue(doctors[i + 1]->qu, newPitNode(&e11));
    enQueue(doctors[i + 1]->qu, newPitNode(&f11));

    enQueue(doctors[i + 1]->qu, newPitNode(&a12));
    enQueue(doctors[i + 1]->qu, newPitNode(&b12));
    enQueue(doctors[i + 1]->qu, newPitNode(&c12));
    enQueue(doctors[i + 1]->qu, newPitNode(&d12));
    enQueue(doctors[i + 1]->qu, newPitNode(&e12));
    enQueue(doctors[i + 1]->qu, newPitNode(&f12));
}
/**
 * 函  数: 释放所有医生信息
 * 参  数: 无
 * 返回值: 无
 */
void freeDoctor(void)
{
    for (int i = 0; i < 6; i++)
    {
        freeQueue(doctors[i]->qu);
        freeQueue(doctors[i]->overqu);
        free(doctors[i]);
    }
}
/**
 * 函  数: 获取挂号病人函数
 * 参  数: 无
 * 返回值: 无
 * 这里的参数先不管啦
 */
int dowFlag = 0;
void *getPitThread(void *arg)
{
    patient tmp = {0};
    int size = sizeof(tmp);

    while (dowFlag)
    {
        if (system("scp sen@192.168.8.126:/home/sen/io/pit ../docs/pitData") != 0)
        {
            HANDLE_ERROR("SCP failed");
            sleep(5);
            continue;
        }

        FILE *pitdf = fopen("../docs/pitData", "rb+");
        if (pitdf == NULL)
        {
            HANDLE_ERROR("Failed to open pitData");
            sleep(5); // 延迟后再重试
            continue;
        }

        while (fread(&tmp, size, 1, pitdf) == 1 && dowFlag)
        {
            printf("Read doctor index: %d\n", tmp.doctrIndex); // 打印调试信息

            if (tmp.doctrIndex < 0 || tmp.doctrIndex > 6)
            {
                fprintf(stderr, "Invalid doctor index: %d\n", tmp.doctrIndex);
                continue; // 如果序号无效，则跳过该条记录
            }

            enQueue(doctors[tmp.doctrIndex]->qu, newPitNode(&tmp)); // 入到相应医生的队列

            if (tmp.doctrIndex == i)
            {
                showQueue();
            }
        }
        if (system("ssh sen@192.168.8.126 'cat /dev/null > /home/sen/io/pit'") != 0)
        {
            HANDLE_ERROR("Failed to clear remote file");
        }
        fclose(pitdf); // 关闭文件
        sleep(10);     // 延迟5秒再重复
    }

    pthread_exit(NULL); // 线程安全退出
}
/**
 * 函  数: 医院叫号页面
 * 参  数: 无
 * 返回值: 无
 */
void Hospital(void)
{
    // 按钮序列
    Button buttons[8] = {
        {10, 75, 155, 240},
        {530, 270, 640, 305}, // 1
        {530, 330, 640, 365}, // 2
        {530, 390, 640, 425}, // 3
        {670, 270, 780, 305}, // 4
        {670, 330, 780, 365}, // 5
        {670, 390, 780, 425},
        {765, 0, 800, 35}}; // 6
    // 创建线程
    pthread_t thread1, thread2, thread3;
    int arg = 2; // 传递给线程的参数

    // 创建线程
    dowFlag = 1;
    tPthread = 1;
    readDoctorsFromFile();
    if (pthread_create(&thread1, NULL, timeThreadFunction, &arg) != 0 || pthread_create(&thread2, NULL, waterThreadFunction, &arg) != 0 || pthread_create(&thread3, NULL, getPitThread, NULL) != 0)
        HANDLE_ERROR(strerror(errno));

    img *BMPtmp = BMPObject("../rec/img/hospital/a.bmp"); // 背景图
    showBMP(LCD, 0, 0, 0, 0, 480, BMPtmp);
    freeBMP(BMPtmp);

    int touch, tmp;
    pitNode *tmpNode;

    testData();
    while (1)
    {

        showDoctors();
        showQueue();
        showed();
        showOver();
        touch = getDirection();

        if (touch == CLICK)
        {
            tmp = detectButton(buttons, 8);
            if (tmp == 0)
            {
                i = (i == 5) ? 0 : i + 1;
                Lcd_ShowFont(250, 170, getColor(0, 255, 255, 255), 0, 0, 0, 0, 530, 270, "");
                continue;
            }
            if (tmp >= 1 && tmp <= doctors[i]->overqu->size)
            {
                Lcd_ShowFont(110, 35, getColor(0, 255, 255, 255), 0, 0, 0, 0, buttons[doctors[i]->overqu->size].x1, buttons[doctors[i]->overqu->size].y1, "");
                enQueue(doctors[i]->qu, getOutNode(doctors[i]->overqu, tmp - 1));
                continue;
            }
            if (tmp == 7)
                break;
        }
        if (touch == LEFT) // 出队
        {
            free(pits[i]);
            pits[i] = outQueue(doctors[i]->qu);
            continue;
        }
        if (touch == DOWN && doctors[i]->qu->size > 0) // 过号
        {
            // 首先检查是否需要将过号队列的第一个元素移除
            if (doctors[i]->overqu->size >= 6)
            {
                tmpNode = outQueue(doctors[i]->overqu);
                if (tmpNode != NULL)
                {
                    free(tmpNode);
                }
            }

            //  qu 队列中移除第一个节点并将其放入 overqu 队列
            tmpNode = outQueue(doctors[i]->qu);
            if (tmpNode != NULL)
            {
                enQueue(doctors[i]->overqu, tmpNode); // 将其移入过号队列
            }
        }
    }
    freeDoctor(); // 释放所有数据
    i = 0;
    dowFlag = 0;
    tPthread = 0;
    for (; i < 6; i++)
    {
        if (pits[i] != NULL)
        {
            free(pits[i]);  // 释放正在看诊的病人的节点
            pits[i] = NULL; // 将指针设置为 NULL
        }
    }
    i = 0;

    sleep(1);
}