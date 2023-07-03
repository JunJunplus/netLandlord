#include "table.h"

Usr_t *g_usr;
//////////////////////////////////////////////////////////////////////////

// 静态变量
//  const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式

//////////////////////////////////////////////////////////////////////////
void slice_str_blank(char *str, uint8_t *arr, uint8_t *num)
{
	const char s[2] = "-"; // 字符串里可以是一个分隔符，也可以是分隔符的集合
	char *token;

	/* 获取第一个子字符串 */
	token = strtok(str, s);

	/* 继续获取其他的子字符串 */
	int count = 0;
	while (token != NULL)
	{
		arr[count] = atoi(token);
		token = strtok(NULL, s);
		count++;
	}

	*num = count;
}
TableFrame *tableInit()
{
	TableFrame *table = malloc(sizeof(TableFrame));

	// 游戏变量
	table->m_wBombTime = 1;
	table->m_bLandScore = 0;
	table->m_wFirstUser = 0;
	table->m_wBankerUser = 0;
	table->m_wCurrentUser = 0;
	table->m_wPlayerCount = 3; // 游戏人数

	memset(table->m_bScoreInfo, 0, sizeof(table->m_bScoreInfo));

	// 运行信息
	table->m_bTurnCardCount = 0;
	table->m_wTurnWiner = 0;
	memset(table->m_bTurnCardData, 0, sizeof(table->m_bTurnCardData));

	// 扑克信息
	memset(table->m_bBackCard, 0, sizeof(table->m_bBackCard));
	memset(table->m_bCardCount, 0, sizeof(table->m_bCardCount));
	memset(table->m_bOutCardCount, 0, sizeof(table->m_bOutCardCount));
	memset(table->m_bHandCardData, 0, sizeof(table->m_bHandCardData));

	return table;
}

// 接口查询
//  void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, Dint dwQueryVer)
//  {
//  	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
//  	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
//  	return NULL;
//  }

// 初始化
//  bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
//  {
//  	//查询接口
//  	assert(pIUnknownEx!=NULL);
//  	m_pITableFrame=GET_OBJECTPTR_INTERFACE(pIUnknownEx,ITableFrame);
//  	if (m_pITableFrame==NULL) return false;

// 	//获取参数
// 	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
// 	assert(m_pGameServiceOption!=NULL);

// 	return true;
// }

// 复位桌子
void RepositTableFrameSink(TableFrame *table)
{
	// 游戏变量
	table->m_wBombTime = 1;
	table->m_bLandScore = 0;
	table->m_wBankerUser = 0;
	table->m_wCurrentUser = 0;
	memset(table->m_bScoreInfo, 0, sizeof(table->m_bScoreInfo));

	// 运行信息
	table->m_bTurnCardCount = 0;
	table->m_wTurnWiner = 0;
	memset(table->m_bTurnCardData, 0, sizeof(table->m_bTurnCardData));

	// 扑克信息
	memset(table->m_bBackCard, 0, sizeof(table->m_bBackCard));
	memset(table->m_bCardCount, 0, sizeof(table->m_bCardCount));
	memset(table->m_bOutCardCount, 0, sizeof(table->m_bOutCardCount));
	memset(table->m_bHandCardData, 0, sizeof(table->m_bHandCardData));

	return;
}

// 开始模式
//  enStartMode __cdecl CTableFrameSink::GetGameStartMode()
//  {
//  	return m_GameStartMode;
//  }

// 游戏状态
//  bool __cdecl CTableFrameSink::IsUserPlaying(int wChairID)
//  {
//  	return true;
//  }

