#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <time.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <linux/fb.h>
#include "touch.h"
#include "font.h"
struct LcdDevice *LCD_Font;
font *f;
font *f1;           //时间字体(线程会乱改)
int tPthread;
#include "tools.h"


FILE *errnoLog;
#define HANDLE_ERROR(msg)                                                                           \
    do                                                                                              \
    {                                                                                               \
        fprintf(errnoLog, "%s : %s - in file %s at line %d\n", getTime(), msg, __FILE__, __LINE__); \
        fflush(errnoLog);                                                                           \
    } while (0)

#include "list.h"
#include "img.h"
#include "login.h"
#include "hospital.h"
#include "advertising.h"
#include "scoreboard.h"
int tp;
unsigned char* LCD;

#endif
