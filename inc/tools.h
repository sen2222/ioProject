#ifndef __ERRNOLOG_H
#define __ERRNOLOG_H


char *getTime(void);
void Lcd_ShowFont(int bmw, int bmh, int bmColor, int size, int fColor, int bw, int bh,int fw, int fh, char *buf);
void *timeThreadFunction(void *arg);
void *waterThreadFunction(void *arg);

#endif