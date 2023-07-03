#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
// 扑克类型
typedef enum _cardKind
{
    CT_INVALID,                // 错误类型
    CT_SINGLE,                 // 单牌类型
    CT_DOUBLE,                 // 对牌类型
    CT_THREE,                  // 三条类型
    CT_ONE_LINE,               // 单连类型
    CT_DOUBLE_LINE,            // 对连类型
    CT_THREE_LINE,             // 三连类型
    CT_THREE_LINE_TAKE_ONE,    // 三带一单
    CT_THREE_LINE_TAKE_DOUBLE, // 三带一对
    CT_FOUR_LINE_TAKE_ONE,     // 四带两单
    CT_FOUR_LINE_TAKE_DOUBLE,  // 四带两对
    CT_BOMB_CARD,              // 炸弹类型
    CT_MISSILE_CARD,           // 火箭类型
} cardkind;

// 数值掩码
#define LOGIC_MASK_COLOR 0xF0 // 花色掩码
#define LOGIC_MASK_VALUE 0x0F // 数值掩码

typedef struct _tagAnalyseResult
{
    uint8_t bFourCount;            // 四张数目
    uint8_t bThreeCount;           // 三张数目
    uint8_t bDoubleCount;          // 两张数目
    uint8_t bSignedCount;          // 单张数目
    uint8_t bFourLogicVolue[5];    // 四张列表
    uint8_t bThreeLogicVolue[6];   // 三张列表
    uint8_t bDoubleLogicVolue[10]; // 两张列表
    uint8_t m_bSCardData[20];      // 手上扑克
    uint8_t m_bDCardData[20];      // 手上扑克
    uint8_t m_bTCardData[20];      // 手上扑克
    uint8_t m_bFCardData[20];      // 手上扑克
}tagAnalyseResult;

static const uint8_t g_bCardListData[54]; // 扑克数据

uint8_t GetCardType(const uint8_t bCardData[], uint8_t bCardCount);
// 获取数值
uint8_t GetCardValue(uint8_t bCardData);
// 获取花色
uint8_t GetCardColor(uint8_t bCardData); 

// 控制函数

// 排列扑克
void SortCardList(uint8_t *bCardData, uint8_t bCardCount);
// 混乱扑克
void RandCardList(uint8_t bCardBuffer[], uint8_t bBufferCount);
// 删除扑克
bool RemoveCard(const uint8_t bRemoveCard[], uint8_t bRemoveCount, uint8_t bCardData[], uint8_t bCardCount);

// 逻辑函数

// 逻辑数值
uint8_t GetCardLogicValue(uint8_t bCardData);
// 对比扑克
bool CompareCard(const uint8_t bFirstList[], const uint8_t bNextList[], uint8_t bFirstCount, uint8_t bNextCount);

// 内部函数

// 分析扑克
void AnalysebCardData(const uint8_t bCardData[], uint8_t bCardCount, tagAnalyseResult *AnalyseResult);