#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <conio.h>
#include <string>
#include <io.h>
#include<iostream>
#include<process.h>
#pragma comment(lib, "ws2_32.lib") // 使用winsock 2.2 library
#define BUFLEN 2000				   // 缓冲区大小
#define fname 300
using namespace std;
char buf[fname];
char des[fname+1];         //收到数据放到的文件路径
char sendfilename[fname+1];		//从本机发送的文件名
char recvfilename[fname+1];
char flags[2]; 

__int64 getFileSize(char * fileName);
void validateFilename(const char* folder, char* filename);
void sendChatPacket(SOCKET sock,char *chatData);
void sendFilePacket(SOCKET sock, char * fullFileName);
void getFileName(char *pathName,char* filename);
int recvData(SOCKET sock, char *buf);
int recvFile(SOCKET sock, char * fileName,char *path);
int executeCmd(char * cmdLine,SOCKET sock);
unsigned __stdcall recvthread(void *p);
void getUniqueName(char *newFileName);
void getFullname(char *name,char *desfname); 


int main()
{
	
    const char* host = "127.0.0.1";	    /* server IP to connect         */
	const char* service = "50500";  	    /* server port to connect       */
	struct sockaddr_in sin;	            /* an Internet endpoint address	*/
	char	buf[BUFLEN+1];   		        /* buffer for one line of text	*/
	SOCKET	sock;		  	

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 0), &wsadata); 
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	 
    memset(&sin, 0, sizeof(sin));						  
    sin.sin_family = AF_INET;							 
    sin.sin_addr.s_addr = inet_addr(host);                
    sin.sin_port = htons((u_short)atoi(service));         
    int ret=connect(sock, (struct sockaddr *)&sin, sizeof(sin));  
    cout<<"正在连接..."<<endl;
    if (ret == -1)
	{
		cout<<"连接到服务器失败！"<<endl;
		closesocket(sock);
		WSACleanup();
          system("pause");
		exit(1);
	}
     else if(ret==0)
     {
          cout<<"连接成功！"<<endl;

     }
    HANDLE recvhandle=(HANDLE)_beginthreadex(NULL,0,&recvthread,(void*)&sock,0,NULL);
	
    while (1)
	{	
        cout<<">";
        fgets(buf,BUFLEN,stdin);
        buf[strlen(buf)-1]='\0';
        if(!executeCmd(buf,sock))
        break;
		
        
	
	}
    system("pause");
	CloseHandle(recvhandle);
    closesocket(sock);                             // 关闭监听套接字
    WSACleanup();            
	exit(0);
}



__int64 getFileSize(char * fileName){
    FILE *srcfile=NULL;	
    if (fopen_s(&srcfile, fileName, "rb")!=0){
		printf("读入文件未找到！\n");
		printf("按任意键继续...");
		getchar();
		getchar();
		exit(1);
	}
    fseek(srcfile,0,SEEK_END);//定位到文件的最后面
    __int64 filesize=ftell(srcfile);//获取文件的大小
    fseek(srcfile, 0, SEEK_SET);//将指针定义为文件的开头
    return filesize;
}

unsigned __stdcall recvthread(void *p){
     char	buf[BUFLEN+1]; 
	 char	msg[BUFLEN+1];
     SOCKET ssock=*(SOCKET*)p;
     while(1){  
        
        int cc = recv(ssock, buf, 1, 0); 

        if(cc == SOCKET_ERROR)
        printf("Error: %d.\n",GetLastError());     //出错。其后必须关闭套接字sock。
          else if(cc > 0) {
          	if(buf[0]=='1')
			  recvData(ssock,msg);
          
		  else if(buf[0]=='2')
			  recvFile(ssock,recvfilename,des);
		  
        }
          

     }
     
    return 0;

}

int executeCmd(char * cmdLine,SOCKET sock){
    char rdir[5]="rdir";
    char send[5]="send";
    char chat[5]="chat";
    char exit[5]="exit";
	char fullfilename[fname+1];
    if(strncmp(cmdLine,rdir,4)==0){
        strcpy(des,cmdLine+5);
        return 1;

    }
    else if(strncmp(cmdLine,send,4)==0){
        strcpy(sendfilename,cmdLine+5);
		sendFilePacket(sock,sendfilename);
        return 1;

    }

    else if(strncmp(cmdLine,chat,4)==0){
        sendChatPacket(sock,cmdLine+5);
        return 1;
    }
    else if(strncmp(cmdLine,exit,4)==0){
        cout<<"exit server..."<<endl;
        system("pause");
        closesocket(sock);                             // 关闭监听套接字
        WSACleanup();
        return 0;
    }
    else{
        return 1;
    }
}

