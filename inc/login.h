#ifndef __LOGIN_H
#define __LOGIN_H

typedef struct 
{   
    char usrName[15];
    char usrPasswd[15];
}__attribute__((packed)) user;

void login(void);

#endif
