#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <pthread.h>

#include <linux/input.h>

#include "lcd.h"
#include "table.h"
#include "tcp.h"
/*
触摸点尺寸 1024-600
types:3 code:0 value:528    x
types:3 code:1 value:326    y
types:1 code:330 value:1    按下
types:1 code:330 value:0    松开
*/
//触摸事件种类
typedef enum touchEvKind
{
    NON,                //无事件
    PRESS,              //按下
    LOOSEN,             //松开
    CLICK,              //单击
    DCLICK,             //双击
    LSLIDE,             //左滑
    RSLIDE,             //右滑
    USLIDE,             //上滑
    DSLIDE,             //下滑
    LLSLIDE,
    RRSLIDE
}touchEvkind_t;

//触摸点坐标
typedef struct touchPos
{
    int x;
    int y;
}touchPos_t;

//触摸事件信息
typedef struct touchEvInfo
{
    struct timespec startTime;      //事件开始时间
    struct timespec endTime;        //事件结束时间
    touchPos_t startPos;            //开始位置
    touchPos_t endPos;              //结束位置
    int evKind;                     //事件种类
}touchEvInfo_t;

typedef enum sys_mode
{
    SNON,
    SACC,
    SPASSWD,
    SLOGIN,
    SCHOOSE,
    SVIDEO,
    SGAME
}sys_mode_t;

int gameOpt;


extern touchPos_t firstPos;                //记录上一次按压的位置
extern touchPos_t p;
extern char acc[20];
extern char passwd[20];
extern int num;

typedef struct _touch_arr{
    uint8_t *arr_card;
    uint8_t count_card;
}touch_arr;

touch_arr *ptouch_arr;

//初始化
extern void touch_init();

//销毁
extern void touch_destory();

//触发触摸事件
extern int trigger_touch_event();

extern void *touch_wait(void *arg);


extern void *touch_wait_longin(void *arg);

extern char touch_login(touchPos_t temppos);

extern char choose_mode_touch();

extern void *touch_step_wait(void *arg);

//////////////////////////////////////////////////////////////////
extern void* touch_select_card(void *arg);

extern void* touch_land_score(void *arg);

extern void *touch_client_server(void *arg);
//////////////////////////////////////////////////////////////////
