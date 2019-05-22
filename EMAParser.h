// EMAParser.h: interface for the CEMAParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMAPARSER_H__EA029292_1BA8_4721_A92B_7C4EDA88452C__INCLUDED_)
#define AFX_EMAPARSER_H__EA029292_1BA8_4721_A92B_7C4EDA88452C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EMA.h"
#include "SParser.h"

class CEMAParser : public CSParser
{
public:
	CEMAParser();
	virtual ~CEMAParser();
	int Open() { ema=EMA_create(); return 1; }
	int Close() { EMA_destroy(ema); return 1; }
	int GetStems(const char* str);

protected:
	char *my_strtok_with_hangulset(char *input, char **backpointer, char *last_char, int *ishangul);
	void stemming(char *string);
	EMA *ema;
	char result[MAXKEYWORDS][MAX_MORPHEME_SIZE+1];
};

#endif // !defined(AFX_EMAPARSER_H__EA029292_1BA8_4721_A92B_7C4EDA88452C__INCLUDED_)
