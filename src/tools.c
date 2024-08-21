#include <pthread.h>
#include "main.h"

/**
 * 函  数: 获取当前时间
 * 参  数: 无
 * 返回值: 时间字符串
 */
char *getTime(void)
{
    time_t timep;
    time(&timep);                     // 获取当前时间
    struct tm *p = localtime(&timep); // 转换为本地时间

    static char timeStr[20];

    // 格式化时间字符串，格式为 "YYYY-MM-DD HH:MM:SS"
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", p);

    return timeStr;
}
/**
 * 函  数: 字符显示快捷方式
 * 参  数:
 *      bmw      - 画板宽度
 *      bmh      - 画板高度
 *      bmColor  - 画板背景颜色，使用getColor(r, g, b, a)
 *      size     - 字体大小
 *      fColor   - 字体颜色 getColor(r, g, b, a)
 *      fw       - 字体显示在LCD屏幕上的X坐标位置。
 *      fh       - 字体显示在LCD屏幕上的Y坐标位置。
 *      buf      - 字符串缓冲区，包含要显示的文本内容。
 * 返回值: 无
 */
void Lcd_ShowFont(int bmw, int bmh, int bmColor, int size, int fColor, int bw, int bh, int fw, int fh, char *buf)
{
    // 创建一个位图，用于存放文字
    bitmap *bm = createBitmapWithInit(bmw, bmh, 4, bmColor); // 背景色为黑色

    fontSetSize(f, size);

    // 将字体绘制到位图上
    fontPrint(f, bm, bw, bh, buf, fColor, 0);

    // 将位图显示到LCD屏幕上
    show_font_to_lcd(LCD_Font->mp, fw, fh, bm);

    // 清理位图内存
    destroyBitmap(bm);
}
/**
 * 函  数: 时间线程函数
 * 参  数: 无
 * 返回值: 无
 * 这里的参数先不管啦
 */
void *timeThreadFunction(void *arg)
{
    int *flag = (int *)arg;
    if (*flag == 1)
    {                                                                          // 创建一个位图，用于存放文字
        bitmap *bm = createBitmapWithInit(220, 40, 4, getColor(255, 0, 0, 0)); // 背景色为黑色、
        while (tPthread)
        {

            // 将字体绘制到位图上
            fontPrint(f1, bm, 10, 5, getTime(), getColor(0, 0, 255, 255), 0);

            // 将位图显示到LCD屏幕上
            show_font_to_lcd(LCD_Font->mp, 0, 440, bm);

            // 清理位图内存
            memset(bm->map, 0, 220 * 40 * 4);
        }
        destroyBitmap(bm);
    }
    else if (*flag == 2)
    {
        // 创建一个位图，用于存放文字
        bitmap *bm = createBitmapWithInit(215, 35, 4, getColor(0, 39, 126, 226));
        // 创建一个模板位图作为背景
        bitmap *bgTemplate = createBitmapWithInit(215, 35, 4, getColor(0, 39, 126, 226));
        // 主循环
        while (tPthread)
        {
            // 将背景模板复制到目标位图
            memcpy(bm->map, bgTemplate->map, 215 * 35 * 4);

            // 将字体绘制到位图上
            fontPrint(f1, bm, 10, 5, getTime(), 0, 0);

            // 将位图显示到LCD屏幕上
            show_font_to_lcd(LCD_Font->mp, 580, 0, bm);

            usleep(450000);
        }
        destroyBitmap(bm);
        destroyBitmap(bgTemplate); // 清理模板位图
    }

    pthread_exit(NULL); // 线程结束时调用
}

/**
 * 函  数: 流水字线程函数
 * 参  数: 无
 * 返回值: 无
 * 这里的参数先不管啦
 */
void *waterThreadFunction(void *arg)
{
    int *flag = (int *)arg;
    if (*flag == 1)
    { // 现有的滚动逻辑
        char *str = "好消息!好消息!好消息!我也不知道啥好消息!";
        bitmap *bm = createBitmapWithInit(380, 40, 4, 0);
        bitmap *bmtmp = createBitmapWithInit(380, 40, 4, 0); // 画板模板

        int i = 380;
        unsigned int color = getColor(0, 0, 255, 255);
        while (tPthread)
        {
            memcpy(bm->map, bmtmp->map, 380 * 40 * 4); // 清空画板
            fontPrint(f1, bm, i, 5, str, color, 0);
            i = (i <= -400) ? 380 : i - 1;
            usleep(10000); // 100毫秒
            show_font_to_lcd(LCD_Font->mp, 220, 440, bm);
        }
    }
    else if (*flag == 2)
    {
        char *str = "温馨提醒: 请保持安静, 避免大声喧哗。如您过号, 请及时联系护士重新排队。若过号超过六人, 需重新挂号。感谢您的配合, 祝您早日康复!";
        font *f2 = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
        fontSetSize(f2, 20);
        bitmap *bm = createBitmapWithInit(800, 20, 4, getColor(0, 3, 130, 216));
        bitmap *bmtmp = createBitmapWithInit(800, 20, 4, getColor(0, 3, 130, 216)); // 画板模板

        int i = 800;
        while (tPthread)
        {
            memcpy(bm->map, bmtmp->map, 800 * 20 * 4);

            fontPrint(f2, bm, i, 0, str, 0, 0);
            i = (i <= -800) ? 800 : i - 1;
            usleep(10000); // 100毫秒
            show_font_to_lcd(LCD_Font->mp, 0, 460, bm);
        }
        fontUnload(f2);
        destroyBitmap(bm);
    }
    else if (*flag == 3)
    {
        char *str = "富强 民主 文明 和谐 自由 平等 公正 法治 爱国 敬业 诚信 友善";
        bitmap *bm = createBitmapWithInit(300, 30, 4, 0);
        bitmap *bmtmp = createBitmapWithInit(300, 30, 4, 0); // 画板模板

        int i = 300;
        while (tPthread)
        {
            memcpy(bm->map, bmtmp->map, 300 * 30 * 4);

            fontPrint(f1, bm, i, 0, str, getColor(0, 0, 205, 0), 0);
            i = (i <= -600) ? 300 : i - 1;
            usleep(10000); // 100毫秒
            show_font_to_lcd(LCD_Font->mp, 250, 0, bm);
        }
        destroyBitmap(bm);
    }
    // 扫描
    pthread_exit(NULL); // 线程结束时调用
}
