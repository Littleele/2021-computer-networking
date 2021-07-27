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
    cout<<"输入目标文件名（含路径）： ";
    fgets(destfname, FNAME_LEN,stdin);
    destfname[strlen(destfname)-1]='\0';

    FILE *destfile=NULL;
    if (fopen_s(&destfile,destfname, "wb")!=0)	{
	printf("目标文件夹未找到！\n");
	printf("按任意键继续...");
	getchar();
	getchar();
	exit(1);
	}

    while(1){
        char srcfname[FNAME_LEN]={};
	    cout<<"输入要打包的#"<<count<<"文件名（含路径）： ";
        count++;
        fgets(srcfname,FNAME_LEN,stdin);
        srcfname[strlen(srcfname)-1]='\0';
        if(strcmp(srcfname,exitt)==0)
        break;//若输入为exit 退出程序

        FILE *srcfile=NULL;	   
        if (fopen_s(&srcfile, srcfname, "rb")!=0){
		printf("读入文件未找到！\n");
		printf("按任意键继续...");
		getchar();
		getchar();
		exit(1);
	    }

        fwrite(srcfname,FNAME_LEN,1,destfile);//写入文件名

        fseek(srcfile,0,SEEK_END);//定位到文件的最后面
        __int64 filesize=ftell(srcfile);//获取文件的大小
        fseek(srcfile, 0, SEEK_SET);//将指针定义为文件的开头
        fwrite(&filesize, sizeof(__int64), 1, destfile);//写入文件大小

        int len = 0;
	// fread的参数：缓冲区起始地址，每个数据(char类型)的字节数，最多读出的数据个数，文件句柄
    //      返回值：实际读出的数据个数
	while ((len = fread(buf, 1, BUF_LEN, srcfile)) >= BUF_LEN) { 
		//fwrite的参数：数据起始地址，每个数据(char类型)的字节数，写入数据的个数，文件句柄
        //      返回值：实际写入的数据个数
		fwrite(buf, 1, BUF_LEN, destfile);
	}
	fwrite(buf, 1, len, destfile);   // 写入剩余部分
    fclose(srcfile);
    }
    fclose(destfile);
    cout<<"打包结束！";
}