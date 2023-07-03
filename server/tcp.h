#pragma once 
#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include "table.h"


#define MAX_EVENTS 1024 /*监听上限*/
#define MAX_USR    3    /*监听的游戏用户上限*/
#define BUFLEN 4096     /*缓存区大小*/
#define SERV_PORT 6666  /*端口号*/

void recvdata(int fd, int events, void *arg);
void senddata(int fd, int events, void *arg);

/*描述就绪文件描述符的相关信息*/
struct myevent_s
{
    int fd;                                             // 要监听的文件描述符
    int events;                                         // 对应的监听事件，EPOLLIN和EPLLOUT
    void *arg;                                          // 指向自己结构体指针
    void (*call_back)(int fd, int events, void *arg);   // 回调函数
    int status;                                         // 是否在监听:1->在红黑树上(监听), 0->不在(不监听)
    int len;
    long last_active;                                   // 记录每次加入红黑树 g_efd 的时间值
    char buf[BUFLEN];                                   // 游戏数据，需要转成Usr_t结构体型
    // TableFrame * table;                                 // 游戏数据
};

int g_efd; // 全局变量，作为红黑树根 

struct myevent_s g_events[MAX_EVENTS + 1]; // 自定义结构体类型数组. +1-->listen fd

void eventset(struct myevent_s *ev, int fd, void (*call_back)(int fd, int events, void *arg), void *arg);
void eventadd(int efd, int events, struct myevent_s *ev);
void eventdel(int efd, struct myevent_s *ev);
void acceptconn(int lfd, int events, void *arg);
void recvdata(int fd, int events, void *arg);
void senddata(int fd, int events, void *arg);
void initlistensocket(int efd, short port);
void *epoll_loop(void *arg);

#include "Organize_game.h"