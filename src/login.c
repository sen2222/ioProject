#include "main.h"
#include <pthread.h>

/**
 * 函  数: 去除字符串中的空格
 * 参  数: 无
 * 返回值: 无
 */
void removeChar(char *str)
{
    int srcIndex = 0, destIndex = 0;

    // 遍历整个字符串
    while (str[srcIndex] != '\0')
    {
        // 如果当前字符不是空格也不是下划线，复制到目标位置
        if (str[srcIndex] != ' ' && str[srcIndex] != '_')
        {
            str[destIndex] = str[srcIndex];
            destIndex++;
        }
        srcIndex++;
    }

    // 在目标字符串的末尾添加终止符
    str[destIndex] = '\0';
}

/**
 * 函  数: 登录处理函数
 * 参  数: usrBuf 账号
 * 参  数: pwBuf 密码
 * 返回值: true 可以登录 flase 密码或者账号错误
 */
bool rogin(char *usrBuf, char *pwBuf)
{
    FILE *df = fopen("../docs/usrData", "rb"); // 读取二进制文件
    if (df == NULL)
    {
        HANDLE_ERROR(strerror(errno));
        return false;
    }

    // 去掉空格
    removeChar(usrBuf);
    removeChar(pwBuf);

    int userSize = sizeof(user); // 获取结构体大小

    user *tmp = (user *)malloc(userSize);

    while (fread(tmp, 1, userSize, df) == userSize)
    {
        if (strcmp(usrBuf, tmp->usrName) == 0 && strcmp(pwBuf, tmp->usrPasswd) == 0)
        {
            free(tmp);
            fclose(df);
            return true;
        }
    }
    fclose(df);
    free(tmp);
    return false;
}

/**
 * 函  数: 注册处理函数
 * 参  数: usrBuf 账号
 * 参  数: pwBuf 密码
 * 返回值: true 注册成功 flase 注册失败(账号存在的情况)
 */
bool regin(char *usrBuf, char *pwBuf)
{
    FILE *df = fopen("../docs/usrData", "ab+"); // 读取二进制文件
    if (df == NULL)
    {
        HANDLE_ERROR(strerror(errno));
        return false;
    }

    // 去掉空格
    removeChar(usrBuf);
    removeChar(pwBuf);

    int userSize = sizeof(user); // 获取结构体大小

    user *tmp = (user *)malloc(userSize);
    // 重置文件指针到文件开头
    fseek(df, 0, SEEK_SET);
    while (fread(tmp, 1, userSize, df) == userSize)
    {
        if (strcmp(usrBuf, tmp->usrName) == 0)
        {
            free(tmp);
            fclose(df);
            return false;
        }
    }
    strncpy(tmp->usrName, usrBuf, sizeof(tmp->usrName) - 1);
    tmp->usrName[sizeof(tmp->usrName) - 1] = '\0'; // 确保字符串以 null 结尾

    strncpy(tmp->usrPasswd, pwBuf, sizeof(tmp->usrPasswd) - 1);
    tmp->usrPasswd[sizeof(tmp->usrPasswd) - 1] = '\0';

    // 写入新用户
    if (fwrite(tmp, userSize, 1, df) != 1)
    {
        HANDLE_ERROR("Failed to write user data.");
        free(tmp);
        fclose(df);
        return false;
    }

    fflush(df);
    free(tmp);
    fclose(df);
    return true;
}

/**
 * 函  数: 登录页面
 * 参  数: 无
 * 返回值: 无
 */
