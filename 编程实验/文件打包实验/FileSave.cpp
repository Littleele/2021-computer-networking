#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<cstring>
using namespace std;
#define BUF_LEN 100
#define FNAME_LEN 300
char buf[BUF_LEN];

int main(){
    char exitt[5]="exit";
    int count=1;
    char destfname[FNAME_LEN]={};
    cout<<"����Ŀ���ļ�������·������ ";
    fgets(destfname, FNAME_LEN,stdin);
    destfname[strlen(destfname)-1]='\0';

    FILE *destfile=NULL;
    if (fopen_s(&destfile,destfname, "wb")!=0)	{
	printf("Ŀ���ļ���δ�ҵ���\n");
	printf("�����������...");
	getchar();
	getchar();
	exit(1);
	}

    while(1){
        char srcfname[FNAME_LEN]={};
	    cout<<"����Ҫ�����#"<<count<<"�ļ�������·������ ";
        count++;
        fgets(srcfname,FNAME_LEN,stdin);
        srcfname[strlen(srcfname)-1]='\0';
        if(strcmp(srcfname,exitt)==0)
        break;//������Ϊexit �˳�����

        FILE *srcfile=NULL;	   
        if (fopen_s(&srcfile, srcfname, "rb")!=0){
		printf("�����ļ�δ�ҵ���\n");
		printf("�����������...");
		getchar();
		getchar();
		exit(1);
	    }

        fwrite(srcfname,FNAME_LEN,1,destfile);//д���ļ���

        fseek(srcfile,0,SEEK_END);//��λ���ļ��������
        __int64 filesize=ftell(srcfile);//��ȡ�ļ��Ĵ�С
        fseek(srcfile, 0, SEEK_SET);//��ָ�붨��Ϊ�ļ��Ŀ�ͷ
        fwrite(&filesize, sizeof(__int64), 1, destfile);//д���ļ���С

        int len = 0;
	// fread�Ĳ�������������ʼ��ַ��ÿ������(char����)���ֽ����������������ݸ������ļ����
    //      ����ֵ��ʵ�ʶ��������ݸ���
	while ((len = fread(buf, 1, BUF_LEN, srcfile)) >= BUF_LEN) { 
		//fwrite�Ĳ�����������ʼ��ַ��ÿ������(char����)���ֽ�����д�����ݵĸ������ļ����
        //      ����ֵ��ʵ��д������ݸ���
		fwrite(buf, 1, BUF_LEN, destfile);
	}
	fwrite(buf, 1, len, destfile);   // д��ʣ�ಿ��
    fclose(srcfile);
    }
    fclose(destfile);
    cout<<"���������";
}