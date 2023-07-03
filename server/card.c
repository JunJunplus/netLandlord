#include "card.h"

//////////////////////////////////////////////////////////////////////////
//静态变量

//扑克数据
static const uint8_t g_bCardListData[54]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x41,0x42,
};

uint8_t GetCardValue(uint8_t bCardData) 
{ 
    return bCardData & LOGIC_MASK_VALUE;
}

uint8_t GetCardColor(uint8_t bCardData) 
{ 
    return bCardData & LOGIC_MASK_COLOR; 
}
//获取类型
uint8_t GetCardType(const uint8_t bCardData[], uint8_t bCardCount)
{
	//开始分析
	switch (bCardCount)
	{
	case 1: //单牌
		{
			return CT_SINGLE;
		}
	case 2:	//对牌和火箭
		{
			if ((bCardData[0]==0x42)&&(bCardData[1]==0x41)) return CT_MISSILE_CARD;
			return (GetCardLogicValue(bCardData[0])==GetCardLogicValue(bCardData[1]))?CT_DOUBLE:CT_INVALID;
		}
	case 3:
	case 4:	//连牌和炸弹
		{
			uint8_t bLogicValue=GetCardLogicValue(bCardData[0]);
            uint8_t i;
			for (i=1;i<bCardCount;i++)
			{
				if (bLogicValue!=GetCardLogicValue(bCardData[i])) break;
			}
			if (i==bCardCount) return (bCardCount==3)?CT_THREE:CT_BOMB_CARD;
			if (bCardCount==3) return CT_INVALID;
			break;
		}
	}

	//其他牌型
	if (bCardCount>=4)
	{
		//分析扑克
		tagAnalyseResult AnalyseResult;
		AnalysebCardData(bCardData,bCardCount,&AnalyseResult);

		//四牌判断
		if (AnalyseResult.bFourCount>0)
		{
			if ((AnalyseResult.bFourCount==1)&&(bCardCount==5)) return CT_FOUR_LINE_TAKE_ONE;
			if ((AnalyseResult.bFourCount==1)&&(AnalyseResult.bDoubleCount==1)&&(bCardCount==6)) return CT_FOUR_LINE_TAKE_ONE;
			if ((AnalyseResult.bFourCount==1)&&(AnalyseResult.bDoubleCount==2)&&(bCardCount==8)) return CT_FOUR_LINE_TAKE_DOUBLE;
			if ((AnalyseResult.bFourCount==1)&&(bCardCount==6)) return CT_FOUR_LINE_TAKE_DOUBLE;

			return CT_INVALID;
		}

		//三牌判断
		if (AnalyseResult.bThreeCount>0)
		{
			//连牌判断
			if (AnalyseResult.bThreeCount>1)
			{
				if (AnalyseResult.bThreeLogicVolue[0]==15) return CT_INVALID;
				for (uint8_t i=1;i<AnalyseResult.bThreeCount;i++)
				{
					if (AnalyseResult.bThreeLogicVolue[i]!=(AnalyseResult.bThreeLogicVolue[0]-i)) return CT_INVALID;
				}
			}

			//牌形判断
			if (AnalyseResult.bThreeCount*3==bCardCount) return CT_THREE_LINE;
			if (AnalyseResult.bThreeCount==(bCardCount-AnalyseResult.bThreeCount*3)) return CT_THREE_LINE_TAKE_ONE;
			if ((AnalyseResult.bDoubleCount==AnalyseResult.bThreeCount)&&(bCardCount==(AnalyseResult.bThreeCount*3+AnalyseResult.bDoubleCount*2))) return CT_THREE_LINE_TAKE_DOUBLE;

			return CT_INVALID;
		}

		//两张类型
		if (AnalyseResult.bDoubleCount>=3)
		{
			//二连判断
			if (AnalyseResult.bDoubleLogicVolue[0]!=15)
			{
				for (uint8_t i=1;i<AnalyseResult.bDoubleCount;i++)
				{
					if (AnalyseResult.bDoubleLogicVolue[i]!=(AnalyseResult.bDoubleLogicVolue[0]-i)) return CT_INVALID;
				}
				if (AnalyseResult.bDoubleCount*2==bCardCount) return CT_DOUBLE_LINE;
			}

			return CT_INVALID;
		}
		
		//单张判断
		if ((AnalyseResult.bSignedCount>=5)&&(AnalyseResult.bSignedCount==bCardCount))
		{
			uint8_t bLogicValue=GetCardLogicValue(bCardData[0]);
			if (bLogicValue>=15) return CT_INVALID;
			for (uint8_t i=1;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(bCardData[i])!=(bLogicValue-i)) return CT_INVALID;
			}

			return CT_ONE_LINE;
		}
		
		return CT_INVALID;
	}

	return CT_INVALID;
}

