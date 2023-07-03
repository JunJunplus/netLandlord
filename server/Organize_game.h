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


#include "table.h"
#include "tcp.h"

#define MAX_EVENTS 1024
#define MAX_TABLE  30


//设置用户为准备状态
void setUsr_ready(int usr_num, int t_id, Usr_t * usr);

//初始化桌子管理数组
void tableArr_Init();

//每当有一位用户登录之后，检查一下是否可以开一桌游戏
int check_tablegameStart();

//初始化桌子
TableFrame * init_table(int table_num);

//检测用户断线退出
void check_usr_exit(struct myevent_s * usr);

//查看哪张桌子空闲可做
int check_table2play();

//分析用户发回来的数据
void analyze_data(struct myevent_s * data);

//同步三家用户数据并且发送数据
void copy_and_sendData(Usr_t * cur_usr);


// 查看当前用户入桌情况
void __check_table();