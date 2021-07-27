/* TCPClient.cpp */
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
using namespace std;
#define	BUFLEN		2000                  // 缓冲区大小
#define WSVERS		MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 使用winsock 2.0 Llibrary

/*------------------------------------------------------------------------
 * main - TCP client for TIME service
 *------------------------------------------------------------------------
 */
int main(int argc, char *argv[])
{
	const char *host = "127.0.0.1";	    /* server IP to connect         */
	const char *service = "50500";  	    /* server port to connect       */
	struct sockaddr_in sin;	            /* an Internet endpoint address	*/
	char	buf[BUFLEN+1];   		    /* buffer for one line of text	*/
	SOCKET	sock;		  	            /* socket descriptor	    	*/
	int	cc;			                    /* recv character count		    */

	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);						  //加载winsock library。WSVERS为请求的版本，wsadata返回系统实际支持的最高版本
  
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
														  //返回：要监听套接字的描述符或INVALID_SOCKET

    memset(&sin, 0, sizeof(sin));						  // 从&sin开始的长度为sizeof(sin)的内存清0
    sin.sin_family = AF_INET;							  // 因特网地址簇
    sin.sin_addr.s_addr = inet_addr(host);                // 设置服务器IP地址(32位)
    sin.sin_port = htons((u_short)atoi(service));         // 设置服务器端口号  

    ,0

    cout<<"输入要发送的信息： ";
    fgets(buf,BUFLEN,stdin);
    buf[strlen(buf)-1]='\0';
    cc=send(sock,buf,BUFLEN,0);

    cc = recv(sock, buf, BUFLEN, 0);                // 第二个参数指向缓冲区，第三个参数为缓冲区大小(字节数)，第四个参数一般设置为0，返回值:(>0)接收到的字节数,(=0)对方已关闭,(<0)连接出错
    if(cc == SOCKET_ERROR)                          // 出错。其后必须关闭套接字sock
         printf("Error: %d.\n",GetLastError());
     else if(cc == 0) {                             // 对方正常关闭
         printf("Server closed!",buf);  
    }  else if(cc > 0) {
         buf[cc] = '\0';	                       // ensure null-termination
         cout<<"收到的消息是： "<<endl;
         cout<<buf;                        // 显示所接收的字符串
    }
    cout<<endl;

    closesocket(sock);                             // 关闭监听套接字
    WSACleanup();                                  // 卸载winsock library

    printf("按回车键继续...");
	getchar();										// 等待任意按键
	getchar();

     
     return 0;


}
