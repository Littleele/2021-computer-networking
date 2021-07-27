/* UDPServer.cpp */
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include<time.h>
#include<sstream>
#include "conio.h"
using namespace std;

#define	BUFLEN		2000                  // ��������С
#define WSVERS		MAKEWORD(2, 2)        // ָ���汾2.2 
#pragma comment(lib,"ws2_32.lib")         // ����winsock 2.2 Llibrary

/*------------------------------------------------------------------------
 * main - TCP client for DAYTIME service
 *------------------------------------------------------------------------
 */

void sendmessage(char* buf,char* pts,struct	sockaddr_in fsin){
	ostringstream ss;
	ss<<"�ͻ��˵���Ϣ�� "<<buf<<endl
	<<"�ͻ���IP��ַ�� "<<(int)fsin.sin_addr.S_un.S_un_b.s_b1<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b2
	<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b3<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b4<<endl
	<<"�ͻ��˵Ķ˿ںţ� "<<fsin.sin_port<<endl
	<<"ʱ�䣺"<<pts<<endl;

	memset(buf,'\0',sizeof(buf));
	strcpy(buf,ss.str().c_str());
	

}

int main(int argc, char *argv[])
{
	char   *host = "127.0.0.1";	    /* server IP Address to connect */
	char   *service = "50500";  	    /* server port to connect       */
	struct sockaddr_in sin;	        /* an Internet endpoint address	*/
	struct sockaddr_in from;        /* sender address               */
	int    fromsize = sizeof(from);
	char   buf[BUFLEN+1];   	    /* buffer for one line of text	*/
	SOCKET	sock;		  	        /* socket descriptor	    	*/
	int	cc;			                /* recv character count		    */
	char	*pts;
	time_t	now;

	WSADATA wsadata;
    WSAStartup(WSVERS, &wsadata);   /* ����winsock library��WSVERSΪ����汾,wsadata����ϵͳʵ��֧�ֵ���߰汾��    */		
    sock = socket(PF_INET, SOCK_DGRAM,IPPROTO_UDP); // ����UDP�׽���, ������������Э���(family)�����ݱ��׽��֣�UDPЭ��ţ� ���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;                     // ��(����)���еĽӿڡ�
	sin.sin_port = htons((u_short)atoi(service));         // ��ָ���ӿڡ�atoi--��asciiת��Ϊint��htons -- ������(host)ת��Ϊ������(network), Ϊshort���͡� 
	bind(sock, (struct sockaddr *)&sin, sizeof(sin));     // �󶨱��ض˿ںţ��ͱ���IP��ַ)
	
	cout<<"��������������"<<endl;
	(void) time(&now);                                      // ȡ��ϵͳʱ��
    pts = ctime(&now);     
	                                 // ��ʱ��ת��Ϊ�ַ���
 	while(!_kbhit()){        
		cc = recvfrom(sock, buf, BUFLEN, 0, (SOCKADDR *)&from, &fromsize);  //���տͻ����ݡ����ؽ����ccΪ���յ��ַ�����from�н������ͻ�IP��ַ�Ͷ˿ںš�                            //����Ƿ��а���
		if (cc == SOCKET_ERROR){
            printf("recvfrom() failed; %d\n", WSAGetLastError());
            break;
        }
        else if (cc == 0)
            break;
		else{
			
        	buf[cc] = '\0';
			cout<<"�ͻ��˵���Ϣ��  "<<buf<<endl;
			cout<<"�ͻ���IP��ַ�� "<<(int)from.sin_addr.S_un.S_un_b.s_b1<<'.'<<(int)from.sin_addr.S_un.S_un_b.s_b2
			<<'.'<<(int)from.sin_addr.S_un.S_un_b.s_b3<<'.'<<(int)from.sin_addr.S_un.S_un_b.s_b4<<endl;
    		cout<<"�ͻ��˶˿ںţ� "<<from.sin_port<<endl;
			cout<<"ʱ�䣺 "<<pts<<endl;

			sendmessage(buf,pts,from);
			sendto(sock, buf, BUFLEN, 0, (SOCKADDR *)&from, sizeof(from));
		}
	}
	closesocket(sock);
	WSACleanup();       	          /* ж��ĳ�汾��DLL */
	getchar();
}