// 游戏开始
bool OnEventGameStart(TableFrame *table)
{
	int fd = open("./score.txt", O_CREAT | O_RDWR);
	if (fd == -1)
	{
		perror("open error");
		return false;
	}
	int lCellScore;
	int ret = read(fd, &lCellScore, sizeof(lCellScore));
	if (ret == -1)
	{
		perror("read error");
		return false;
	}

	// 设置状态
	//  m_pITableFrame->SetGameStatus(GS_WK_SCORE);

	// 混乱扑克
	uint8_t bRandCard[54];
	RandCardList(bRandCard, sizeof(bRandCard) / sizeof(bRandCard[0]));

	// 分发扑克
	for (int i = 0; i < table->m_wPlayerCount; i++)
	{
		table->m_bCardCount[i] = 17;
		memcpy(&table->m_bHandCardData[i], &bRandCard[i * table->m_bCardCount[i]], sizeof(uint8_t) * table->m_bCardCount[i]);
		SortCardList(table->m_bHandCardData[i], table->m_bCardCount[i]);
	}
	memcpy(table->m_bBackCard, &bRandCard[51], sizeof(table->m_bBackCard));
	SortCardList(table->m_bBackCard, 3);

	// 按黑红梅方5的顺序设置首叫用户
	for (int i = 0; i < table->m_wPlayerCount; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (table->m_bHandCardData[i][j] == 0x35)
				table->m_wCurrentUser = i;
		}
	}
	if (table->m_wCurrentUser == -1)
	{
		for (int i = 0; i < table->m_wPlayerCount; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				if (table->m_bHandCardData[i][j] == 0x25)
					table->m_wCurrentUser = i;
			}
		}
	}
	if (table->m_wCurrentUser == -1)
	{
		for (int i = 0; i < table->m_wPlayerCount; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				if (table->m_bHandCardData[i][j] == 0x15)
					table->m_wCurrentUser = i;
			}
		}
	}
	if (table->m_wCurrentUser == -1)
	{
		for (int i = 0; i < table->m_wPlayerCount; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				if (table->m_bHandCardData[i][j] == 0x05)
					table->m_wCurrentUser = i;
			}
		}
	}
	// 设置用户
	table->m_wFirstUser = table->m_wCurrentUser;
	// m_wCurrentUser=m_wFirstUser;
	printf("首叫用户：%d\n", table->m_wFirstUser);
	// 发送扑克
	S_SendCard sendCard;
	sendCard.wCurrentUser = table->m_wCurrentUser;
	for (int i = 0; i < table->m_wPlayerCount; i++)
	{
		SortCardList(table->m_bHandCardData[i], 17);
		memcpy(sendCard.bCardData, &table->m_bHandCardData[i], sizeof(sendCard.bCardData));
		printf("玩家%d:", i);
		for (int j = 0; j < 17; j++)
		{
			printf("%d ", sendCard.bCardData[j]);
		}
		printf("\n");
		// m_pITableFrame->SendTableData(i, SUB_S_SEND_CARD, &SendCard, sizeof(SendCard));
		// m_pITableFrame->SendLookonData(i, SUB_S_SEND_CARD, &SendCard, sizeof(SendCard));
	}
	return true;
}

