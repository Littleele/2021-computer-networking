#include<iostream>
#include<cstdio>
#include<cstring>
#include<time.h>
using namespace std;
#define BUF_LEN 100
#define USER_NAME_LEN 20
#define EMAIL_LEN 80
#define TIME_BUF_LEN 30
typedef unsigned long DWORD;

struct Person
{
	char username[USER_NAME_LEN]; // Ա����
	int level;					  // ���ʼ���
	char email[EMAIL_LEN];		  // Email��ַ
	DWORD sendtime;               // ����ʱ�䣨��дÿ���û���Ϣ��ʱ�䣩
	time_t regtime;               // ע��ʱ�䣨������������ʱ�䣩
};

int main()
{
	FILE * pFile;
	if ((fopen_s(&pFile, "Persons.stru", "wb"))!=0) {
		cout<<"Can't open the file!\n";
		getchar();
		exit(1);
	}
    char exitt[5]="exit";
	
    while(1){
        Person temp;
		char inputBuf[BUF_LEN]={};
        cout<<"Username: ";
        fgets(inputBuf, BUF_LEN, stdin);
		inputBuf[strlen(inputBuf)-1] ='\0';
        if(strcmp(inputBuf,exitt)==0)
        exit(0);
		strcpy(temp.username, inputBuf);

        char	pts[TIME_BUF_LEN];		   // pointer to time string	       
	    time_t	now;			           //current time	
		(void)time(&now);                   // ȡ��ϵͳʱ��
		ctime_s(pts, TIME_BUF_LEN, &now);   // ��ʱ��ת��Ϊ�ַ���
        cout<<"Level: ";
        cin>>temp.level;
        cout<<"Email: ";
        scanf("%s", inputBuf);
		strcpy(temp.email, inputBuf);
        temp.sendtime = (DWORD)now;         // ???��??????
	    temp.regtime = now;
        cout<<"\n";
        if (fwrite(&temp, sizeof(Person), 1,pFile) != 1) {
			cout<<"File write error!\n";
		}
		getchar();
		
	}
	fclose(pFile);
	
}
