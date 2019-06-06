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
#include <vector>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <time.h>
#include "EMAParser.h"
#include "EMA.h"
#include "SParser.h"

#define _MAXLEN 15


using namespace std;

const int FileNum = 10;
const int KS = 4;		//�ִ�����
//const int KMEANS_KEY = 2; //ÿƪ����ȡ�����ٹؼ��ֽ��з���

static int Maxkeyfrequence = 1;//�ĳ���1�����ٻ���1��Ƶ��Ϊ1�Ĺؼ��ʡ�
static int cycle=0;

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

KEYS tfidf[FileNum];


static vector<char *> key_words; //���ַ�������char[_MAXLEN]���޷�push_back
static vector<float> weight;
static vector<vector<float> > matrix(FileNum);

int IsKeyExist(ELEM*, int , char*);  //Prototype
void DealKeyWord(ELEM*, int&, char*);//Prototype
void SortbyKey(ELEM*, int);//Prototype
void tf_idf_wrk(KEYS tfidf[FileNum]);
void kmeans_analyze(KEYS tfidf[FileNum]);

static float cosine[KS][FileNum]={0};
static int cluster[FileNum];
static int cluster_k_cnt[KS]={0};
static int pre_cluster_k_cnt[KS];
static int kstep[KS];
static int arraycount=0;
static float sumup=0;
static float sumdown1=0;
static float sumdown2=0;

void k_loop();

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
			char ch='0';
	
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
		fout<<"------------------------------------rk-w---"<<endl;

		cout << "Max key frequence is " << Maxkeyfrequence << endl;
		fout << "Max key frequence is " << Maxkeyfrequence << endl;

		cout<<"!!!CONGRATULATIONS!!!"<<endl;
		fout<<"!!!CONGRATULATIONS!!!"<<endl;

		cout<<"Atricle "<<n<<" has already been processed."<<endl;
		fout<<"Atricle "<<n<<" has already been processed."<<endl;

		cout<<"Program Continue..."<<endl;
		fout<<"Program Continue..."<<endl;

		cout<<"-------------------------------------------"<<endl;
		fout<<"-----2019-5-25---------edit-at-3:37--------"<<endl;

		Maxkeyfrequence = 1; //һƪ�������� �������Ƶ��
		tfidf[n].elems = iElemCnt;
	}
	tf_idf_wrk(tfidf);
	kmeans_analyze(tfidf);
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
		i am 1033180218 wrk;
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

