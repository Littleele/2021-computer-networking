#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<cstring>
#include<io.h>
using namespace std;

#define BUF_LEN 100
#define FNAME_LEN 300
char buf[BUF_LEN];

void getUniqueName(char *newFileName){
    int cnt=1;
    char temp[FNAME_LEN]={};
    
    while(access(newFileName,0)==0){ //���ļ��Ѵ���
        char str[10]={};
        int i=0,leftpos=0;
        for(i=0;i<strlen(newFileName);i++){
            if(newFileName[i]=='(')
            leftpos=i;
            if(newFileName[i]=='.')
            break;
        }
        if(leftpos!=0){
            strncpy(temp,newFileName+i,FNAME_LEN);
            newFileName[leftpos]='\0';
            strncat(newFileName,temp,FNAME_LEN);

        }
        cnt++;
       
        for(i=0;i<strlen(newFileName);i++){
            if(newFileName[i]=='.')
            break;
        }
        str[0]='(';
        itoa(cnt,str+1,10);
        str[strlen(str)]=')';
        memset(temp,'\0',sizeof(temp));
        strncpy(temp,newFileName,i);
        strcat(temp,str);
        strcat(temp,newFileName+i);
        strncpy(newFileName,temp,FNAME_LEN);    
    }
    
}

void getFullname(char *fname,char *desfname){
  char ch='\\';
  int namepos=strrchr(fname,ch)-fname;
  char buf[FNAME_LEN]={};
  strncpy(buf,desfname,FNAME_LEN);
  strcat(buf,fname+namepos);
  strncpy(fname,buf,FNAME_LEN);

}

int main(){
    char desfname[FNAME_LEN]={};
    cout<<"����Ŀ���ļ��У� ";
    fgets(desfname, FNAME_LEN,stdin);
    desfname[strlen(desfname)-1]='\0';

    // FILE *desfile=NULL;
    // if (fopen_s(&desfile,desfname, "wb")!=0){
    // printf("Ŀ���ļ���δ�ҵ���\n");
    // printf("�����������...");
    // getchar();
    // getchar();
    // exit(1);
    // }

    char srcname[FNAME_LEN]={};
    cout<<"����Ҫ������ļ��� ";
    fgets(srcname,FNAME_LEN,stdin);
    srcname[strlen(srcname)-1]='\0';

    FILE *srcfile=NULL;    
    if (fopen_s(&srcfile, srcname, "rb")!=0){
    printf("����ļ�δ�ҵ���\n");
    printf("�����������...");
    getchar();
    getchar();
    exit(1);
    }//��������

    while(1){
        char fname[FNAME_LEN]={};
        if(fread(fname,FNAME_LEN,1,srcfile)!=1)//�����ļ���
        break;
        getFullname(fname,desfname);
        getUniqueName(fname);//����������������� 

        FILE *fullfl=NULL;
        if (fopen_s(&fullfl,fname,"wb")!=0){
        printf("����ļ�δ�ҵ���\n");
        printf("�����������...");
        getchar();
        getchar();
        exit(1);
        }

        __int64 filesize;
        fread(&filesize, sizeof( __int64), 1, srcfile);//�����ļ���С

        int len=0;
        while(len<filesize){
            if(BUF_LEN<=filesize-len){
                fread(buf,1,BUF_LEN,srcfile);
                len+=BUF_LEN;
                fwrite(buf,1,BUF_LEN,fullfl);
            }
            else{
                fread(buf,1,filesize-len,srcfile);
                fwrite(buf,1,filesize-len,fullfl);
                len+=(filesize-len);
                
            }
    }

    fclose(fullfl);
}
    cout<<"��������������������...";

}
