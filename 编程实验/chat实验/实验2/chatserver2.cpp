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
#pragma comment(lib,"ws2_32.lib")  //ʹ��winsock 2.2 library


struct Thread{
    struct sockaddr_in fsin;
    SOCKET sock;//��ͻ������ӵ��׽���
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
	ss<<"��Ϣ�� "<<buf<<endl
	<<"ʱ�䣺"<<ctime(&now)
	<<"�ͻ���IP��ַ�� "<<(int)fsin.sin_addr.S_un.S_un_b.s_b1<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b2
	<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b3<<'.'<<(int)fsin.sin_addr.S_un.S_un_b.s_b4<<endl
	<<"�ͻ��˶˿ںţ� "<<fsin.sin_port;
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
            cout<<"�ͻ��������˳�"<<endl;
            cout<<"---------------------------------------"<<endl;
            break;
        } 
        else {
          cout<<"�������յ����ݣ�"<<endl;
          buf[len] = '\0';	                       // ensure null-termination
          message(buf,newthread.fsin);
          printf("%s\n",buf);                         // ��ʾ�����յ��ַ���
          sendtoall(buf);
          cout<<"---------------------------------------"<<endl;
        }
    }
        thread[newthread.num].handle=NULL;
        closesocket(newthread.sock);
        return 0;
    }


int main(int argc, char *argv[]) 
/* argc: �����в��������� ���磺C:\> TCPServer 8080 
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
	WSAStartup(WSVERS, &wsadata);						// ����winsock library��WSVERSָ������ʹ�õİ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
	msock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	// �����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
														// ���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET
	memset(&sin, 0, sizeof(sin));						// ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
	sin.sin_family = AF_INET;							// ��������ַ��(INET-Internet)
	sin.sin_addr.s_addr = INADDR_ANY;					// ��������(�ӿڵ�)IP��ַ��
	sin.sin_port = htons((u_short)atoi(service));		// �����Ķ˿ںš�atoi--��asciiת��Ϊint��htons--������������(host to network��s-short 16λ)
	bind(msock, (struct sockaddr *)&sin, sizeof(sin));  // �󶨼�����IP��ַ�Ͷ˿ں�

	listen(msock, 5);                                   // ��������Ϊ5������������У����ѵ������������������еȴ�����
	cout<<"Ⱥ�ķ�������������"<<endl;
    now = time(NULL);
    pts = ctime(&now);
	cout<<"����ʱ�䣺"<<pts;
    cout<<"***************************************\n";
 

	 
    while(!_kbhit()){ 		                                   // ����Ƿ��а���,���û�������ѭ����ִ��
       alen = sizeof(struct sockaddr);                         // ȡ����ַ�ṹ�ĳ���
	   SOCKET ssock = accept(msock, (struct sockaddr *)&fsin, &alen); // ����������������������������������������󲢽������ӣ����ظ����ӵ��׽��֣����򣬱���䱻����ֱ�����зǿա�fsin�����ͻ���IP��ַ�Ͷ˿ں�

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
       cout<<"Ⱥ����������";
                     
     }

    (void) closesocket(msock);                                 // �رռ����׽���
     WSACleanup();                                             // ж��winsock library
     printf("���س�������...");
 	 getchar();										// �ȴ����ⰴ��
	 getchar();


}