void tf_idf_wrk(KEYS tfidf[FileNum])
{
	int df = 0;
	char * idfkey_now = new char[_MAXLEN];
	char * idfkey_other = new char[_MAXLEN];

	ofstream fout("Output.txt",ios::out|ios::app);
	cout.fill(' ');
	for (int x = 0; x < FileNum; x++)
		{
			for (int y = 0; y < tfidf[x].elems; y++)
			{
				df = 0;
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
				tfidf[x].idf[y] = log( (float)FileNum / (1+df) )   /   log(2);
				tfidf[x].tf_idf[y] = tfidf[x].tf[y] * tfidf[x].idf[y];
				cout <<setiosflags(ios::fixed)<<"In file "<< x <<setw(16)<< tfidf[x].ele[y].key <<"'s tf= "<<setw(10)<< tfidf[x].tf[y] <<" idf= "<<setw(10)<< tfidf[x].idf[y] <<" tf-idf= "<<setw(10)<< tfidf[x].tf_idf[y] <<endl;
				fout <<setiosflags(ios::fixed)<<"In file "<< x <<setw(16)<< tfidf[x].ele[y].key <<"'s tf= "<<setw(10)<< tfidf[x].tf[y] <<" idf= "<<setw(10)<< tfidf[x].idf[y] <<" tf-idf= "<<setw(10)<< tfidf[x].tf_idf[y] <<endl;
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
			fout<<"-------------------------------------Rk-W--"<<endl;
			
		}
}

void kmeans_analyze(KEYS tfidf[FileNum])
{

	for (int x = 0; x < FileNum; x++)
	{
		for (int y = 0; y < tfidf[x].elems; y++)
		{
			if( key_words.empty() )
				key_words.push_back(tempkey);
			else
				for(int i=0;i<key_words.size();i++)
				{
					if(strcmp(key_words[i],tfidf[x].ele[y].key)==0)
						break;
					if(i+1==key_words.size())
						key_words.push_back(tempkey);
				
				}
		}
		
	}
	weight.resize(key_words.size());

	for (int m = 0; m < FileNum; m++)
	{
		for (int n = 0; n < key_words.size(); n++)
			for(int key = 0; key < KMEANS_KEY; key++)
			{
				if( strcmp(important[m * KMEANS_KEY + key].impkey,key_words[n])==0 )
				{
					weight[n] = important[m * KMEANS_KEY + key].imptfidf;
					break;
				}else if(key == 2)
					weight[n] = 0;
				
			}
		matrix[m] = weight;
	}

	cout<<"Please enter "<<KS<<" different centers, in range of 0-"<<FileNum<<"."<<endl;
	for(int k=0; k<KS; k++)
	{
		cin>>kstep[k];
		cout<<"good , input next!"<<endl;
	}


	
	for (m = 0; m < FileNum; m++)
	{
		for(k=0; k< KS; k++)
		{
			for (int n = 0; n < key_words.size(); n++)
			{
				 sumup += matrix[m][n] * matrix[kstep[k]][n];
				 sumdown1 += matrix[m][n] * matrix[m][n];
				 sumdown2 += matrix[kstep[k]][n] * matrix[kstep[k]][n];
			}
			cosine[k][m] = sumup / ( sqrt(sumdown1)*sqrt(sumdown2) );
			sumup=0;
			sumdown1=0;
			sumdown2=0;
		
		}
	
	}

	k_loop();
			

	cout<<"==================================="<<endl;
	cout<<"KMEANS FINISHED ARFTER "<<cycle<<" CYCLES!!!"<<endl;

	for(k=0; k< KS; k++)
	{
		cout<<"Cluster "<<k<<" has "<<cluster_k_cnt[k]<<" articles:"<<endl;
	}
}

void k_loop()
{
			/**
			 *�˴����һ�����ܣ�
			 *������m���ĸ�k��cosֵ���
			 *��m�滮���k�أ���������m
			 *��������k�ص���������
			 *���������������֮����Դ���������
			 *������ĸ��µ����ģ����±���m��cosֵ��
			 *ֱ���������Ļ������ٱ仯Ϊֹ
			 **/
	float newcenter[KS][FileNum*KMEANS_KEY];
	for (int m = 0; m < FileNum; m++)
	{
		for(int k=0; k< KS-1; k++)
		{
			if(cosine[k+1][m] > cosine[k][m])
				cluster[m]=k+1;
			else
				cluster[m]=k;
		}
	}


	for(int k=0; k< KS; k++)
	{
		pre_cluster_k_cnt[k]=cluster_k_cnt[k];
		for (m = 0; m < FileNum; m++)
		{
			if(cluster[m]==k)
			{
				arraycount++;
				for (int n = 0; n < key_words.size(); n++)
					newcenter[k][n]+=matrix[m][n];

			}
		}
		for (int n = 0; n < key_words.size(); n++)
				newcenter[k][n]/=arraycount;
			
		cluster_k_cnt[k]=arraycount;
		arraycount=0;
	}

	for (m = 0; m < FileNum; m++)
	{
		for(k=0; k< KS; k++)
		{
			for (int n = 0; n < key_words.size(); n++)
			{
				 sumup += matrix[m][n] * newcenter[k][n];
				 sumdown1 += matrix[m][n] * matrix[m][n];
				 sumdown2 += newcenter[k][n] * newcenter[k][n];
			}
			cosine[k][m] = sumup / ( sqrt(sumdown1)*sqrt(sumdown2) );
			sumup=0;
			sumdown1=0;
			sumdown2=0;
		
		}
	
	}

	for(k=0; k< KS; k++)
	{
		cout<<cluster_k_cnt[k]<<endl;
		if(cluster_k_cnt[k]!=pre_cluster_k_cnt[k])
			break;
		if(cluster_k_cnt[k]==pre_cluster_k_cnt[k])
			continue;
		if(k==KS-1)
			return;

	}

	cycle++;

	cout<<"KMEANS_CYCLE "<<cycle<<" FINISHED!!"<<endl;

	k_loop();
}