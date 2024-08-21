#include "main.h"
#include <pthread.h>
/*******************************************链表部分*************************************************/

/**
 * 函数: 判断链表是否为空
 * 参数: img 图片链表管理结构体
 * 返回值: true 为空 false 不为空
 */
bool isEmpty(const imgList *lt)
{
    return (lt->num == 0);
}

/**
 * 函数: 初始化一个图片链表
 * 参数: 无
 * 返回值: 链表管理结构体
 */
imgList *newimgList(void)
{
    imgList *lt = (imgList *)malloc(sizeof(imgList));
    if (lt == NULL)
    {
        HANDLE_ERROR("Failed to allocate memory for imgList");
        return NULL;
    }

    lt->head = NULL;
    lt->num = 0;
    return lt;
}

/**
 * 函数: 创建一个图片节点
 * 参数: bm 图片信息结构体地址
 * 返回值: 图片节点指针
 */
imgNode *newBmpNode(img *bmp)
{
    imgNode *newNode = (imgNode *)malloc(sizeof(imgNode));
    if (newNode == NULL)
    {
        HANDLE_ERROR("Failed to allocate memory for imgNode");
        return NULL;
    }

    newNode->imgObject = bmp;
    INIT_LIST_HEAD(&newNode->list);
    return newNode;
}

/**
 * 函数: 图片节点加入链表
 * 参数: lt 图片链表管理结构体
 * 参数: imgNode 图片节点
 * 返回值: true 成功 false 失败
 */
bool insertHead(imgList *lt, imgNode *imgNode)
{
    if (lt == NULL)
    {
        HANDLE_ERROR("imgList is NULL");
        return false;
    }

    if (imgNode == NULL)
    {
        HANDLE_ERROR("imgNode is NULL");
        return false;
    }

    // 链表为空
    if (isEmpty(lt))
    {
        lt->head = imgNode;
    }

    list_add_tail(&imgNode->list, &lt->head->list);
    lt->head = imgNode;
    lt->num++;

    return true;
}

/**
 * 函数: 前移头结点
 * 参数: lt 图片链表管理结构体
 * 返回值: NULL 链表为空 imgNode 头结点指向的图片节点
 */
imgNode *forward(imgList *lt)
{
    if (lt == NULL || lt->head == NULL)
    {
        HANDLE_ERROR("imgList or head is NULL");
        return NULL;
    }

    lt->head = list_entry(lt->head->list.next, imgNode, list);
    return list_entry(lt->head->list.prev, imgNode, list);
}
/**
 * 函数: 后移头结点
 * 参数: lt 图片链表管理结构体
 * 返回值: NULL 链表为空 imgNode 头结点指向的图片节点
 */
imgNode *aforward(imgList *lt)
{
    if (lt == NULL || lt->head == NULL)
    {
        HANDLE_ERROR("imgList or head is NULL");
        return NULL;
    }

    lt->head = list_entry(lt->head->list.prev, imgNode, list);
    return lt->head;
}

/**
 * 函数: 获得头结点
 * 参数: lt 图片链表管理结构体
 * 返回值: NULL 链表为空 imgNode 头结点指向的图片节点
 */
imgNode *getHear(imgList *lt)
{
    if (lt == NULL)
    {
        HANDLE_ERROR("imgList is NULL");
        return NULL;
    }

    return lt->head;
}

/**
 * 函数: 获得头结点~第五个节点指针
 * 参数: lt 图片链表管理结构体 bm 指针结构体
 * 返回值: 无
 */
void GetSeq(const imgList *lt, img *bm[])
{
    if (isEmpty(lt))
        return;
    int n = 0;
    imgList tmp = *lt;
    while (n < 5)
    {
        bm[n] = forward(&tmp)->imgObject;
        n++;
    }
}

/**
 * 函  数: 释放图片链表
 * 参  数: lt 链表管理结构体
 * 返回值: 无
 */
void freeImgList(imgList *lt)
{
    if (lt == NULL)
    {
        return; // 如果链表管理结构体为空，直接返回
    }

    if (lt->head != NULL)
    {

        struct list_head *pos, *n;
        imgNode *imgtmp;

        // 遍历链表中的每个节点
        list_for_each_safe(pos, n, &lt->head->list)
        {
            // 从链表中删除当前节点
            list_del(pos);

            // 获取当前节点的指针
            imgtmp = list_entry(pos, imgNode, list);

            // 释放节点中的图片数据和图片结构体
            if (imgtmp->imgObject != NULL)
            {
                freeBMP(imgtmp->imgObject); // 正确传递图片结构体
            }

            // 释放当前节点
            free(imgtmp);
        }
    }
    // 最后释放链表管理结构体
    free(lt);
}