//排列扑克
void SortCardList(uint8_t *bCardData, uint8_t bCardCount)
{
	//转换数值
	uint8_t bLogicVolue[20];
	for (uint8_t i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);

	//排序操作
	bool bSorted=true;
	uint8_t bTempData,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (uint8_t i=0;i<bLast;i++)
		{
			if ((bLogicVolue[i]<bLogicVolue[i+1])||
			   ((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
			{
				//交换位置
				bTempData=bCardData[i];
				bCardData[i]=bCardData[i+1];
				bCardData[i+1]=bTempData;
				bTempData=bLogicVolue[i];
				bLogicVolue[i]=bLogicVolue[i+1];
				bLogicVolue[i+1]=bTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//混乱扑克
void RandCardList(uint8_t bCardBuffer[], uint8_t bBufferCount)
{
    srand((unsigned int)time(NULL));
	//混乱准备
	uint8_t bCardData[sizeof(g_bCardListData)];
	memcpy(bCardData,g_bCardListData,sizeof(g_bCardListData));

	//混乱扑克
	uint8_t bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

//删除扑克
bool RemoveCard(const uint8_t bRemoveCard[], uint8_t bRemoveCount, uint8_t bCardData[], uint8_t bCardCount)
{
	//检验数据
	//ASSERT(bRemoveCount<=bCardCount);

	//定义变量
	uint8_t bDeleteCount=0,bTempCardData[20];
    //
	if (bCardCount>20) return false;
	memcpy(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//置零扑克
	for (uint8_t i=0;i<bRemoveCount;i++)
	{
		for (uint8_t j=0;j<bCardCount;j++)
		{
			if (bRemoveCard[i]==bTempCardData[j])
			{
				bDeleteCount++;
				bTempCardData[j]=0;
				break;
			}
		}
	}
	if (bDeleteCount!=bRemoveCount) return false;

	//清理扑克
	uint8_t bCardPos=0;
	for (uint8_t i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}

//逻辑数值
uint8_t GetCardLogicValue(uint8_t bCardData)
{
	//扑克属性
	uint8_t bCardColor=GetCardColor(bCardData);
	uint8_t bCardValue=GetCardValue(bCardData);

	//转换数值
	if (bCardColor==0x40) return bCardValue+15;
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//对比扑克
bool CompareCard(const uint8_t bFirstList[], const uint8_t bNextList[], uint8_t bFirstCount, uint8_t bNextCount)
{
	//获取类型
	uint8_t bNextType=GetCardType(bNextList,bNextCount);
	uint8_t bFirstType=GetCardType(bFirstList,bFirstCount);

	//类型判断
	if (bFirstType==CT_INVALID) return false;
	if (bFirstType==CT_MISSILE_CARD) return true;

	//炸弹判断
	if ((bFirstType==CT_BOMB_CARD)&&(bNextType!=CT_BOMB_CARD)) return true;
	if ((bFirstType!=CT_BOMB_CARD)&&(bNextType==CT_BOMB_CARD)) return false;

	//规则判断
	if ((bFirstType!=bNextType)||(bFirstCount!=bNextCount)) return false;

	//开始对比
	switch (bNextType)
	{
	case CT_SINGLE:
	case CT_DOUBLE:
	case CT_THREE:
	case CT_ONE_LINE:
	case CT_DOUBLE_LINE:
	case CT_THREE_LINE:
	case CT_BOMB_CARD:
		{
			uint8_t bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			uint8_t bNextLogicValue=GetCardLogicValue(bNextList[0]);
			return bFirstLogicValue>bNextLogicValue;
		}
	case CT_THREE_LINE_TAKE_ONE:
	case CT_THREE_LINE_TAKE_DOUBLE:
		{
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(bNextList,bNextCount,&NextResult);
			AnalysebCardData(bFirstList,bFirstCount,&FirstResult);
			return FirstResult.bThreeLogicVolue[0]>NextResult.bThreeLogicVolue[0];
		}
	case CT_FOUR_LINE_TAKE_ONE:
	case CT_FOUR_LINE_TAKE_DOUBLE:
		{
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(bNextList,bNextCount,&NextResult);
			AnalysebCardData(bFirstList,bFirstCount,&FirstResult);
			return FirstResult.bFourLogicVolue[0]>NextResult.bFourLogicVolue[0];
		}
	}
	
	return false;
}

//分析扑克
void AnalysebCardData(const uint8_t bCardData[], uint8_t bCardCount, tagAnalyseResult *AnalyseResult)
{
	//变量定义
	uint8_t bSameCount=1,bCardValueTemp=0;
	uint8_t bLogicValue=GetCardLogicValue(bCardData[0]);

	//设置结果
	memset(AnalyseResult,0,sizeof(tagAnalyseResult));

	//扑克分析
	for (uint8_t i=1;i<bCardCount;i++)
	{
		//获取扑克
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//保存结果
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 2:		//两张
				{
					memcpy(&(AnalyseResult->m_bDCardData[AnalyseResult->bDoubleCount*2]),&bCardData[i-2+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],2);
					AnalyseResult->bDoubleLogicVolue[AnalyseResult->bDoubleCount++]=bLogicValue;
					break;
				}
			case 3:		//三张
				{
					memcpy(&AnalyseResult->m_bTCardData[AnalyseResult->bThreeCount*3],&bCardData[i-3+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],3);
					AnalyseResult->bThreeLogicVolue[AnalyseResult->bThreeCount++]=bLogicValue;
					break;
				}
			case 4:		//四张
				{
					memcpy(&AnalyseResult->m_bFCardData[AnalyseResult->bFourCount*4],&bCardData[i-4+((i==bCardCount-1&&bCardValueTemp==bLogicValue)?1:0)],4);
					AnalyseResult->bFourLogicVolue[AnalyseResult->bFourCount++]=bLogicValue;
					break;
				}
			}
		}

		//设置变量
		if (bCardValueTemp!=bLogicValue)
		{
			if(bSameCount==1)
			{
				if(i!=bCardCount-1)
					AnalyseResult->m_bSCardData[AnalyseResult->bSignedCount++]=bCardData[i-1];
				else
				{
					AnalyseResult->m_bSCardData[AnalyseResult->bSignedCount++]=bCardData[i-1];
					AnalyseResult->m_bSCardData[AnalyseResult->bSignedCount++]=bCardData[i];
				}
			}
			else
			{
				if(i==bCardCount-1)
					AnalyseResult->m_bSCardData[AnalyseResult->bSignedCount++]=bCardData[i];
			}
			bSameCount=1;
			bLogicValue=bCardValueTemp;

		}
	}

	//单牌数目
	uint8_t bOtherCount=AnalyseResult->bDoubleCount*2+AnalyseResult->bThreeCount*3+AnalyseResult->bFourCount*4;
	return;
}

