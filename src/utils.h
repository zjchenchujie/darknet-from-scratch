#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#define CHUJIE_PRINTF(LevelStr, Msg, ...) \
    do { \
        fprintf(stderr, "[Level]:%s [Func]:%s [Line]:%d [Info]:" Msg, LevelStr, __FUNCTION__, __LINE__, ## __VA_ARGS__); \
    }while(0)

#define CHUJIE_PRINTF_RED(LevelStr, Msg, ...) \
    do{ \
        fprintf(stderr, "\033[0;31m [Level]:%s [Func]:%s [Line]:%d [Info]:" Msg "\033[0;39m\n", LevelStr, __FUNCTION__, __LINE__, ## __VA_ARGS__); \
    }while(0)

#define FILE_ERR(Msg, ...) CHUJIE_PRINTF_RED("Fatal", Msg, ## __VA_ARGS__)
#define MEM_ERR(Msg, ...) CHUJIE_PRINTF_RED("Fatal", Msg, ## __VA_ARGS__)
#define STD_ERR(Msg, ...) CHUJIE_PRINTF("Warning", Msg, ## __VA_ARGS__)
#define DEBUG_INFO(Msg, ...) CHUJIE_PRINTF("Debug", Msg, ## __VA_ARGS__)

char* fgetl(FILE *file);
void strip(char* s);




#endif