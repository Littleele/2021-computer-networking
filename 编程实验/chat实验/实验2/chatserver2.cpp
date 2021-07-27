#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include<sstream>
#include "conio.h"
#include<process.h>
using namespace std;

#define MAXLEN	   5
#define	WSVERS	MAKEWORD(2, 0)
#define	BUFLEN		2000   
char	buf[BUFLEN+1]; 
#pragma comment(lib,"ws2_32.lib")  //使用winsock 2.2 library


struct Thread{
    struct sockaddr_in fsin;
    SOCKET sock;//与客户端链接的套接字
    HANDLE handle;
    int num;

    Thread(){
        handle=NULL;
    }
};

Thread thread[MAXLEN];

void message(char* buf,struct	sockaddr_in fsin){
	ostringstream ss;
    time_t now = time(NULL);
	ss<<"消息： "<<buf<<endl
	<<"时间："<<ctime(&now)
	<<"客户端IP地址： "<<(int)fsin.sin_addr.S_un.S_un_b.s_b1<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b2
	<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b3<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b4<<endl
	<<"客户端端口号： "<<fsin.sin_port;
	memset(buf,'\0',sizeof(buf));
	strcpy(buf,ss.str().c_str());
	
}

void sendtoall(const char*p){
    for(int i=0;i<MAXLEN;i++){
        if(thread[i].handle!=NULL){
            send(thread[i].sock,p,BUFLEN,0);
        }
    }
}

unsigned __stdcall recvseverthread(void *p){
    Thread newthread=*(Thread*)p;
    char buf[BUFLEN+1];
    while(1){
        int len=recv(newthread.sock,buf,BUFLEN,0);
        if(len == SOCKET_ERROR){
            printf("Error: %d.\n",GetLastError()); 
            cout<<"---------------------------------------"<<endl;
            break;
        }
        else if(len==0){
            cout<<"客户端正常退出"<<endl;
            cout<<"---------------------------------------"<<endl;
            break;
        } 
        else {
          cout<<"服务器收到内容："<<endl;
          buf[len] = '\0';	                       // ensure null-termination
          message(buf,newthread.fsin);
          printf("%s\n",buf);                         // 显示所接收的字符串
          sendtoall(buf);
          cout<<"---------------------------------------"<<endl;
        }
    }
        thread[newthread.num].handle=NULL;
        closesocket(newthread.sock);
        return 0;
    }


int main(int argc, char *argv[]) 
/* argc: 命令行参数个数， 例如：C:\> TCPServer 8080 
                     argc=2 argv[0]="TCPServer",argv[1]="8080" */
{
	struct	sockaddr_in fsin;	    /* the from address of a client	  */
	SOCKET	msock;		    /* master & slave sockets	      */
	WSADATA wsadata; 
	const char	*service = "50500";
	struct  sockaddr_in sin;	    /* an Internet endpoint address		*/
    int	    alen;			        /* from-address length		        */
	char	*pts;			        /* pointer to time string	        */
	time_t	now;			        /* current time			            */

	int cc;
	WSAStartup(WSVERS, &wsadata);						// 加载winsock library。WSVERS指明请求使用的版本。wsadata返回系统实际支持的最高版本
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// 创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
														// 返回：要监听套接字的描述符或INVALID_SOCKET
	memset(&sin, 0, sizeof(sin));						// 从&sin开始的长度为sizeof(sin)的内存清0
	sin.sin_family = AF_INET;							// 因特网地址簇(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// 监听所有(接口的)IP地址。
	sin.sin_port = htons((u_short)atoi(service));		// 监听的端口号。atoi--把ascii转化为int，htons--主机序到网络序(host to network，s-short 16位)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // 绑定监听的IP地址和端口号

	listen(msock, 5);                                   // 建立长度为5的连接请求队列，并把到来的连接请求加入队列等待处理。
	cout<<"群聊服务器已启动！"<<endl;
    now = time(NULL);
    pts = ctime(&now);
	cout<<"启动时间："<<pts;
    cout<<"***************************************\n";
 

	 
    while(!_kbhit()){ 		                                   // 检测是否有按键,如果没有则进入循环体执行
       alen = sizeof(struct sockaddr);                         // 取到地址结构的长度
	   SOCKET ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // 如果在连接请求队列中有连接请求，则接受连接请求并建立连接，返回该连接的套接字，否则，本语句被阻塞直到队列非空。fsin包含客户端IP地址和端口号

        int full=1;
       for(int i=0;i<MAXLEN;i++){
           if(thread[i].handle==NULL){
               thread[i].sock=ssock;
               thread[i].num=i;
               thread[i].fsin=fsin;
               thread[i].handle=(HANDLE)_beginthreadex(NULL, 0, &recvseverthread, (void*)&thread[i], 0, NULL);
               full=0;
               CloseHandle(thread[i].handle);
               break;
           }
       } 

       if(full)
       cout<<"群聊人数已满";
                     
     }

    (void) closesocket(msock);                                 // 关闭监听套接字
     WSACleanup();                                             // 卸载winsock library
     printf("按回车键继续...");
 	 getchar();										// 等待任意按键
	 getchar();


}