// 游戏结束
bool OnEventGameEnd(TableFrame *table, int wChairID, uint8_t cbReason)
{
	int fd = open("./score.txt", O_RDWR);
	if (fd == -1)
	{
		perror("open error");
		return false;
	}
	int lCellScore;
	int usrScore;
	read(fd, &usrScore,sizeof(int));
	bool bLandWin = (table->m_bCardCount[table->m_wBankerUser] == 0) ? true : false;

	// 春天判断
	if (wChairID == table->m_wBankerUser)
	{
		int wUser1 = (table->m_wBankerUser + 1) % GAME_PLAYER;
		int wUser2 = (table->m_wBankerUser + 2) % GAME_PLAYER;
		if ((table->m_bOutCardCount[wUser1] == 0) && (table->m_bOutCardCount[wUser2] == 0))
			table->m_wBombTime *= 2;
	}
	else
	{
		if (table->m_bOutCardCount[table->m_wBankerUser] == 1)
			table->m_wBombTime *= 2;
	}

	// 统计积分
	font *f = init_font(60);
	char buf[300] = {0};

	for (int i = 0; i < table->m_wPlayerCount; i++)
	{
		if (i == table->m_wBankerUser)
		{
			lCellScore = table->m_wBombTime * table->m_bLandScore * ((bLandWin == true) ? 2 : -2);
			printf("table->m_wBankerUser[%d]",i);
		}
		else
			lCellScore = (table->m_wBombTime * table->m_bLandScore * ((bLandWin == true) ? -1 : 1));
		char temp[30] = {0};
		sprintf(temp, "玩家:%d---分数:%d\n", i, lCellScore);
		strcat(buf, temp);
		table->m_bBackCard[i] = lCellScore;
		if(i == wChairID)
		{
			usrScore += lCellScore;
			write(fd, &usrScore, sizeof(lCellScore));
		}
	}
	
	

	score_font(lcd_p, f,buf, 0, 0);
	close(fd);
	bmp_show_size("79.bmp", 300, 280);
	// 计算税收
	// if (table->m_pGameServiceOption->wServerType & GAME_GENRE_GOLD)
	// {
	// 	for (int i = 0; i < table->m_wPlayerCount; i++)
	// 	{
	// 		if (GameEnd.lGameScore[i] >= 100L)
	// 		{
	// 			GameEnd.lGameTax += GameEnd.lGameScore[i] / 50L;
	// 			GameEnd.lGameScore[i] = GameEnd.lGameScore[i] * 49L / 50L;
	// 		}
	// 	}
	// }

	// // 发送信息
	// m_pITableFrame->SendTableData(-1, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
	// m_pITableFrame->SendLookonData(-1, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

	// // 修改积分
	// tagScoreInfo ScoreInfo[m_wPlayerCount];
	// memset(&ScoreInfo, 0, sizeof(ScoreInfo));
	// for (int i = 0; i < m_wPlayerCount; i++)
	// {
	// 	ScoreInfo[i].lScore = GameEnd.lGameScore[i];
	// 	ScoreInfo[i].ScoreKind = (GameEnd.lGameScore[i] > 0L) ? enScoreKind_Win : enScoreKind_Lost;
	// }
	// m_pITableFrame->WriteTableScore(ScoreInfo, m_wPlayerCount, GameEnd.lGameTax);
	return true;
	// // 效验参数
	// // assert(pIServerUserItem != NULL);
	// assert(wChairID < table->m_wPlayerCount);

	// // 构造数据
	// S_GameEnd GameEnd;
	// memset(&GameEnd, 0, sizeof(GameEnd));
	// GameEnd.lGameScore[wChairID] = -20 * m_pGameServiceOption->lCellScore;

	// // 剩余扑克
	// uint8_t bCardPos = 0;
	// for (int i = 0; i < m_wPlayerCount; i++)
	// {
	// 	GameEnd.bCardCount[i] = m_bCardCount[i];
	// 	memcpy(&GameEnd.bCardData[bCardPos], m_bHandCardData[i], m_bCardCount[i] * sizeof(uint8_t));
	// 	bCardPos += m_bCardCount[i];
	// }

	// // 发送信息
	// m_pITableFrame->SendTableData(-1, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
	// m_pITableFrame->SendLookonData(-1, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

	// // 修改积分
	// tagScoreInfo ScoreInfo;
	// ScoreInfo.ScoreKind = enScoreKind_Draw;
	// ScoreInfo.lScore = GameEnd.lGameScore[wChairID];
	// m_pITableFrame->WriteUserScore(wChairID, ScoreInfo);

	// // 结束游戏
	// m_pITableFrame->ConcludeGame();
}
#if 0



// 发送场景
// bool __cdecl CTableFrameSink::SendGameScene(int wChiarID, IServerUserItem *pIServerUserItem, uint8_t cbGameStatus, bool bSendSecret)
// {
// 	switch (cbGameStatus)
// 	{
// 	case GS_WK_FREE: // 空闲状态
// 	{
// 		// 构造数据
// 		CMD_S_StatusFree StatusFree;
// 		memset(&StatusFree, 0, sizeof(StatusFree));

