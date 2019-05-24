/****************************************************************************************************************************
*  Parser���: ����һƪ�����г��ֵĹؼ����Լ����ؽ��ʵĸ���. �Թؼ���������Ҫ��:
*  1. ���ʿ�ͷ��ĸ�Ĵ�Сд��ͬһ�ؼ���.
*  2. �޶��ؼ���. ( ��ȥ�򵥴�: ȥ���, ȥ���, ȥһЩ���ô�, �� I, you, he, she, a,...
*  3. ��ͬʱ̬��ͬһ�ʸ��ĵ�������ͬһ�ؼ���. ( �Զ���ȥʱ̬���� )
*  4. ���������ֵ���ͬ��������ͬһ�ؼ���. ( ������ȥ���������� )
*  5. ����Ž������ԭ���ʸ�Ϊ׼����ͬһ�ؼ���.
*  �������: ֧�ִ�Դ�ļ� Input.txt ��������, ����ɴ�DOS��Ŀ���ļ� Output.txt ���.
*****************************************************************************************************************************/
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include "EMAParser.h"
#include "EMA.h"
#include "SParser.h"
#include "KMEANS.h"
#define _MAXLEN 15

using namespace std;

const int FileNum = 40;
static int Maxkeyfrequence = 1;//�ĳ���1�����ٻ���1��Ƶ��Ϊ1�Ĺؼ��ʡ�

typedef struct 
{
	char  key[_MAXLEN]; //Limite the length (< 10) of key word 
	int iKeyCnt;  //The times the key occurs

}ELEM;

typedef struct
{
	ELEM ele[1000];
	int elems;
	float tf[1000];
	float idf[1000];
	float tf_idf[1000];
}KEYS;

static KEYS tfidf[FileNum];

int IsKeyExist(ELEM*, int , char*);  //Prototype
void DealKeyWord(ELEM*, int&, char*);//Prototype
void SortbyKey(ELEM*, int);//Prototype
void tf_idf(KEYS tfidf[FileNum]);

int main()
{
	CEMAParser parser;
	int f = 0;
	char * pArticle = new char[3000];
    ofstream fout("Output.txt");
	//ifstream fin("Input.txt")���ƶ��������档
	if(!fout)
	    printf( "Can't open file Output.txt.\n" );
    else
	{
	  cout << "Open file Output.txt successfully." << endl;
	  fout << "Open file Output.txt successfully." << endl;
	} 
	//�˴��޸�,��ѭ���ķ�ʽ���ζ���FileNum���ļ������������ȥ�ͱ����˳���
	for(int n=0;n<FileNum;n++)
	{
		char filename[10];
		memset(filename,0,sizeof(filename));
		sprintf(filename,"%d.txt",n);
		ifstream fin(filename);
		
		if(!fin)
		{
			cout << "Can't open file " << n << ".txt." << endl;
			fout << "Can't optn file " << n << ".txt." << endl;
		}
		else
		{
			cout << "Open file " << n << ".txt successfully.\n" << endl;
			fout << "Open file " << n << ".txt successfully.\n" << endl;
		}
			char ch;
	
		for(int i = 0;  i < 3000 && (fin.get(ch) != 0); i++)
		{
			cout << ch;
			fout << ch;
			if(ch != '\n')
				pArticle[i] = ch;
			else
				pArticle[i] = ' ';
		}
		pArticle[i] = '\0';   // Imitate the pArticle as a string
		fin.close();
		cout << endl;
		fout << endl;
		parser.Open(); // pen Parser
		parser.GetStems(pArticle);// Analyse the Parser
		int kn = parser.GetKeyNumber(); // Get the total number of key words
		cout << "\nThere are " << kn << " key words in the article " << n << " without caring about redundance.\n";
		fout << "\n\nThere are " << kn << " key words in the article " << n << " without caring about redundance.\n";
		char * pKey;
		ELEM pElem[1000];  // Limite the key words less than 500
		int iElemCnt = 0;
		for( int j = 0; j < kn; j++ )
		{
			pKey = parser.GetKey(j); //Get a key word
			DealKeyWord(pElem, iElemCnt, pKey);
		}
		SortbyKey( pElem, iElemCnt  );
   		cout << "There are " << iElemCnt << " key words in the article " << n << "  after geting rid of redundance.\n";
		fout << "There are " << iElemCnt << " key words in the article " << n << "  after geting rid of redundance.\n";
		for( int m = 0; m < iElemCnt; m++ )
		{
			strcpy(tfidf[n].ele[m].key , pElem[ m ].key);
			tfidf[n].tf[m] = (float)pElem[m].iKeyCnt / Maxkeyfrequence;
			cout << pElem[ m ].key << " occurrence: " << pElem[ m ].iKeyCnt << endl;
			fout << pElem[ m ].key << " occurrence: " << pElem[ m ].iKeyCnt << endl;
		}

		cout<<"-------------------------------------------"<<endl;
		fout<<"-------------------------------------------"<<endl;

		cout << "Max key frequence is " << Maxkeyfrequence << endl;
		fout << "Max key frequence is " << Maxkeyfrequence << endl;

		cout<<"!!!CONGRATULATIONS!!!"<<endl;
		fout<<"!!!CONGRATULATIONS!!!"<<endl;

		cout<<"Atricle "<<n<<" has already been processed."<<endl;
		fout<<"Atricle "<<n<<" has already been processed."<<endl;

		cout<<"Program Continue..."<<endl;
		fout<<"Program Continue..."<<endl;

		cout<<"-------------------------------------------"<<endl;
		fout<<"-------------------------------------------"<<endl;

		Maxkeyfrequence = 1; //һƪ�������� �������Ƶ��
		tfidf[n].elems = iElemCnt;
	}
	tf_idf(tfidf);
   	fout.close();
    return 0;
}