/*******************************************映射部分*************************************************/
/**
 * 函  数: 从文件描述符读取数据，带错误处理机制
 * 参  数: fd 文件描述符
 * 参  数: buf 缓冲区指针
 * 参  数: count 需要读取的字节数
 * 返回值: 无
 */
void checkRead(int fd, char *buf, int count)
{
    int n = 0, m = 0;
    while (m < count)
    {
        n = read(fd, buf + m, count - m);
        if (n == -1)
        {
            if (errno == EINTR)
                continue;
            else // 其他读取错误
            {
                HANDLE_ERROR(strerror(errno));
                exit(1);
            }
        }
        if (n == 0)
            break; // 文件结束
        m += n;    // 成功读了几个字节
    }
}

/**
 * 函  数: 获取一个bmp图像的关键信息，并申请内存把图片信息读进去
 * 参  数: address 图片的地址
 * 返回值: bmp图像的关键信息的结构体
 */
img *BMPObject(char *address)
{
    if (address == NULL)
    {
        HANDLE_ERROR("address is NULL");
        return NULL;
    }

    // 打开图片文件
    int fd = open(address, O_RDONLY);
    if (fd == -1)
    {
        HANDLE_ERROR(strerror(errno));
        exit(1);
    }

    // 读取BMP格式头，获取图片信息
    struct bitmap_header header;
    struct bitmap_info info;

    // 读取文件头
    checkRead(fd, (char *)&header, BMP_HEADER_SIZE);

    // 读取信息头
    checkRead(fd, (char *)&info, BMP_INFO_SIZE);

    // 构建图片信息结构体
    img *newImg = (img *)malloc(sizeof(img));
    if (newImg == NULL)
    {
        HANDLE_ERROR(strerror(errno));
        close(fd);
        exit(1);
    }

    newImg->width = info.width;
    newImg->height = info.height;
    newImg->bpp = info.bit_count;
    newImg->size = header.size;

    // 读取图片信息到代码空间
    newImg->bm = calloc(1, newImg->size);
    if (newImg->bm == NULL)
    {
        HANDLE_ERROR(strerror(errno));
        free(newImg);
        close(fd);
        exit(1);
    }

    checkRead(fd, newImg->bm, newImg->size);

    close(fd);
    return newImg;
}

/**
 * 函  数: 打印图片到液晶屏上
 * 参  数: p 液晶屏映射的地址
 * 参  数: x y 图片起始位置(左上角)
 * 参  数: bmph bmpw 从图片的哪个位置开始映射
 * 参  数: bmp 图片信息结构体
 * 返回值: 无
 */
void showBMP(char *p, int x, int y, int bmph, int bmpw, int stoph, img *bmp)
{
    if (p == NULL || bmp == NULL)
    {
        HANDLE_ERROR("Null pointer passed to showBMP");
        return;
    }

    int bit = bmp->bpp / 8; // 一像素位数
    // printf("%d\n", bit);
    // 计算无效字节数
    int pad = (4 - (bmp->width * bit) % 4) % 4;

    // 映射
    int i, j;
    int lcd_offset;
    // 指针指向指定位置
    char *top = bmp->bm + (bmp->width * bit + pad) * (bmp->height - 1 - bmph) + bmpw * bit;
    char *rgb;
    for (i = 0; i < bmp->height - bmph && i < LCD_HEIGHT - y && i < stoph; i++) // 行
    {
        rgb = top;
        for (j = 0; j < bmp->width - bmpw && j < LCD_WIDTH - x; j++) // 列
        {
            lcd_offset = ((y + i) * LCD_WIDTH + (x + j)) * 4;
            memcpy(p + lcd_offset, rgb, bit); // 映射过去
            rgb += bit;
        }
        top -= (bmp->width * bit + pad); // 移动到上一行
    }
}

/**
 * 函数: 清屏函数
 * 参数: p 液晶屏映射的地址
 * 返回值: 无
 */
void LCDclear(char *p, int x, int y)
{
    if (p == NULL)
    {
        HANDLE_ERROR("Null pointer passed to LCDclear");
        return;
    }

    unsigned int bleak = 0x0000E5EE;
    int screen_width = 800;  // 屏幕的宽度
    int screen_height = 480; // 屏幕的高度

    // 计算起始位置在屏幕缓冲区中的偏移量
    int start_position = y * screen_width + x;

    // 计算需要清除的像素数
    int pixels_to_clear = screen_width * screen_height - start_position;

    // 从指定的 x, y 坐标开始清除屏幕缓冲区
    memset(p + start_position * 4, bleak, pixels_to_clear * 4);
}

