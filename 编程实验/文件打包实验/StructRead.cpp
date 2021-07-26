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
	FILE* infile;
	Person person;
	
	if ((infile=fopen("Persons.stru","rb"))==NULL)
	{
		cout<<"Can't open the file!\n";
		getchar();
		exit(1);
	}
	while (fread(&person, sizeof(struct Person),1,infile)==1){
		cout<<"�û���: "<<person.username<<"\n";
		cout<<"����: "<<person.level<<"\n";
		cout<<"�����ʼ�: "<<person.email<<"\n";
		time_t sendtime = person.sendtime;
		cout<<"����ʱ��: "<<ctime(&sendtime)<<"\n";
		cout<<"ע��ʱ��: "<<ctime(&person.regtime)<<"\n";
	}
	fclose(infile);
}
