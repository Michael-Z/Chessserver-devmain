#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE CGameLogic::m_cbCardListData[MAX_CARD]=
{	
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount==MAX_COUNT);

	//ը������
	BYTE bSameCount = 0;
	SortCardList(cbCardData,cbCardCount);	

	BYTE cbKingCount=0;
	for(BYTE i=0;i<cbCardCount;i++)
	{
		if( GetCardValue(cbCardData[i])>10 )
		{
			cbKingCount++;
		}		
	}
	BYTE cbSameCount=1;
	for(BYTE i=0;i<cbCardCount-1;i++)
	{
		if( GetCardValue(cbCardData[i]) == GetCardValue(cbCardData[i+1]) )
		{
			cbSameCount++;
		}		
	}

	if(cbKingCount==MAX_COUNT && cbSameCount==MAX_COUNT) return OX_BIG_THREE_KING;
	if(cbKingCount==MAX_COUNT && cbSameCount!=MAX_COUNT) return OX_MIX_THREE_KING;
	if(cbKingCount!=MAX_COUNT && cbSameCount==MAX_COUNT) return OX_SMALL_THREE_KING;
	

	BYTE bTemp[MAX_COUNT];
	BYTE bSum=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		bTemp[i]=GetCardLogicValue(cbCardData[i]);
		bSum+=bTemp[i];
	}	

	return bSum%10;
}

//��ȡ����
BYTE CGameLogic::GetTimes(BYTE cbCardData[], BYTE cbCardCount)
{
	if(cbCardCount!=MAX_COUNT)return 0;

	BYTE bTimes=GetCardType(cbCardData,MAX_COUNT);
	if(bTimes<8)return 1;
	else if(bTimes==8)return 2;
	else if(bTimes==9)return 3;
	else if(bTimes==11)return 5;
	else if(bTimes==12)return 7;
	else if(bTimes==13)return 9;

	return 1;
}

//��ȡ����
BYTE CGameLogic::GetAllCardValue(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE sum=0;
	for(BYTE i=0;i<cbCardCount;i++)
	{
		sum += GetCardLogicValue(cbCardData[i]);
	}
	ASSERT(sum>0);
	return (sum%10);
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount)
{
	//ת����ֵ
	BYTE cbLogicValue[MAX_COUNT];
	for (BYTE i=0;i<cbCardCount;i++) cbLogicValue[i]=GetCardValue(cbCardData[i]);	

	//�������
	bool bSorted=true;
	BYTE cbTempData,bLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((cbLogicValue[i]<cbLogicValue[i+1])||
				((cbLogicValue[i]==cbLogicValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbTempData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbTempData;
				cbTempData=cbLogicValue[i];
				cbLogicValue[i]=cbLogicValue[i+1];
				cbLogicValue[i+1]=cbTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//CopyMemory(cbCardBuffer,m_cbCardListData,cbBufferCount);
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(CountArray(m_cbCardListData)-bRandCount);
		cbCardBuffer[bRandCount++]=cbCardData[bPosition];
		cbCardData[bPosition]=cbCardData[CountArray(m_cbCardListData)-bRandCount];
	} while (bRandCount<cbBufferCount);

	return;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	//�˿�����
	BYTE bCardColor=GetCardColor(cbCardData);
	BYTE bCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	return (bCardValue>10)?(10):bCardValue;
}
//��ȡ��������
BYTE CGameLogic::GetFaceCardCount(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE cbCardNum = 0;
	for(int i = 0; i < cbCardCount; i++)
	{
		if(GetCardValue(cbCardData[i]) > 10) cbCardNum++;	
	}
	return cbCardNum;
}

//�Ա��˿�,Ϊtrue���м�Ӯ��Ϊfalse��ׯ��Ӯ,FirstΪ�мң�NextΪׯ��
bool CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	//��ȡ����
	BYTE cbNextType=GetCardType(cbNextData,cbCardCount);
	BYTE cbFirstType=GetCardType(cbFirstData,cbCardCount);

	if (cbNextType != cbFirstType)
	{
		return (cbFirstType > cbNextType)?true:false;
	}
	else
	{
		//�����С
		BYTE bFirstTemp[MAX_COUNT],bNextTemp[MAX_COUNT];
		CopyMemory(bFirstTemp,cbFirstData,cbCardCount);
		CopyMemory(bNextTemp,cbNextData,cbCardCount);
		SortCardList(bFirstTemp,cbCardCount);
		SortCardList(bNextTemp,cbCardCount);

		//�ȽϹ�������
		BYTE cbFirstFaceCardNum = GetFaceCardCount(bFirstTemp,cbCardCount);
		BYTE cbNextFaceCardNum  = GetFaceCardCount(bNextTemp,cbCardCount);

		//�Ƚ���ֵ
		BYTE cbNextMaxValue=GetCardValue(bNextTemp[0]);
		BYTE cbFirstMaxValue=GetCardValue(bFirstTemp[0]);

		//���ǻ�����
		if(cbNextType==OX_MIX_THREE_KING && cbFirstType==OX_MIX_THREE_KING)
		{
			//�Ƚ�����ƴ�С
			if(cbFirstMaxValue > cbNextMaxValue) return true;
			else if(cbFirstMaxValue < cbNextMaxValue) return false;
			else if(cbFirstMaxValue == cbNextMaxValue) return (GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0])) ? true : false;			
		}
		else
		{
			//�ȽϹ�����
			if(cbFirstFaceCardNum > cbNextFaceCardNum) return true;
			else if(cbFirstFaceCardNum < cbNextFaceCardNum) return false;
			else if(cbFirstFaceCardNum == cbNextFaceCardNum) 
			{
				//�Ƚ�����ƴ�С
				if(cbFirstMaxValue > cbNextMaxValue) return true;
				else if(cbFirstMaxValue < cbNextMaxValue) return false;
				else if(cbFirstMaxValue == cbNextMaxValue) return (GetCardColor(bFirstTemp[0]) > GetCardColor(bNextTemp[0])) ? true : false;
			}			
		}			
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////