/**
 * 函  数: 释放图片结构体
 * 参  数: p 图片结构体指针
 * 返回值: 无
 */
void freeBMP(img *p)
{
    if (p == NULL)
    {
        HANDLE_ERROR("Null pointer passed to freeBMP");
        return;
    }

    free(p->bm);
    free(p);
}

/*******************************************图像处理部分*************************************************/

// 将 BMP 图片缩放到指定的宽度和高度的函数
img *resizeBMP(img *src, int target_width, int target_height)
{
    if (src == NULL)
    {
        HANDLE_ERROR("Source image pointer is NULL");
        return NULL;
    }

    img *resizedImg = (img *)malloc(sizeof(img));
    if (resizedImg == NULL)
    {
        HANDLE_ERROR("Failed to allocate memory for resized image");
        exit(1);
    }

    resizedImg->width = target_width;
    resizedImg->height = target_height;
    resizedImg->bpp = src->bpp;
    resizedImg->size = target_width * target_height * (resizedImg->bpp / 8);
    resizedImg->bm = (unsigned char *)malloc(resizedImg->size);

    if (resizedImg->bm == NULL)
    {
        HANDLE_ERROR("Failed to allocate memory for resized image data");
        free(resizedImg);
        exit(1);
    }

    float x_ratio = (float)src->width / (float)target_width;
    float y_ratio = (float)src->height / (float)target_height;
    int src_row_size = (src->width * (src->bpp / 8) + 3) & ~3;
    int dst_row_size = (target_width * (resizedImg->bpp / 8) + 3) & ~3;

    for (int y = 0; y < target_height; y++)
    {
        for (int x = 0; x < target_width; x++)
        {
            int src_x = (int)(x * x_ratio);
            int src_y = (int)(y * y_ratio);
            int src_index = (src_y * src_row_size) + (src_x * (resizedImg->bpp / 8));
            int dst_index = (y * dst_row_size) + (x * (resizedImg->bpp / 8));
            memcpy(&resizedImg->bm[dst_index], &src->bm[src_index], resizedImg->bpp / 8);
        }
    }

    return resizedImg;
}

/**
 * 函  数: 遍历图片目录,创建全部图片的信息链表
 * 参  数: address 目录地址
 * 返回值: 链表管理结构体
 */
