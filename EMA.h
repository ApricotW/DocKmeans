#ifndef __EMA_HEADER_FILE__
#define __EMA_HEADER_FILE__

#define MAX_MORPHEME_SIZE 50
//#define MAX_INDEX_RESULT  30

#define EMA_MAX_LETTER	27
#define EMA_MAX_CHAR	30
#define EMA_MAX_TERMS	1000
#define EMA_MAX_NAME	50


#define EMA_FAIL	-1
#define EMA_SUCCESS	0

#define EMA_EMPTY	0
#define EMA_DATA	1
#define EMA_POINTER	2

#define EMA_UC(ch) (ch <= 'Z' && ch >= 'A')
#define EMA_LC(ch) (ch <= 'z' && ch >= 'a')
#define EMA_LETTER(ch) (EMA_UC(ch) || EMA_LC(ch))
#define EMA_FORCELC(ch) (ch-('A'-'a'))

#define TRUE 1
#define FALSE 0

typedef struct{
	int handle;	//it's just for convention
}EMA;

typedef unsigned char UINT1;

//checks whether a given word is a stopword or not

extern "C" int EMA_isStopword(EMA *, char *, int length);

extern "C" int EMA_getIndexTerms(EMA *, char *, char (*result)[MAX_MORPHEME_SIZE+1]/*,
		char **output*/);

//stemming a given word
//limit is the max size of result
//we assume the size of the longest term is 50.
//limit = MAX_MA_SIZE
extern "C" char * EMA_extractTerms(EMA *, char *str, char *nnn, unsigned long limit);
extern "C" int EMA_is_an_englishTerm(EMA *, char *);

//initialize the English Stemmer
extern "C" EMA* EMA_create();
extern "C" UINT1 EMA_close(EMA *);
extern "C" UINT1 EMA_init(EMA *);
extern "C" UINT1 EMA_destroy(EMA *);
extern "C" UINT1 EMA_final(EMA *);

#endif
