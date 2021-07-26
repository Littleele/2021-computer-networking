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
#pragma comment(lib,"ws2_32.lib")  //ʹ��winsock 2.2 library
/*------------------------------------------------------------------------
 * main - Iterative TCP server for TIME service
 *------------------------------------------------------------------------
 */
void sendmessage(char* buf,char* pts,struct	sockaddr_in fsin,SOCKET ssock){
	ostringstream ss;
	ss<<"���ݣ� "<<buf<<endl
	<<"ʱ�䣺"<<pts
	<<"�ͻ���IP��ַ�� "<<(int)fsin.sin_addr.S_un.S_un_b.s_b1<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b2
	<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b3<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b4<<endl
	<<"�ͻ��˵Ķ˿ںţ� "<<fsin.sin_port<<endl;
	memset(buf,'\0',sizeof(buf));
	strcpy(buf,ss.str().c_str());
	int cc = send(ssock, buf, strlen(buf), 0);  


}
int main(int argc, char *argv[]) 
/* argc: �����в��������� ���磺C:\> TCPServer 8080 
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

	WSAStartup(WSVERS, &wsadata);						// ����winsock library��WSVERSָ������ʹ�õİ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// �����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
														// ���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET
	
	memset(&sin, 0, sizeof(sin));						// ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;							// ��������ַ��(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// ��������(�ӿڵ�)IP��ַ��
	sin.sin_port = htons((u_short)atoi(service));		// �����Ķ˿ںš�atoi--��asciiת��Ϊint��htons--������������(host to network��s-short 16λ)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // �󶨼�����IP��ַ�Ͷ˿ں�
	listen(msock, 5);                                   // ��������Ϊ5������������У����ѵ������������������еȴ�����
	cout<<"��������������"<<endl;

    while(!_kbhit()){ 		                                   // ����Ƿ��а���,���û�������ѭ����ִ��
       alen = sizeof(struct sockaddr);                         // ȡ����ַ�ṹ�ĳ���
	   ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // ����������������������������������������󲢽������ӣ����ظ����ӵ��׽��֣����򣬱���䱻����ֱ�����зǿա�fsin�����ͻ���IP��ַ�Ͷ˿ں�
	    
       (void) time(&now);                                      // ȡ��ϵͳʱ��
       pts = ctime(&now);                                      // ��ʱ��ת��Ϊ�ַ���
	   cc = recv(ssock, buf, BUFLEN, 0);                // �ڶ�������ָ�򻺳���������������Ϊ��������С(�ֽ���)�����ĸ�����һ������Ϊ0������ֵ:(>0)���յ����ֽ���,(=0)�Է��ѹر�,(<0)���ӳ���
    	if(cc == SOCKET_ERROR)                          // ����������ر��׽���sock
        printf("Error: %d.\n",GetLastError());
     	else if(cc == 0) {                             // �Է������ر�
         printf("Server closed!",buf);  
        }  else if(cc > 0) {
        buf[cc] = '\0';	                       // ensure null-termination
        cout<<"�յ���Ϣ��"<<buf<<endl;                        // ��ʾ�����յ��ַ���
        }

		cout<<"�յ�ʱ�䣺 "<<pts;
    	cout<<"�ͻ���IP��ַ�� "<<(int)fsin.sin_addr.S_un.S_un_b.s_b1<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b2
		<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b3<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b4<<endl;
    	cout<<"�ͻ��˶˿ںţ� "<<fsin.sin_port<<endl;
		
		cout<<endl;
		sendmessage(buf,pts,fsin,ssock);

       (void) closesocket(ssock);                              // �ر������׽���
    }
    (void) closesocket(msock);                                 // �رռ����׽���
     WSACleanup();                                             // ж��winsock library
     printf("���س�������...");
 	 getchar();										// �ȴ����ⰴ��
	 getchar();


}