int IsKeyExist(ELEM* pElem, int iElemCnt, char* pKey)
{
	for(int i = 0; i < iElemCnt; i++)  //iElemCnt�ǵ�ǰ�ṹ���ŵĸ���
	{
		if(strcmp( pElem[i].key, pKey) == 0)
			return i;
	}
	return -1;
}

void DealKeyWord( ELEM* pElem, int& iElemCnt, char* pKey )
{
	int iIndex = IsKeyExist( pElem, iElemCnt, pKey );
	if (iIndex > -1) // if it exists
	{
		pElem[iIndex].iKeyCnt++;
		if (pElem[iIndex].iKeyCnt > Maxkeyfrequence)
			Maxkeyfrequence = pElem[iIndex].iKeyCnt;
	}
	else    
	{
	/*	
		int length = strlen(pKey); 
		length = (length < 10 ? length: 10);
		strncpy(pElem[iElemCnt].key, pKey, length);	
		pElem[iElemCnt].key[length] = '\0';
	*/
		memcpy(pElem[iElemCnt].key, pKey, _MAXLEN); //Memory copy
		pElem[iElemCnt].iKeyCnt = 1;
		iElemCnt++;
	}
}

void SortbyKey( ELEM *pElem, int iElemCnt ) //Sort by key word
{
	for( int i = 1; i <= iElemCnt; i++ )
		for( int j = 0; j < iElemCnt - i - 1; j++ )
		{
			if(strcmp(pElem[ j ].key, pElem[ j + 1 ].key) > 0)//����ĸ��С����
			{
				ELEM temp = pElem[j + 1];
			    pElem[j + 1] = pElem[j];
				pElem[j] = temp;
			}
		}
}

void tf_idf(KEYS tfidf[FileNum])
{
	ofstream fout("Output.txt");
	cout.fill(' ');
	for (int x = 0; x < FileNum; x++)
		{
			int df = 0;
			for (int y = 0; y < tfidf[x].elems; y++)
			{
				char * idfkey_now = new char[_MAXLEN];
				char * idfkey_other = new char[_MAXLEN];
				idfkey_now = tfidf[x].ele[y].key;
				for (int x1 = 0; x1 < FileNum; x1++) 
				{
					if (x1 == x) continue;
					for (int y1 = 0; y1 < tfidf[x].elems; y1++)
					{
						idfkey_other = tfidf[x1].ele[y1].key;
						if (strcmp(idfkey_now, idfkey_other) == 0)
						{
							df++;
							break;
						}
					}
				}
				tfidf[x].idf[y] = log((float)FileNum / df+1)/log(2);
				tfidf[x].tf_idf[y] = tfidf[x].tf[y] * tfidf[x].idf[y];
				cout <<setiosflags(ios::fixed)<<"In file "<< x <<setw(16)<< tfidf[x].ele[y].key <<"'s tf= "<< tfidf[x].tf[y] <<setw(8)<<" idf= "<< tfidf[x].idf[y] <<setw(8)<<" tf-idf= "<< tfidf[x].tf_idf[y] <<endl;
				fout <<setiosflags(ios::fixed)<<"In file "<< x <<setw(16)<< tfidf[x].ele[y].key <<"'s tf= "<< tfidf[x].tf[y] <<setw(8)<<" idf= "<< tfidf[x].idf[y] <<setw(8)<<" tf-idf= "<< tfidf[x].tf_idf[y] <<endl;
			}
			cout<<"-------------------------------------------"<<endl;
			fout<<"-------------------------------------------"<<endl;

			cout<<"!!!CONGRATULATIONS!!!"<<endl;
			fout<<"!!!CONGRATULATIONS!!!"<<endl;

			cout<<"Atricle "<<x<<" has already been processed."<<endl;
			fout<<"Atricle "<<x<<" has already been processed."<<endl;

			cout<<"Program Continue..."<<endl;
			fout<<"Program Continue..."<<endl;

			cout<<"-------------------------------------------"<<endl;
			fout<<"-------------------------------------------"<<endl;
			
		}
}
	