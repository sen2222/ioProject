#include "main.h"

/**
 * 函  数: 格式化广告路径
 * 参  数: 无
 * 返回值: 路径字符串
 */
char *getVideoName(void)
{
    // 创建目录指针并打开目录
    struct dirent *entry;
    DIR *dp = opendir("../rec/video/1");
    if (dp == NULL)
    {
        HANDLE_ERROR("Failed to open video directory");
        return NULL;
    }

    int size = 1024;
    char *fileName = (char *)malloc(size);

    memset(fileName, 0, size); // 清零
    strcat(fileName, "./mplayer -quiet -geometry 0:0 -zoom -x 800 -y 480 -loop 0");

    // 循环读取目录中的每个项
    while ((entry = readdir(dp)) != NULL)
    {
        // 跳过 "." 和 ".." 目录项
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        // 拼接文件路径
        if (strlen(fileName) + strlen(entry->d_name) + 11 >= size)
        {
            size *= 2;
            printf("%s\n", fileName);
            char *tmp = (char *)realloc(fileName, size);
            if (tmp == NULL)
            {
                HANDLE_ERROR("Out of memory");
                free(fileName);
                return NULL;
            }
            fileName = tmp;
        }
        strcat(fileName, " ../rec/video/1/");
        strcat(fileName, entry->d_name);
    }
    strcat(fileName, " &");
    closedir(dp);
    return fileName;
}

/**
 * 播放视频
 */
void advertisingPlay(void)
{
    // 创建视频路径名
    char *fileName = getVideoName();

    if (fileName != NULL)
    {
        system(fileName);
        while (getDirection() != UP)
            ;
        system("../script/kill_all_mplayer.sh"); // 杀死视频线程
        free(fileName);
    }
}