void sendChatPacket(SOCKET sock,char *chatData){
	flags[0]='1';
	send(sock,flags,1,0);
    send(sock,chatData,strlen(chatData),0);
}

void sendFilePacket(SOCKET sock, char * fullFileName){
    FILE *srcfile=NULL;	
	char buf[BUFLEN];   
	char filename[fname];
    char size[fname];
    int cc;
    if (fopen_s(&srcfile, fullFileName, "rb")!=0){
	printf("文件不存在！\n");
	return;
	}
	getFileName(fullFileName,filename);
    flags[0]='2';
	send(sock,flags,1,0);
	send(sock,filename,fname,0);

    __int64 filesize=getFileSize(fullFileName);
    itoa(filesize,size,10);
    send(sock,size,fname,0);

    int len=0;
    while(len<filesize){
        if(BUFLEN<=filesize-len){
            fread(buf,sizeof(char), BUFLEN, srcfile);
            len+=BUFLEN;
            send(sock,buf,BUFLEN,0);
        }
        else{
            fread(buf,sizeof(char), filesize-len, srcfile);
            send(sock,buf,filesize-len,0);
            len+=(filesize-len);
                
        }
    
    } 

    fclose(srcfile);

}

void getFileName(char *pathName,char* filename){

    char ch='\\';
    int namepos=strrchr(pathName,ch)-pathName;
    strcpy(filename,pathName+namepos+1);


}




int recvData(SOCKET sock, char *buf){
	int cc = recv(sock, buf, BUFLEN, 0); 
    if(cc == SOCKET_ERROR)
    printf("Error: %d.\n",GetLastError());     //出错。其后必须关闭套接字sock。
    else if(cc > 0) {
    cout<<">收到消息：";
    buf[cc] = '\0';	                       // ensure null-termination
    printf("%s\n",buf);                        
	return 1;
    }
    else{
    cout<<"服务器关闭"<<endl;
    }

	return 0;

}

int recvFile(SOCKET sock, char * fileName,char *path){
	FILE *destfile=NULL;
    char buf[BUFLEN];
	char orgname[fname];
    char filesize[fname];
    __int64 size;
	int cc;
	cc=recv(sock,fileName,fname,0);//读取文件名
	strcpy(orgname,fileName);
	getFullname(fileName,path);
    getUniqueName(fileName);
    
	if (fopen_s(&destfile,fileName, "wb")!=0){
	printf("目标文件夹未找到！\n");
	printf("按任意键继续...");
	return 0;
	}

    cc=recv(sock,filesize,fname,0);
    size=atoi(filesize);

    cout<<">收到文件："<<orgname<<endl;

    int len=0;
    while(len<size){
        if(BUFLEN<=size-len){
            cc=recv(sock, buf, BUFLEN, 0);
            len+=BUFLEN;
            fwrite(buf,1,BUFLEN,destfile);
        }
        else{
            recv(sock, buf, size-len, 0);
            fwrite(buf,1,size-len,destfile);
            len+=(size-len);
                
        }
    
    } 

     fclose(destfile);
     
     return 1;
}

void getUniqueName(char *newFileName){
    int cnt=1;
    char temp[fname]={};
    
    while(access(newFileName,0)==0){ //若文件已存在
        char str[10]={};
        int i=0,leftpos=0;
        for(i=0;i<strlen(newFileName);i++){
            if(newFileName[i]=='(')
            leftpos=i;
            if(newFileName[i]=='.')
            break;
        }
        if(leftpos!=0){
            strncpy(temp,newFileName+i,fname);
            newFileName[leftpos]='\0';
            strncat(newFileName,temp,fname);

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
        strncpy(newFileName,temp,fname);    
    }
    
}

void getFullname(char *name,char *desfname){
  strcpy(buf,desfname);
  char ch[3]="\\";
  strcat(buf,ch);
  strcat(buf,name);
  strcpy(name,buf);

}

