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
	char username[USER_NAME_LEN]; // 员工名
	int level;					  // 工资级别
	char email[EMAIL_LEN];		  // Email地址
	DWORD sendtime;               // 发送时间（填写每个用户信息的时间）
	time_t regtime;               // 注册时间（本程序启动的时间）
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
		(void)time(&now);                   // 取得系统时间
		ctime_s(pts, TIME_BUF_LEN, &now);   // 把时间转换为字符串
        cout<<"Level: ";
        cin>>temp.level;
        cout<<"Email: ";
        scanf("%s", inputBuf);
		strcpy(temp.email, inputBuf);
        temp.sendtime = (DWORD)now;         // ???÷??????
	    temp.regtime = now;
        cout<<"\n";
        if (fwrite(&temp, sizeof(Person), 1,pFile) != 1) {
			cout<<"File write error!\n";
		}
		getchar();
		
	}
	fclose(pFile);
	
}
