#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE
#include <stdint.h>
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						ID_LAND								//游戏 I D
#define GAME_PLAYER					3									//游戏人数
#define GAME_NAME					TEXT("斗地主")						//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//游戏类型

//游戏状态
#define GS_WK_FREE					GS_FREE								//等待开始
#define GS_WK_SCORE					GS_PLAYING							//叫分状态
#define GS_WK_PLAYING				GS_PLAYING+1						//游戏进行

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_SEND_CARD				100									//发牌命令
#define SUB_S_LAND_SCORE			101									//叫分命令
#define SUB_S_GAME_START			102									//游戏开始
#define SUB_S_OUT_CARD				103									//用户出牌
#define SUB_S_PASS_CARD				104									//放弃出牌
#define SUB_S_GAME_END				105									//游戏结束

//游戏状态
typedef struct CMD_S_StatusFree
{
	long							lBaseScore;							//基础积分
}S_StatusFree;

//游戏状态
typedef struct CMD_S_StatusScore
{
	uint8_t							bLandScore;							//地主分数
	long							lBaseScore;							//基础积分
	int				 				wCurrentUser;						//当前玩家
	uint8_t							bScoreInfo[3];						//叫分信息
	uint8_t							bCardData[17];						//手上扑克
}S_StatusScore;

//游戏状态
typedef struct CMD_S_StatusPlay
{
	int								wLandUser;							//坑主玩家
	int								wBombTime;							//炸弹倍数
	long							lBaseScore;							//基础积分
	uint8_t							bLandScore;							//地主分数
	int								wLastOutUser;						//出牌的人
	int				 				wCurrentUser;						//当前玩家
	uint8_t							bBackCard[3];						//底牌扑克
	uint8_t							bCardData[20];						//手上扑克
	uint8_t							bCardCount[3];						//扑克数目
	uint8_t							bTurnCardCount;						//基础出牌
	uint8_t							bTurnCardData[20];					//出牌列表
}S_StatusPlay;

//发送扑克
typedef struct CMD_S_SendCard
{
	int				 				wCurrentUser;						//当前玩家
	uint8_t							bCardData[17];						//扑克列表
}S_SendCard;

//用户叫分
typedef struct CMD_S_LandScore
{
	int								bLandUser;							//叫分玩家
	int				 				wCurrentUser;						//当前玩家
	uint8_t							bLandScore;							//上次叫分
	uint8_t							bCurrentScore;						//当前叫分
}S_LandScore;

//游戏开始
typedef struct CMD_S_GameStart
{
	int				 				wLandUser;							//地主玩家
	uint8_t							bLandScore;							//地主分数
	int				 				wCurrentUser;						//当前玩家
	uint8_t							bBackCard[3];						//底牌扑克
}S_GameStart;

//用户出牌
typedef struct CMD_S_OutCard
{
	uint8_t							bCardCount;							//出牌数目
	int				 				wCurrentUser;						//当前玩家
	int								wOutCardUser;						//出牌玩家
	uint8_t							bCardData[20];						//扑克列表
}S_OutCard;

//放弃出牌
typedef struct CMD_S_PassCard
{
	uint8_t							bNewTurn;							//一轮开始
	int				 				wPassUser;							//放弃玩家
	int				 				wCurrentUser;						//当前玩家
}PassCard;

//游戏结束
typedef struct CMD_S_GameEnd
{
	long							lGameTax;							//游戏税收
	long							lGameScore[3];						//游戏积分
	uint8_t							bCardCount[3];						//扑克数目
	uint8_t							bCardData[54];						//扑克列表 
}S_GameEnd;

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_LAND_SCORE			1									//用户叫分
#define SUB_C_OUT_CART				2									//用户出牌
#define SUB_C_PASS_CARD				3									//放弃出牌

//用户叫分
typedef struct CMD_C_LandScore
{
	uint8_t							bLandScore;							//地主分数
}C_LandScore;

//出牌数据包
typedef struct CMD_C_OutCard
{
	uint8_t							bCardCount;							//出牌数目
	uint8_t							bCardData[20];						//扑克列表
}C_OutCard;

//////////////////////////////////////////////////////////////////////////

#endif