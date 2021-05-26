#pragma once

#include <string>

using namespace std;

#define DEFAULT_DEEP 8
#define MINIMUM_DEEP 3

#define UN_USED -1

class CCracker
{
public:
	CCracker(void);
	~CCracker(void);
	bool Crack(const string strEncode);
	int* GetGesture(void);
	int GetGestureLength(void);
private:
	int GetNewNumber(const int aNum);
	void ReleaseNumber(const int aNum);
	void UseNumber(const int aNum);
	bool IsUsed(const int aNum);

	bool Recursive(const int nDeep = DEFAULT_DEEP, const int nCurrentDeep = 0);
	bool Check(const int aNum[], const int nDeep);
private:
	string m_strEncode;
	int m_aGesture[9];
	int m_nLength;

	//标记某个数字是否被使用
	bool m_IsUse[9];
};

