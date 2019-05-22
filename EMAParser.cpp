// EMAParser.cpp: implementation of the CEMAParser class.
//
//////////////////////////////////////////////////////////////////////

#include "EMAParser.h"
#include <ctype.h>
#include <string.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEMAParser::CEMAParser()
{
}

CEMAParser::~CEMAParser()
{
}

int CEMAParser::GetStems(const char* str)
{
	int len=strlen(str);
	char* s=new char[len+1];
	memcpy(s, str, len+1);

	stemming(s);

	delete[] s;
	return GetKeyNumber();
}

char *CEMAParser::my_strtok_with_hangulset(char *input, char **backpointer, char *last_char, int *ishangul)
{
	char *ptr;
	int  flag=0;
	char *start;
	int i=0;

	if(input==0 && *last_char!=0){
		(*backpointer)--;
		**backpointer=*last_char;
	}	
	*last_char=0;

	*ishangul=0;
	if(input==0){
		if(*backpointer==0){
			return 0;
		}
		start=*backpointer;
	}
	else{
		start=input;
	}

	for(; *start!='\0'; start++){
		flag=0;
		if(isspace(*start) || iscntrl(*start)){
			flag=1;
		}
		else if((*start& 0x80)==0x80 && ((*(start+1) & 0x80) !=0x80)){
			flag=1;
		}
		else if(*start==';' || *start==':'){
			flag=1;
		}
		else if(((unsigned char)(*start) & 0x80)==0x80){
			*ishangul=1;
			break;
		}
	

		if(*start=='&'){

			if(*(start+3)==';'){  //lt, gt
					flag=1;
					start+=2;
			}
			else if(*(start+4)==';'){ //
					flag=1;
					start+=3;
			}
			else if(*(start+5)==';'){ //quot
					flag=1;
					start+=4;
			}
			else if(*(start+6)==';'){
					flag=1;
					start+=5;
			}
			else if(*(start+7)==';'){ // middot
					flag=1;
					start+=6;
			}
		}

		if(flag==0){
			break;
		}
	}
	if(*start=='\0'){
		*backpointer=0;
		return 0;
	}

	*last_char=0;
	for(i=0, ptr=start; *ptr!='\0' && i < MAX_MORPHEME_SIZE; i++, ptr++){
		if(isspace(*ptr) || iscntrl(*ptr) || *ptr==';' || *ptr==':'){
			*ptr='\0';
			*backpointer=ptr+1;
			return start;
		}
		else if(((*ptr & 0x80)==0x80)&& ((*(ptr+1) & 0x80)!=0x80)){
				*ptr='\0';
				*backpointer=ptr+1;
				return start;
		}
		else if(((unsigned char)(*ptr)&0x80)==0x80){
			ptr++;
			i++;
			*ishangul=1;
			continue;
		}

		if(*ptr=='&'){

			if(*(ptr+3)==';'){
					*ptr='\0';
					*backpointer=ptr+3;
					return start;
			}
			else if(*(ptr+4)==';'){
					*ptr='\0';
					*backpointer=ptr+4;
					return start;
			}
			else if(*(ptr+5)==';'){
					*ptr='\0';
					*backpointer=ptr+5;
					return start;
			}
			else if(*(ptr+6)==';'){
					*ptr='\0';
					*backpointer=ptr+6;
					return start;
			}
			else if(*(ptr+7)==';'){
					*ptr='\0';
					*backpointer=ptr+7;
					return start;
			}
		}
	}
	if(*ptr!='\0'){
		*last_char=*ptr;
		*ptr='\0';
		*backpointer=ptr+1;
		return start;
	}
	*backpointer='\0';
	return start;
}

void CEMAParser::stemming(char *string)
{
	char *termPtr4MA, *term_bptr;

	int rn=0, keynum=0;
	char last_char;
	int is_hangul;
	//int i;

	SetKeyNumber(0);
	term_bptr=0;
	termPtr4MA=(char*)my_strtok_with_hangulset(string,  &term_bptr, 
				&last_char, &is_hangul);

	while(termPtr4MA!=0){
		if(is_hangul==0){
			rn=EMA_getIndexTerms(ema, termPtr4MA, &result[keynum]);
			keynum+=rn;
		}
		else{
			//printf("error\n"); exit(-1);
			return;
		}
		termPtr4MA=(char*)my_strtok_with_hangulset(0, &term_bptr,
								&last_char, &is_hangul);
	}
	for(int i=0; i < keynum; i++)
		SetKey(i, result[i]);
	SetKeyNumber(keynum);
}