// 		// 设置变量
// 		StatusFree.lBaseScore = m_pGameServiceOption->lCellScore;

// 		// 发送场景
// 		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusFree, sizeof(StatusFree));
// 	}
// 	case GS_WK_SCORE: // 叫分状态
// 	{
// 		// 构造数据
// 		CMD_S_StatusScore StatusScore;
// 		memset(&StatusScore, 0, sizeof(StatusScore));

// 		// 设置变量
// 		StatusScore.bLandScore = m_bLandScore;
// 		StatusScore.wCurrentUser = m_wCurrentUser;
// 		StatusScore.lBaseScore = m_pGameServiceOption->lCellScore;
// 		memcpy(StatusScore.bScoreInfo, m_bScoreInfo, sizeof(m_bScoreInfo));
// 		memcpy(StatusScore.bCardData, m_bHandCardData[wChiarID], m_bCardCount[wChiarID]);

// 		// 发送场景
// 		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusScore, sizeof(StatusScore));
// 	}
// 	case GS_WK_PLAYING: // 游戏状态
// 	{
// 		// 构造数据
// 		CMD_S_StatusPlay StatusPlay;
// 		memset(&StatusPlay, 0, sizeof(StatusPlay));

// 		// 设置变量
// 		StatusPlay.wBombTime = m_wBombTime;
// 		StatusPlay.wLandUser = m_wBankerUser;
// 		StatusPlay.wCurrentUser = m_wCurrentUser;
// 		StatusPlay.wLastOutUser = m_wTurnWiner;
// 		StatusPlay.bLandScore = m_bLandScore;
// 		StatusPlay.bTurnCardCount = m_bTurnCardCount;
// 		StatusPlay.lBaseScore = m_pGameServiceOption->lCellScore;
// 		memcpy(StatusPlay.bBackCard, m_bBackCard, sizeof(m_bBackCard));
// 		memcpy(StatusPlay.bCardCount, m_bCardCount, sizeof(m_bCardCount));
// 		memcpy(StatusPlay.bTurnCardData, m_bTurnCardData, sizeof(uint8_t) * m_bTurnCardCount);
// 		memcpy(StatusPlay.bCardData, m_bHandCardData[wChiarID], sizeof(uint8_t) * m_bCardCount[wChiarID]);

// 		// 发送场景
// 		return m_pITableFrame->SendGameScene(pIServerUserItem, &StatusPlay, sizeof(StatusPlay));
// 	}
// 	}

// 	return false;
// }

// 定时器事件
// bool __cdecl CTableFrameSink::OnTimerMessage(int wTimerID, WPARAM wBindParam)
// {
// 	return false;
// }

// 游戏消息处理
// bool __cdecl CTableFrameSink::OnGameMessage(int wSubCmdID, const void *pDataBuffer, int wDataSize, IServerUserItem *pIServerUserItem)
// {
// 	switch (wSubCmdID)
// 	{
// 	case SUB_C_LAND_SCORE: // 用户叫分
// 	{
// 		// 效验数据
// 		assert(wDataSize == sizeof(CMD_C_LandScore));
// 		if (wDataSize != sizeof(CMD_C_LandScore))
// 			return false;

// 		// 用户效验
// 		tagServerUserData *pUserData = pIServerUserItem->GetUserData();
// 		if (pUserData->cbUserStatus != US_PLAY)
// 			return true;

// 		// 消息处理
// 		CMD_C_LandScore *pLandScore = (CMD_C_LandScore *)pDataBuffer;
// 		return OnUserLandScore(pUserData->wChairID, pLandScore->bLandScore);
// 	}
// 	case SUB_C_OUT_CART: // 用户出牌
// 	{
// 		// 变量定义
// 		CMD_C_OutCard *pOutCard = (CMD_C_OutCard *)pDataBuffer;
// 		int wHeadSize = sizeof(CMD_C_OutCard) - sizeof(pOutCard->bCardData);

