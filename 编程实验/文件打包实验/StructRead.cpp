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
	FILE* infile;
	Person person;
	
	if ((infile=fopen("Persons.stru","rb"))==NULL)
	{
		cout<<"Can't open the file!\n";
		getchar();
		exit(1);
	}
	while (fread(&person, sizeof(struct Person),1,infile)==1){
		cout<<"用户名: "<<person.username<<"\n";
		cout<<"级别: "<<person.level<<"\n";
		cout<<"电子邮件: "<<person.email<<"\n";
		time_t sendtime = person.sendtime;
		cout<<"发送时间: "<<ctime(&sendtime)<<"\n";
		cout<<"注册时间: "<<ctime(&person.regtime)<<"\n";
	}
	fclose(infile);
}
