#include "StdAfx.h"
#include "Cracker.h"

#include "sha1.h"

#include <string>
#include <assert.h>

using namespace std;


CCracker::CCracker(void)
{
	ZeroMemory(m_aGesture, sizeof(m_aGesture));
	m_nLength = UN_USED;
	m_strEncode = _T("");
	memset(&m_IsUse, false, sizeof(m_IsUse));
}


CCracker::~CCracker(void)
{
}

//递归函数，遍历以0-8为元素的所有可能排列
bool CCracker::Recursive(const int nMaxDeep, const int nCurrentDeep)
{
	assert(nMaxDeep>=0);
	assert(0<=nCurrentDeep&&nCurrentDeep<=nMaxDeep);

	static int aGestureNum[9];
	if(nCurrentDeep==0)
		memset(aGestureNum, UN_USED, sizeof(aGestureNum));
	
	for (int i=0;i<9-nCurrentDeep;i++)
	{
		aGestureNum[nCurrentDeep] = GetNewNumber(aGestureNum[nCurrentDeep]);

		assert(aGestureNum[nCurrentDeep]!=-1);
		
		//考虑到重用性，只验证指定深度
		if (nCurrentDeep == nMaxDeep)
		{
			if (Check(aGestureNum, nMaxDeep))
				return true;
			continue;
		}

		if (Recursive(nMaxDeep, nCurrentDeep+1))
			return true;
	}

	ReleaseNumber(aGestureNum[nCurrentDeep]);
	aGestureNum[nCurrentDeep] = UN_USED;

	return false;
}

bool CCracker::Crack(const string strEncode)
{
	m_strEncode = strEncode;

	//从0位到8位试个遍
	for (int i=MINIMUM_DEEP;i<=DEFAULT_DEEP;i++)
		if(Recursive(i))
			return true;
	
	return false;
}

int* CCracker::GetGesture(void)
{
	return m_aGesture;
}

int CCracker::GetGestureLength(void)
{
	return m_nLength;
}

void CCracker::ReleaseNumber(const int aNum)
{
	assert(m_IsUse[aNum]==true);
	
	m_IsUse[aNum] = false;

	return;
}

void CCracker::UseNumber(const int aNum)
{
	assert(m_IsUse[aNum]==false);

	m_IsUse[aNum] = true;

	return;
}

bool CCracker::IsUsed(const int aNum)
{
	return m_IsUse[aNum];
}

int CCracker::GetNewNumber(const int aNum)
{
	assert(0<=aNum&&aNum<=8||aNum==UN_USED);
	
	//释放数字
	if(aNum!=UN_USED)
		ReleaseNumber(aNum);
	
	//找出下一个未被使用的数字
	for (int i=aNum+1;i<=DEFAULT_DEEP;i++)
	{
		if(!IsUsed(i))
		{
			UseNumber(i);
			return i;
		}
	}
	return -1;
}

//检查是不是
bool CCracker::Check(const int aNum[], const int nDeep)
{
	char *pszGesture;
	//数组以空结束，方便存入string
	pszGesture = new char[nDeep+1+1];
	ZeroMemory(pszGesture, nDeep+1+1);

	for (int i=0;i<=nDeep;i++)
		pszGesture[i] = (char)(aNum[i]);

	CSHA1 cs;
	char szEncode[40+1];
	cs.SHA_GO(pszGesture, nDeep+1, szEncode);

	//均为小写形式
	if (m_strEncode==szEncode)
	{
		m_nLength = nDeep+1;
		memcpy(m_aGesture, aNum, m_nLength*sizeof(&aNum));
		

		delete pszGesture;

		return true;
	}

	delete pszGesture;

	return false;
}