imgList *listDirectory(const char *address)
{
    // 创建目录指针并打开目录
    struct dirent *entry;
    DIR *dp = opendir(address);
    if (dp == NULL)
    {
        // printf("0\n");
        HANDLE_ERROR("Failed to open image directory");
        return NULL;
    }

    // 创建图片链表
    imgList *lt = newimgList();

    char fileName[1024] = {0};

    // 循环读取目录中的每个项
    while ((entry = readdir(dp)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(fileName, sizeof(fileName), "%s/%s", address, entry->d_name); // 拼接地址

        if (insertHead(lt, newBmpNode(BMPObject(fileName))) == false)
        {
            fprintf(errnoLog, "%s : Failed to insert image into list: %s\n", getTime(), fileName);
            fflush(errnoLog);
        }
    }

    // 关闭目录流，释放资源
    closedir(dp);
    return lt;
}

/**
 * 函  数: 全屏页面显示
 * 参  数: lt 图片链表管理结构体
 * 返回值: 无
 */
void FullLCD(imgList *lt)
{
    int tmp;
    img *imgtmp;
    showBMP(LCD, 0, 0, 0, 0, 480, getHear(lt)->imgObject);
    while (1)
    {
        tmp = getDirection(); // 获取屏幕情况
        if (tmp == LEFT)
        {
            imgtmp = forward(lt)->imgObject;
            for (int i = 0; i <= 800; i += 80)
            {
                showBMP(LCD, 0, 0, 0, i, 480, imgtmp);
                showBMP(LCD, 800 - i, 0, 0, 0, 480, getHear(lt)->imgObject);
            }
        }
        else if (tmp == RIGHT)
        {
            showBMP(LCD, 0, 0, 0, 0, 480, aforward(lt)->imgObject);
        }
        else if (tmp == CLICK)
            break;
    }
}

int slidingFlag = 0;
/**
 * 函  数: 轮播函数
 * 参  数: newImg 新照片
 * 参  数: oldImg 上一张照片
 * 返回值: 无
 */
void *slidingImg(void *lt)
{
    lt = (imgList *)lt;
    int i;
    imgNode *oldImg;
    imgNode *newImg;
    srand(time(NULL));
    while (slidingFlag)
    {
        i = 0;
        oldImg = forward(lt);
        newImg = getHear(lt);
        if (rand() % 2 == 0)
        {
            for (; i <= 800 && slidingFlag; i += 1)
            {
                showBMP(LCD, 0, 0, 0, i, 480, oldImg->imgObject);
                showBMP(LCD, 800 - i, 0, 0, 0, 480, newImg->imgObject);
            }
        }
        else
        {
            for (; i <= 480 && slidingFlag; i += 1)
            {
                showBMP(LCD, 0, 0, i, 0, 480, oldImg->imgObject);
                showBMP(LCD, 0, 480 - i, 0, 0, 480, newImg->imgObject);
            }
        }
        // 适当延迟，保持图片显示稳定
        if (slidingFlag)
        {
            sleep(5); // 每次轮播的延时
        }
    }
    pthread_exit(NULL); // 线程结束时调用
}

/**
 * 函  数: 详情页面显示
 * 参  数: lt 图片链表管理结构体
 * 返回值: 无
 */
void showGallery(void)
{
    // 按键序列
    Button buttons2[7] = {
        {0, 0, 800, 384},      // 大图像区域
        {0, 384, 80, 464},     // 播放按键
        {240, 384, 340, 480},  // 图片2
        {340, 384, 440, 480},  // 图片3
        {440, 384, 540, 480},  // 图片4
        {540, 384, 640, 480},  // 图片5
        {700, 380, 800, 480}}; // 退出
    imgList *lt = listDirectory("../rec/img/1");
    if (lt == NULL || lt->num == 0)
    {
        HANDLE_ERROR("dis");
        return;
    }
    // 返回图标
    img *breakBmp = BMPObject("../rec/img/break.bmp");
    // 播放图标
    img *bofanBmp = BMPObject("../rec/img/bofan.bmp");

    // 相册逻辑
    img *bm[5], *imgtmp;
    int i, button, tmp;
    // 清空一下背景
    showBMP(LCD, 0, 0, 0, 0, 480, getHear(lt)->imgObject); // 大图片区域
    LCDclear(LCD, 0, 384);
    showBMP(LCD, 720, 400, 0, 0, 480, breakBmp);
    showBMP(LCD, 0, 400, 0, 0, 480, bofanBmp);
    while (1)
    {
        GetSeq(lt, bm); // 获取0~5张照片
        for (i = 0; i < 5; i++)
        {
            imgtmp = resizeBMP(bm[i], 100, 71);
            showBMP(LCD, 140 + i * 105, 400, 0, 0, 480, imgtmp);
            freeBMP(imgtmp);
        }

        tmp = getDirection(); // 获取屏幕情况
        if (tmp == LEFT)
        {
            forward(lt);
            showBMP(LCD, 0, 0, 0, 0, 384, getHear(lt)->imgObject); // 大图片区域
        }
        else if (tmp == RIGHT)
        {
            showBMP(LCD, 0, 0, 0, 0, 384, aforward(lt)->imgObject);
        } // 大图片区域
        else if (tmp == CLICK)
        {
            button = detectButton(buttons2, 7);
            // printf("%d\n", button);
            // 点了大图片
            if (button == 0)
            {
                FullLCD(lt);
                LCDclear(LCD, 0, 384);
                showBMP(LCD, 720, 400, 0, 0, 480, breakBmp);
                showBMP(LCD, 0, 400, 0, 0, 480, bofanBmp);
            }
            else if (button == 1)
            {
                // saveImgListToFile(lt, "../imgfile"); // 把图片写入文件
                // system("./sliding &");
                pthread_t thread;
                // 创建线程
                slidingFlag = 1;
                if (pthread_create(&thread, NULL, slidingImg, (void *)lt) != 0)
                    HANDLE_ERROR(strerror(errno));
                while (getDirection() != UP)
                    ;
                slidingFlag = 0;
                pthread_join(thread, NULL);
                LCDclear(LCD, 0, 384);
                showBMP(LCD, 720, 400, 0, 0, 480, breakBmp);
                showBMP(LCD, 0, 400, 0, 0, 480, bofanBmp);
                showBMP(LCD, 0, 0, 0, 0, 384, getHear(lt)->imgObject);
            }
            else if (button == 2 || button == 3 || button == 4 || button == 5) // 点了小图片区域
            {
                for (i = 1; i < button; i++)
                    forward(lt);
                showBMP(LCD, 0, 0, 0, 0, 384, getHear(lt)->imgObject); // 大图片区域
            }
            else if (button == 6)
            {
                freeImgList(lt);
                freeBMP(breakBmp);
                freeBMP(bofanBmp);
                return;
            }
        }
    }
}