// 		// 效验数据
// 		assert(wDataSize >= wHeadSize);
// 		if (wDataSize < wHeadSize)
// 			return false;
// 		assert(wDataSize == (wHeadSize + pOutCard->bCardCount * sizeof(pOutCard->bCardData[0])));
// 		if (wDataSize != (wHeadSize + pOutCard->bCardCount * sizeof(pOutCard->bCardData[0])))
// 			return false;

// 		// 用户效验
// 		tagServerUserData *pUserData = pIServerUserItem->GetUserData();
// 		if (pUserData->cbUserStatus != US_PLAY)
// 			return true;

// 		// 消息处理
// 		return OnUserOutCard(pUserData->wChairID, pOutCard->bCardData, pOutCard->bCardCount);
// 	}
// 	case SUB_C_PASS_CARD:
// 	{
// 		// 用户效验
// 		tagServerUserData *pUserData = pIServerUserItem->GetUserData();
// 		if (pUserData->cbUserStatus != US_PLAY)
// 			return true;

// 		// 消息处理
// 		return OnUserPassCard(pUserData->wChairID);
// 	}
// 	}

// 	return true;
// }

// 框架消息处理
// bool __cdecl CTableFrameSink::OnFrameMessage(int wSubCmdID, const void *pDataBuffer, int wDataSize, IServerUserItem *pIServerUserItem)
// {
// 	return false;
// }
#endif

// 叫分事件
bool OnUserLandScore(TableFrame *table, int wChairID, uint8_t bLandScore)
{
	// 效验状态
	// if (table->m_pITableFrame->GetGameStatus() != GS_WK_SCORE)
	// 	return true;
	// if (wChairID != table->m_wCurrentUser)
	// 	return false;

	// 效验参数
	printf("用户叫分:%d\n", bLandScore);
	if (((bLandScore > 3) && (bLandScore != 255)) || (bLandScore <= table->m_bLandScore))
		return false;

	// 设置变量
	if (bLandScore != 255)
	{
		table->m_bLandScore = bLandScore;
		table->m_wBankerUser = table->m_wCurrentUser;
	}
	table->m_bScoreInfo[wChairID] = bLandScore;

	// 开始判断
	if ((table->m_bLandScore == 3) || (table->m_wFirstUser == (wChairID + 1) % table->m_wPlayerCount))
	{
		printf("table->m_wFirstUser [%d] table->m_wPlayerCount [%d]", table->m_wFirstUser, table->m_wPlayerCount);
		// 设置变量
		if (table->m_bLandScore == 0)
			table->m_bLandScore = 1;
		if (table->m_wBankerUser == -1)
			table->m_wBankerUser = table->m_wFirstUser;

		// 设置状态
		// m_pITableFrame->SetGameStatus(GS_WK_PLAYING);

		// 发送底牌
		table->m_bCardCount[table->m_wBankerUser] = 20;
		printf("玩家%d是地主\n", table->m_wBankerUser);
		memcpy(&table->m_bHandCardData[table->m_wBankerUser][17], table->m_bBackCard, sizeof(table->m_bBackCard));
		SortCardList(table->m_bHandCardData[table->m_wBankerUser], table->m_bCardCount[table->m_wBankerUser]);

		// 出牌信息
		table->m_bTurnCardCount = 0;
		table->m_wTurnWiner = table->m_wBankerUser;
		table->m_wCurrentUser = table->m_wBankerUser;


		return false;
	}

	// 设置用户
	table->m_wCurrentUser = (wChairID + 1) % table->m_wPlayerCount;

	return true;
}

