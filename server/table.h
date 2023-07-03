#pragma once

#include "card.h"
#include "cmd_land.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <time.h>


//////////////////////////////////////////////////////////////////////////

// 游戏桌子类
typedef struct TableFrameSink
{
    // 游戏变量
    int m_wBombTime;            // 炸弹倍数
    uint8_t m_bLandScore;       // 地主分数
    int m_wFirstUser;           // 首叫用户
    int m_wBankerUser;          // 庄家用户
    int m_wCurrentUser;         // 当前玩家
    uint8_t m_bScoreInfo[3];    // 叫分信息

    // 运行信息

    int m_wTurnWiner;            // 胜利玩家
    uint8_t m_bTurnCardCount;    // 出牌数目
    uint8_t m_bTurnCardData[20]; // 出牌列表

    // 扑克信息

    uint8_t m_bBackCard[3];         // 游戏底牌
    uint8_t m_bCardCount[3];        // 扑克数目
    uint8_t m_bOutCardCount[3];     // 出牌次数
    uint8_t m_bHandCardData[3][20]; // 手上扑克

    // 属性变量
    int m_wPlayerCount; // 游戏人数

} TableFrame;

typedef enum{
    LOGIN = 0,                      // 用户登录等待阶段，等待服务器分牌
    CALL_LANDLORD,                  // 叫地主
    GAMING,                         // 正式游戏阶段
    FINISH                          // 游戏结束                         
}usr_stage;


//定义一个结构体包含了用户的所有阶段以及用户游戏阶段的数据（桌子对象）
typedef struct{
    uint8_t     usr_id;             //  用户
    bool        usr_turn;           //  用户回合标志位
    uint8_t     Table_id;           //  桌子号
    usr_stage   Stage;              //  用户状态
    TableFrame  Table;              //  桌子对象
}Usr_t;


#define GER_NORMAL						0x00								
#define GER_DISMISS						0x01								
#define GER_USER_LEFT					0x02
// 初始化
extern TableFrame *tableInit();
// 游戏开始
bool OnEventGameStart(TableFrame *table);
// 叫分事件
extern bool OnUserLandScore(TableFrame *table, int wChairID, uint8_t bLandScore);
// 用户出牌
extern bool OnUserOutCard(TableFrame *table, int wChairID, uint8_t bCardData[], uint8_t bCardCount);
// 复位桌子
void RepositTableFrameSink(TableFrame *table);
// 用户放弃
extern bool OnUserPassCard(TableFrame *table, int wChairID);
// 游戏结束
extern bool OnEventGameEnd(TableFrame *table, int wChairID, uint8_t cbReason);

void slice_str_blank(char *str, uint8_t *arr, uint8_t *num);

//////////////////////////////////////////////////////////////////////////
