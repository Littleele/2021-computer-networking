#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include<iostream>
#include<process.h>
using namespace std;
#define	BUFLEN		2000                  // 缓冲区大小
#define WSVERS		MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 使用winsock 2.0 Llibrary

unsigned __stdcall recvthread(void *p){
     char	buf[BUFLEN+1]; 
     SOCKET ssock=*(SOCKET*)p;
     while(1){
          int cc;
          while((cc = recv(ssock, buf, BUFLEN, 0))>0){
               buf[cc] = '\0';	                       // ensure null-termination
               printf("%s",buf); 

          } 
          if(cc == SOCKET_ERROR){
               printf("Error: %d.\n",GetLastError());     //出错。其后必须关闭套接字sock。
               exit(1);
          }
          else{
               cout<<"Connection closed!"<<endl;
               closesocket(ssock); // 关闭监听套接字
			WSACleanup();	  // 卸载winsock library
			exit(1);  //退出程序
			break;
          }


     }
     
    return 0;

}


int main(int argc, char *argv[])
{

	struct sockaddr_in sin;	            /* an Internet endpoint address	*/
	char	buf[BUFLEN+1];   		        /* buffer for one line of text	*/
	SOCKET	sock;		  	            /* socket descriptor	    	*/
	int	cc;			                    /* recv character count		    */
    if(argc!=3){
        cout<<"Command Error! Please input: TCPClient IPAdress PortNumber"<<endl;
        system("pause");
        exit(1);
    }

    const char* host = argv[1];	    /* server IP to connect         */
    const char* service = argv[2];  	    /* server port to connect       */

	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);						  //加载winsock library。WSVERS为请求的版本，wsadata返回系统实际支持的最高版本
  
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
														  //返回：要监听套接字的描述符或INVALID_SOCKET
    memset(&sin, 0, sizeof(sin));						  // 从&sin开始的长度为sizeof(sin)的内存清0
    sin.sin_family = AF_INET;							  // 因特网地址簇
    sin.sin_addr.s_addr = inet_addr(host);                // 服务器IP地址(32位)
    sin.sin_port = htons((u_short)atoi(service));         // 服务器端口号  
    int ret=connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // 连接到服务器，第二个参数指向存放服务器地址的结构，第三个参数为该结构的大小，返回值为0时表示无错误发生，返回SOCKET_ERROR表示出错，应用程序可通过WSAGetLastError()获取相应错误代码。
     if (ret == -1)
	{
		cout<<"连接到服务器失败！"<<endl;
          printf("Error: %d.\n",GetLastError()); 
		closesocket(sock);
		WSACleanup();
          system("pause");
		exit(1);
	}

     HANDLE recvhandle=(HANDLE)_beginthreadex(NULL,0,&recvthread,(void*)&sock,0,NULL);
     
     while(1){
                  
          fgets(buf,BUFLEN,stdin);
          int len=strlen(buf);
          buf[len-1]='\r';
          buf[len]='\n';        //cmd格式
         
          int sendlen=send(sock,buf,len+1,0);
     }
     
     CloseHandle(recvhandle);
     closesocket(sock);                             // 关闭监听套接字
     WSACleanup();                                  // 卸载winsock library
     
     return 0;									// 等待任意按键
}

