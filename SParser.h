// SParser.h: interface for the CSParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPARSER_H__10D8C76B_C1F8_4882_BC91_4970924C8ABE__INCLUDED_)
#define AFX_SPARSER_H__10D8C76B_C1F8_4882_BC91_4970924C8ABE__INCLUDED_

#if _MSC_VER > 10000
#pragma once
#endif // _MSC_VER > 10000

#define MAXKEYWORDS	10000

class CSParser
{
public:
	CSParser();
	virtual ~CSParser();

	virtual int Open()=0;
	virtual int Close()=0;

	virtual int GetStems(const char* str)=0;

	char* GetKey(int k) { return keys[k]; }
	int GetKeyNumber() { return nkeys; }
	void SetKeyNumber(int k) { nkeys=k; }
	void SetKey(int k, char* c) { keys[k]=c; }

protected:
	int nkeys;
	char *keys[MAXKEYWORDS];
};

#endif // !defined(AFX_SPARSER_H__10D8C76B_C1F8_4882_BC91_4970924C8ABE__INCLUDED_)