// 用户出牌
bool OnUserOutCard(TableFrame *table, int wChairID, uint8_t bCardData[], uint8_t bCardCount)
{

	if(bCardCount == 0)
		return false;
	SortCardList(bCardData, bCardCount);
	// 类型判断
	uint8_t bCardType = GetCardType(bCardData, bCardCount);
	if (bCardType == CT_INVALID)
		return false;

	// 更随出牌
	if (table->m_bTurnCardCount == 0)
		table->m_bTurnCardCount = bCardCount;
	else if (CompareCard(bCardData, table->m_bTurnCardData, bCardCount, table->m_bTurnCardCount) == false)
		return false;
	printf("用户出的牌:");
	for (int i = 0; i < bCardCount; i++)
	{
		printf("%d ", GetCardValue(bCardData[i]));
	}
	printf("\n");
	// 删除扑克
	if (RemoveCard(bCardData, bCardCount, table->m_bHandCardData[wChairID], table->m_bCardCount[wChairID]) == false)
		return false;
	table->m_bCardCount[wChairID] -= bCardCount;

	// 出牌记录
	table->m_bTurnCardCount = bCardCount;
	table->m_bOutCardCount[wChairID]++;
	memcpy(table->m_bTurnCardData, bCardData, sizeof(uint8_t) * bCardCount);

	// 炸弹判断
	if ((bCardType == CT_BOMB_CARD) || (bCardType == CT_MISSILE_CARD))
		table->m_wBombTime *= 2;

	// 切换用户
	table->m_wTurnWiner = wChairID;
	if (table->m_bCardCount[wChairID] != 0)
	{
		if (bCardType != CT_MISSILE_CARD)
			table->m_wCurrentUser = (table->m_wCurrentUser + 1) % table->m_wPlayerCount;
	}
	else
		table->m_wCurrentUser = -1;

	// 构造数据
	S_OutCard outCard;
	outCard.bCardCount = bCardCount;
	outCard.wOutCardUser = wChairID;
	outCard.wCurrentUser = table->m_wCurrentUser;
	memcpy(outCard.bCardData, table->m_bTurnCardData, table->m_bTurnCardCount * sizeof(uint8_t));

	// 发送数据
	// int wSendSize = sizeof(OutCard) - sizeof(OutCard.bCardData) + OutCard.bCardCount * sizeof(uint8_t);
	// table->m_pITableFrame->SendTableData(-1, SUB_S_OUT_CARD, &OutCard, wSendSize);
	// m_pITableFrame->SendLookonData(-1, SUB_S_OUT_CARD, &OutCard, wSendSize);

	// 出牌最大
	if (bCardType == CT_MISSILE_CARD)
		table->m_bTurnCardCount = 0;

	// 结束判断
	// if (table->m_wCurrentUser == -1)
	// 	OnEventGameEnd(wChairID, NULL, GER_NORMAL);

	return true;
}

// 用户放弃
bool OnUserPassCard(TableFrame *table, int wChairID)
{
	// 效验状态
	// if (table->m_pITableFrame->GetGameStatus() != GS_WK_PLAYING)
	// 	return true;
	// if ((wChairID != m_wCurrentUser) || (m_bTurnCardCount == 0))
	// 	return false;

	// 设置变量
	table->m_wCurrentUser = (table->m_wCurrentUser + 1) % table->m_wPlayerCount;
	if (table->m_wCurrentUser == table->m_wTurnWiner)
		table->m_bTurnCardCount = 0;

	// 发送数据
	// CMD_S_PassCard PassCard;
	// PassCard.wPassUser = wChairID;
	// PassCard.wCurrentUser = m_wCurrentUser;
	// PassCard.bNewTurn = (m_bTurnCardCount == 0) ? TRUE : FALSE;
	// m_pITableFrame->SendTableData(-1, SUB_S_PASS_CARD, &PassCard, sizeof(PassCard));
	// m_pITableFrame->SendLookonData(-1, SUB_S_PASS_CARD, &PassCard, sizeof(PassCard));

	return true;
}

//////////////////////////////////////////////////////////////////////////
