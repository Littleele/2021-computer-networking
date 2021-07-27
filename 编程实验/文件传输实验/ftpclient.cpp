#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include<iostream>
#include<process.h>
using namespace std;
#define	BUFLEN 2000                  // ��������С
#define FNAME_LEN 300
#define WSVERS		MAKEWORD(2, 0)        // ָ���汾2.0 
#pragma comment(lib,"ws2_32.lib")         // ʹ��winsock 2.0 Llibrary

char	buf[BUFLEN+1];   		        /* buffer for one line of text	*/

void getfilename(const char*srcname,char*filename){
     char ch='\\';
     int namepos=strrchr(srcname,ch)-srcname;
     strcpy(filename,srcname+namepos+1);

}



int saveFile(SOCKET sock, char * fileName,const char*destname) {
     FILE *destfile=NULL;
     char	buf[BUFLEN+1]; 
     if (fopen_s(&destfile,destname, "wb")!=0){
	printf("Ŀ���ļ���δ�ҵ���\n");
	printf("�����������...");
	getchar();
	getchar();
	exit(1);
	}
    
     fwrite(fileName,FNAME_LEN,1,destfile);//д���ļ���
     __int64 filesize=0;

     while(1){
          int cc;
          while((cc = recv(sock, buf, BUFLEN, 0))>0){	                       // ensure null-termination
               filesize+=cc;
               fwrite(buf, 1, cc, destfile);
 

          } 
          if(cc == SOCKET_ERROR){
               printf("Error: %d.\n",GetLastError());     //����������ر��׽���sock��
               return 0;
          }
          else if(cc==0){
               cout<<"���ؽ���"<<endl;
               closesocket(sock); // �رռ����׽���
			WSACleanup();	  // ж��winsock library
			break;
          }


     }
     
     fclose(destfile);
     
     return 1;
     
}

void recvthread(SOCKET ssock){
    int cc = recv(ssock, buf, BUFLEN, 0);
    if(cc > 0) {
         buf[cc] = '\0';	                       // ensure null-termination
         cout<<buf;                        // ��ʾ�����յ��ַ���
    }                
    else if(cc == SOCKET_ERROR)                          // ����������ر��׽���sock
         printf("Error: %d.\n",GetLastError());
     else if(cc == 0) {                             // �Է������ر�
         printf("Server closed!",buf);  
    }  
     

}

int dataport(char*buf,const char*host){
     int ip1,ip2,ip3,ip4,port1,port2;
     char*ip=strchr(buf,'(');
     sscanf(ip,"(%d,%d,%d,%d,%d,%d).", &ip1, &ip2, &ip3, &ip4, &port1, &port2);
     int port=port1*256+port2;
     return port; 
}

SOCKET dataconnet(int port,const char*host){     
     struct sockaddr_in sin;
     SOCKET sock;                              
  
     WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);	
     sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //�����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
														  //���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET
    memset(&sin, 0, sizeof(sin));						  // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
    sin.sin_family = AF_INET;							  // ��������ַ��
    sin.sin_addr.s_addr = inet_addr(host);                // ������IP��ַ(32λ)
    sin.sin_port = htons((u_short)port);       
    int ret=connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // ���ӵ����������ڶ�������ָ���ŷ�������ַ�Ľṹ������������Ϊ�ýṹ�Ĵ�С������ֵΪ0ʱ��ʾ�޴�����������SOCKET_ERROR��ʾ����Ӧ�ó����ͨ��WSAGetLastError()��ȡ��Ӧ������롣
     if (ret == -1)
	{
		cout<<"Failed to connect server!"<<endl;
          printf("Error: %d.\n",GetLastError()); 
		closesocket(sock);
		WSACleanup();
          system("pause");
		exit(1);
	}
     return sock;
     
}

int main(int argc, char *argv[])
{

	struct sockaddr_in sin;	            /* an Internet endpoint address	*/
     
	char filename[FNAME_LEN]={};
     char msg[BUFLEN+1]={};
	SOCKET sock;		  	            /* socket descriptor	    	*/
     SOCKET datasock;
	int	cc;			                    /* recv character count		    */
    if(argc!=4){
        cout<<"Command Error! Please input: TCPClient IPAdress PortNumber"<<endl;
        system("pause");
        exit(1);
     }

     //argv[1]="172.18.187.251";
    const char* host = argv[1];
    //argv[2]="\\ebook\\ftp.pdf";	    /* server IP to connect         */
    const char* src = argv[2]; 
     //argv[3]="C:\\Users\\18230\\Desktop\\test";
    const char* des=argv[3];

	WSADATA wsadata;
	WSAStartup(WSVERS, &wsadata);						  //����winsock library��WSVERSΪ����İ汾��wsadata����ϵͳʵ��֧�ֵ���߰汾
  
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //�����׽��֣�������������Э���(family)�����׽��֣�TCPЭ��
														  //���أ�Ҫ�����׽��ֵ���������INVALID_SOCKET
    memset(&sin, 0, sizeof(sin));						  // ��&sin��ʼ�ĳ���Ϊsizeof(sin)���ڴ���0
    sin.sin_family = AF_INET;							  // ��������ַ��
    sin.sin_addr.s_addr = inet_addr(host);                // ������IP��ַ(32λ)
    sin.sin_port = htons((u_short)21);       
    int ret=connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // ���ӵ����������ڶ�������ָ���ŷ�������ַ�Ľṹ������������Ϊ�ýṹ�Ĵ�С������ֵΪ0ʱ��ʾ�޴�����������SOCKET_ERROR��ʾ����Ӧ�ó����ͨ��WSAGetLastError()��ȡ��Ӧ������롣
     if (ret == -1)
	{
		cout<<"Failed to connect server!"<<endl;
          printf("Error: %d.\n",GetLastError()); 
		closesocket(sock);
		WSACleanup();
          system("pause");
		exit(1);
	}

     getfilename(src,filename);

     int sendlen;
     int len;
     sprintf(msg,"user net\r\n");
     cout<<msg;
     len=strlen(msg);
     sendlen=send(sock,msg,len+1,0);
     recvthread(sock);
     recvthread(sock);

     sprintf(msg,"pass 123456\r\n");
     cout<<msg;
     len=strlen(msg);
     sendlen=send(sock,msg,len+1,0);
     recvthread(sock);
     
     sprintf(msg,"pasv\r\n");
     cout<<msg;
     len=strlen(msg);
     sendlen=send(sock,msg,len+1,0);
     recvthread(sock);

     int port;                          //������������
     port=dataport(buf,host);
     datasock=dataconnet(port,host);

     sprintf(msg, "retr %s\r\n", src);
     cout<<msg;
     len=strlen(msg);
     sendlen=send(sock,msg,len+1,0);
     recvthread(sock);
     recvthread(sock);
     
     cout<<"��ʼ�����ļ�...\n\r";
     saveFile(datasock,filename,des);

     sprintf(msg, "quit\r\n");
     cout<<msg;
     len=strlen(msg);
     sendlen=send(sock,msg,len+1,0);
     recvthread(sock);


     closesocket(sock);                             // �رռ����׽���
     WSACleanup();                                  // ж��winsock library
     
     return 0;									// �ȴ����ⰴ��
}

