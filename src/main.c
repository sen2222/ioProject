#include "main.h"

/**
 * 函  数: 初始化函数
 * 参  数: 无
 * 返回值: 无
 */
void Init(void)
{
    // 打开屏幕即字库需要的
    LCD_Font = malloc(sizeof(struct LcdDevice));
    LCD_Font->fd = open("/dev/fb0", O_RDWR); // 打开lcd
    LCD_Font->mp = mmap(NULL, LCD_HEIGHT * LCD_WIDTH * 4, PROT_WRITE, MAP_SHARED, LCD_Font->fd, 0);
    LCD = (char *)LCD_Font->mp;
    if (LCD_Font->mp == MAP_FAILED)
    {
        HANDLE_ERROR("Failed to map LCD memory");
        exit(1);
    }
    // 打开字体
    f = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    f1 = fontLoad("/usr/share/fonts/DroidSansFallback.ttf");
    fontSetSize(f1, 30);

    errnoLog = fopen("../docs/log", "a"); // 打开错误日志
    // 从服务器下载视频
    system("../script/download_video.sh &");
    system("../script/download_img.sh &"); // 下载服务器图片

    tp = open("/dev/input/event0", O_RDWR); // 触控
}
//
/**
 * 函  数: 主页面目录
 * 参  数: 无
 * 返回值: 无
 */
void menu(void)
{
    Button buttons1[6] =
    {
        {51, 10, 240, 205},
        {300, 10, 490, 210},
        {550, 10, 730, 210},
        {50, 270, 240, 440},
        {300, 270, 490, 440},
        {550, 270, 730, 440}};

    img *menuImg = BMPObject("../rec/img/menu.bmp");
    while (1)
    {
        showBMP(LCD, 0, 0, 0, 0, 480, menuImg);

        while (getDirection() != CLICK)
            ;
        switch (detectButton(buttons1, 6))
        {
        case 0:
            showGallery(); // 相册(图片广告)
            break;
        case 1:
            advertisingPlay();
            break;
        case 2:
            scoreBoard();
            break;
        case 3:
            Hospital(); // 挂号系统
            break;

        case 5:
            return;
        default:
            break;
        }
    }
}

int main()
{
    // 开机加载视频(装酷用的)
    system("./mplayer -quiet  -geometry 0:0 -zoom -x 800 -y 480 -loop 1  ../rec/video/open.mp4 &");

    // 初始化
    Init();
   sleep(4);

    system("../script/kill_all_mplayer.sh"); // 杀死视频线程

    while (1)
    {
        login();
        menu();
    }

    return 0;
}