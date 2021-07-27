/* TCPServer.cpp - main */
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include<sstream>
#include "conio.h"
using namespace std;
#define	WSVERS	MAKEWORD(2, 0)
#define	BUFLEN		2000   
char	buf[BUFLEN+1]; 
#pragma comment(lib,"ws2_32.lib")  //使用winsock 2.2 library
/*------------------------------------------------------------------------
 * main - Iterative TCP server for TIME service
 *------------------------------------------------------------------------
 */
void sendmessage(char* buf,char* pts,struct	sockaddr_in fsin,SOCKET ssock){
	ostringstream ss;
	ss<<"内容： "<<buf<<endl
	<<"时间："<<pts
	<<"客户端IP地址： "<<(int)fsin.sin_addr.S_un.S_un_b.s_b1<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b2
	<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b3<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b4<<endl
	<<"客户端的端口号： "<<fsin.sin_port<<endl;
	memset(buf,'\0',sizeof(buf));
	strcpy(buf,ss.str().c_str());
	int cc = send(ssock, buf, strlen(buf), 0);  


}
int main(int argc, char *argv[]) 
/* argc: 命令行参数个数， 例如：C:\> TCPServer 8080 
                     argc=2 argv[0]="TCPServer",argv[1]="8080" */
{
	struct	sockaddr_in fsin;	    /* the from address of a client	  */
	SOCKET	msock, ssock;		    /* master & slave sockets	      */
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
	cout<<"服务器已启动！"<<endl;

    while(!_kbhit()){ 		                                   // 检测是否有按键,如果没有则进入循环体执行
       alen = sizeof(struct sockaddr);                         // 取到地址结构的长度
	   ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // 如果在连接请求队列中有连接请求，则接受连接请求并建立连接，返回该连接的套接字，否则，本语句被阻塞直到队列非空。fsin包含客户端IP地址和端口号
	    
       (void) time(&now);                                      // 取得系统时间
       pts = ctime(&now);                                      // 把时间转换为字符串
	   cc = recv(ssock, buf, BUFLEN, 0);                // 第二个参数指向缓冲区，第三个参数为缓冲区大小(字节数)，第四个参数一般设置为0，返回值:(>0)接收到的字节数,(=0)对方已关闭,(<0)连接出错
    	if(cc == SOCKET_ERROR)                          // 出错。其后必须关闭套接字sock
        printf("Error: %d.\n",GetLastError());
     	else if(cc == 0) {                             // 对方正常关闭
         printf("Server closed!",buf);  
        }  else if(cc > 0) {
        buf[cc] = '\0';	                       // ensure null-termination
        cout<<"收到消息："<<buf<<endl;                        // 显示所接收的字符串
        }

		cout<<"收到时间： "<<pts;
    	cout<<"客户端IP地址： "<<(int)fsin.sin_addr.S_un.S_un_b.s_b1<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b2
		<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b3<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b4<<endl;
    	cout<<"客户端端口号： "<<fsin.sin_port<<endl;
		
		cout<<endl;
		sendmessage(buf,pts,fsin,ssock);

       (void) closesocket(ssock);                              // 关闭连接套接字
    }
    (void) closesocket(msock);                                 // 关闭监听套接字
     WSACleanup();                                             // 卸载winsock library
     printf("按回车键继续...");
 	 getchar();										// 等待任意按键
	 getchar();


}

