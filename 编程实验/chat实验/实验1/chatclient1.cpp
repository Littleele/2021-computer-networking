#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include<iostream>
#include<process.h>
using namespace std;
#define	BUFLEN		2000                  // ��������С
#define WSVERS		MAKEWORD(2, 0)        // ָ���汾2.0 
#pragma comment(lib,"ws2_32.lib")         // ʹ��winsock 2.0 Llibrary

unsigned __stdcall recvthread(void *p){
     char	buf[BUFLEN+1]; 
     SOCKET ssock=*(SOCKET*)p;
     while(1){
          cout<<"������Ϣ��";   
          int cc = recv(ssock, buf, BUFLEN, 0); 
          cout<<"\r";//��û������Ϣ ����
          if(cc == SOCKET_ERROR)
         printf("Error: %d.\n",GetLastError());     //����������ر��׽���sock��
          else if(cc > 0) {
          cout<<"�յ���Ϣ��"<<endl;
          buf[cc] = '\0';	                       // ensure null-termination
          printf("%s\n",buf);                         // ��ʾ�����յ��ַ���
          }
          else{
               cout<<"�������ر�"<<endl;
          }
          cout<<"---------------------------------------"<<endl;

     }
     
    return 0;

}


int main(int argc, char *argv[])
{
	const char* host = "127.0.0.1";	    /* server IP to connect         */
	const char* service = "50500";  	    /* server port to connect       */
	struct sockaddr_in sin;	            /* an Internet endpoint address	*/
	char	buf[BUFLEN+1];   		        /* buffer for one line of text	*/
	SOCKET	sock;		  	            /* socket descriptor	    	*/
	int	cc;			                    /* recv character count		    */

	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);						  //����winsock library��WSVERSΪ����İ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
  
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //�����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
														  //���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET
    memset(&sin, 0, sizeof(sin));						  // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
    sin.sin_family = AF_INET;							  // ��������ַ��
    sin.sin_addr.s_addr = inet_addr(host);                // ������IP��ַ(32λ)
    sin.sin_port = htons((u_short)atoi(service));         // �������˿ں�  
    int ret=connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // ���ӵ����������ڶ�������ָ���ŷ�������ַ�Ľṹ������������Ϊ�ýṹ�Ĵ�С������ֵΪ0ʱ��ʾ�޴�����������SOCKET_ERROR��ʾ����Ӧ�ó����ͨ��WSAGetLastError()��ȡ��Ӧ������롣
     if (ret == -1)
	{
		cout<<"���ӵ�������ʧ�ܣ�"<<endl;
		closesocket(sock);
		WSACleanup();
          system("pause");
		exit(1);
	}
     else if(ret==0)
     {
          cout<<"�ɹ����ӵ���������"<<endl;
          cout<<"***************************************"<<endl;
     }

     HANDLE recvhandle=(HANDLE)_beginthreadex(NULL,0,&recvthread,(void*)&sock,0,NULL);
     
     while(1){
                  
          fgets(buf,BUFLEN,stdin);
          buf[strlen(buf)-1]='\0';
          int sendlen=send(sock,buf,BUFLEN,0);
     }
     
     CloseHandle(recvhandle);
     closesocket(sock);                             // �رռ����׽���
     WSACleanup();                                  // ж��winsock library
     

     printf("���س�������...");
     getchar();										// �ȴ����ⰴ��
}