void login(void)
{
    // return;
    // 按键序列
    Button buttons0[17] =
        {
            {667, 410, 733, 480}, // 0

            {600, 200, 667, 270}, // 1
            {667, 200, 733, 270}, // 2
            {733, 200, 800, 270}, // 3

            {600, 270, 667, 340}, // 4
            {667, 270, 733, 340}, // 5
            {733, 270, 800, 340}, // 6

            {600, 340, 667, 410}, // 7
            {667, 340, 733, 410}, // 8
            {733, 340, 800, 410}, // 9

            {600, 410, 667, 480}, // delete
            {733, 410, 800, 480}, // enter

            {600, 150, 680, 190}, // 登录
            {700, 150, 780, 190}, // 注册
            {770, 100, 800, 130}, // 显示按钮

            {650, 50, 770, 80},   // 账号框
            {650, 100, 770, 130}, // 密码框

        };
    memset(LCD, 0, 800 * 480 * 4);
    // LCDclear(LCD, 0, 440);

    pthread_t thread1, thread2;
    int arg = 1; // 传递给线程的参数

    // 创建线程
    if (pthread_create(&thread1, NULL, timeThreadFunction, &arg) != 0 || pthread_create(&thread2, NULL, waterThreadFunction, &arg) != 0)
        HANDLE_ERROR(strerror(errno));
    tPthread = 1;

    // 账号密码画板

    Lcd_ShowFont(50, 30, getColor(0, 0, 0, 0), 25, getColor(0, 255, 255, 255), 0, 5, 600, 50, "账号");
    Lcd_ShowFont(50, 30, getColor(0, 0, 0, 0), 25, getColor(0, 255, 255, 255), 0, 5, 600, 100, "密码");

    // // 账号输入框
    Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 0, 650, 50, "");

    // 密码输入框
    Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 0, 650, 100, "");

    // 登录注册按钮
    Lcd_ShowFont(80, 40, getColor(0, 224, 224, 224), 25, 0, 22, 10, 600, 150, "登录");
    Lcd_ShowFont(80, 40, getColor(0, 0, 255, 255), 25, 0, 22, 10, 700, 150, "注册");

    // 显示密码按钮
    Lcd_ShowFont(20, 20, getColor(0, 0, 255, 255), 25, 0, 22, 10, 775, 105, "");
    // 显示键盘和播放视频
    showBMP(LCD, 600, 200, 0, 0, 480, BMPObject("../rec/img/keyboard.bmp"));
    system("./mplayer -slave -quiet -input file=/pipe -geometry 0:0 -zoom -x 600 -y 440 -loop 0 -msglevel all=0 ../rec/video/login.mp4 &");

    // 5. 循环检测用户输入
    char usrBuf[20] = {0};  // 显示账号输入缓冲区
    char pwBuf[20] = {0};   // 显示密码输入缓冲区
    char pwstars[20] = {0}; // 密码星星
    char ch;
    bool flag = 1;   // 输入账号密码标志位
    bool loflag = 1; // 登录注册标志位
    bool pwflag = 0; // 显示密码标志位
    int tmp, usrLen = 0, pwLen = 0;

    while (1)
    {
        // 获取按键输入
        while (getDirection() != CLICK)
            ;
        Lcd_ShowFont(80, 30, 0, 25, 0, 0, 5, 650, 15, ""); // 屏蔽提示

        // 转换到哪个按键
        tmp = detectButton(buttons0, 17);
        printf("%d\n", tmp);
        if (tmp == 15) // 输入账号
        {
            flag = 1;
            usrBuf[usrLen] = '_';
            pwBuf[pwLen] = '\0';
            pwstars[pwLen] = '\0';
            pwflag = 0;
            Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 50, usrBuf);
            Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 100, pwstars);
        }
        else if (tmp == 16) // 输入密码
        {
            flag = 0;
            pwBuf[pwLen] = '_';
            pwstars[pwLen] = '_';
            usrBuf[usrLen] = '\0';
            pwflag = 0;
            Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 50, usrBuf);
            Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 100, pwstars);
        }
        else if (tmp == 12) // 点了登录
        {
            loflag = 1;
            Lcd_ShowFont(80, 40, getColor(0, 224, 224, 224), 25, 0, 22, 10, 600, 150, "登录");
            Lcd_ShowFont(80, 40, getColor(0, 0, 255, 255), 25, 0, 22, 10, 700, 150, "注册");
        }
        else if (tmp == 13) // 点了注册
        {
            loflag = 0;
            Lcd_ShowFont(80, 40, getColor(0, 0, 255, 255), 25, 0, 22, 10, 600, 150, "登录");
            Lcd_ShowFont(80, 40, getColor(0, 224, 224, 224), 25, 0, 22, 10, 700, 150, "注册");
        }
        else if (tmp == 14)
        {
            pwflag = !pwflag;
            if (pwflag)
            {
                Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 100, pwBuf);
                Lcd_ShowFont(20, 20, getColor(0, 224, 224, 224), 25, 0, 22, 10, 775, 105, "");
            }
            else
            {
                Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 100, pwstars);
                Lcd_ShowFont(20, 20, getColor(0, 0, 255, 255), 25, 0, 22, 10, 775, 105, "");
            }
        }

        // 输入账号状态
        if (flag)
        {
            if (0 <= tmp && tmp <= 9)
            {
                if (usrLen >= 15)
                {
                    Lcd_ShowFont(80, 30, 0, 25, getColor(0, 255, 0, 0), 0, 5, 650, 15, "最大八位"); // 红色
                    continue;
                }
                ch = (char)(tmp + '0');
                usrBuf[usrLen] = ch;
                strncat(usrBuf, " ", 1); // 追加字符到缓冲区
                usrLen += 2;
                strncat(usrBuf, "_", 1); // 追加字符到缓冲区
                Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 50, usrBuf);
            }
            if (tmp == 11) // 删除
            {
                if (usrLen > 0)
                {
                    usrBuf[usrLen - 1] = '\0';
                    usrBuf[usrLen - 2] = '_';
                    usrLen -= 2;
                }
                Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 50, usrBuf);
            }
            else if (tmp == 10)
            {
                flag = !flag;
            }
        }
        else
        {
            if (0 <= tmp && tmp <= 9)
            {
                if (pwLen >= 15)
                {
                    Lcd_ShowFont(80, 30, 0, 25, getColor(0, 255, 0, 0), 0, 5, 650, 15, "最大八位");
                    continue;
                }
                ch = (char)(tmp + '0');
                pwBuf[pwLen] = ch;
                pwBuf[pwLen + 1] = ' ';
                pwstars[pwLen] = '*';
                pwstars[pwLen + 1] = ' ';

                pwLen += 2;
                pwBuf[pwLen] = '_';
                pwstars[pwLen] = '_';
                if (pwflag)
                    Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 100, pwBuf);
                else
                    Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 100, pwstars);
            }
            if (tmp == 11)
            {
                if (pwLen > 0)
                {
                    pwBuf[pwLen - 1] = '\0';
                    pwBuf[pwLen - 2] = '_';
                    pwstars[pwLen - 1] = '\0';
                    pwstars[pwLen - 2] = '_';
                    pwLen -= 2;
                }
                if (pwflag)
                    Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 100, pwBuf);
                else
                    Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 5, 650, 100, pwstars);
            }
            else if (tmp == 10)
            {
                if (loflag)
                {
                    if (rogin(usrBuf, pwBuf))
                    {
                        Lcd_ShowFont(80, 30, 0, 25, getColor(0, 255, 0, 0), 0, 5, 650, 15, "登录成功");
                        break;
                    }
                    else
                    {
                        Lcd_ShowFont(80, 30, 0, 25, getColor(0, 255, 0, 0), 0, 5, 650, 15, "登录失败");
                        bzero(usrBuf, usrLen);
                        bzero(pwBuf, pwLen);
                        bzero(pwstars, pwLen);
                        // // 账号输入框
                        Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 0, 650, 50, "");
                        // 密码输入框
                        Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 0, 650, 100, "");
                    }
                }
                else
                {
                    if (regin(usrBuf, pwBuf))
                    {
                        Lcd_ShowFont(80, 30, 0, 25, getColor(0, 255, 0, 0), 0, 5, 650, 15, "注册成功");
                        sleep(1);
                        Lcd_ShowFont(80, 30, 0, 25, getColor(0, 255, 0, 0), 0, 5, 650, 15, "请登录");
                        bzero(usrBuf, usrLen);
                        bzero(pwBuf, pwLen);
                        bzero(pwstars, pwLen);
                        loflag = 1;
                        flag = 1;
                        Lcd_ShowFont(80, 40, getColor(0, 224, 224, 224), 25, 0, 22, 10, 600, 150, "登录");
                        Lcd_ShowFont(80, 40, getColor(0, 0, 255, 255), 25, 0, 22, 10, 700, 150, "注册");
                        // // 账号输入框
                        Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 0, 650, 50, "");
                        // 密码输入框
                        Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 0, 650, 100, "");
                    }
                    else
                    {
                        Lcd_ShowFont(80, 30, 0, 25, getColor(0, 255, 0, 0), 0, 5, 650, 15, "注册失败");
                        bzero(usrBuf, usrLen);
                        bzero(pwBuf, pwLen);
                        bzero(pwstars, pwLen);
                        // // 账号输入框
                        Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 0, 650, 50, "");
                        // 密码输入框
                        Lcd_ShowFont(120, 30, getColor(0, 224, 224, 224), 25, 0, 0, 0, 650, 100, "");
                    }
                }
                usrLen = 0;
                pwLen = 0;
            }
        }
    }
    tPthread = 0;
    system("../script/kill_all_mplayer.sh");
}
