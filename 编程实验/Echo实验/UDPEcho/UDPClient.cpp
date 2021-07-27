/* UDPClient.cpp */
#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <time.h>
using namespace std;
#define	BUFLEN		2000                  // ��������С
#define WSVERS		MAKEWORD(2, 2)        // ָ���汾2.2 
#pragma comment(lib,"ws2_32.lib")         // ����winsock 2.2 Llibrary

int  main(int argc, char *argv[])
{
	const char	*host = "127.0.0.1";	    /* server IP to connect         */
	const char	*service = "50500";  	    /* server port to connect       */
	struct sockaddr_in toAddr;	        /* an Internet endpoint address	*/
	char	buf[BUFLEN+1];   		    /* buffer for one line of text	*/
	SOCKET	sock;		  	            /* socket descriptor	    	*/
	int	cc;			                    /* recv character count		    */
	char	*pts;			            /* pointer to time string	    */
	time_t	now;			            /* current time			        */

	WSADATA wsadata;
    WSAStartup(WSVERS, &wsadata);       /* ����ĳ�汾Socket��DLL        */	

    sock = socket(PF_INET, SOCK_DGRAM,IPPROTO_UDP);

	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons((u_short)atoi(service));    //atoi����asciiת��Ϊint. htons��������(host)ת��Ϊ������(network), s--short
	toAddr.sin_addr.s_addr = inet_addr(host);           //���hostΪ��������Ҫ���ú���gethostbyname������ת��ΪIP��ַ


	memset(buf,'\0',BUFLEN);  //fill 1000 bytes with 'e'
    (void) time(&now);                                      // ȡ��ϵͳʱ��
    pts = ctime(&now);                                      // ��ʱ��ת��Ϊ�ַ���
    memcpy(buf,pts,strlen(pts));

	cout<<"������Ϣ�� ";
	fgets(buf,BUFLEN,stdin);
	buf[strlen(buf)-1]='\0';
	cout<<endl;

	cc = sendto(sock, buf, BUFLEN, 0,(SOCKADDR *)&toAddr, sizeof(toAddr));
    if (cc == SOCKET_ERROR){
        printf("����ʧ�ܣ�����ţ�%d\n", WSAGetLastError());
     }
	else{
		cout<<endl;
		int tosize = sizeof(toAddr);
		cc = recvfrom(sock, buf, BUFLEN, 0, (SOCKADDR *)&toAddr, &tosize);
		if(cc==SOCKET_ERROR){
			cout<<"����ʧ�ܣ�����ţ�"<<WSAGetLastError()<<endl;
		}
		if(cc==0){
			return 0;
		}
		else 
		cout<<buf;

	}
	

	closesocket(sock);
	WSACleanup();       	          /* ж��ĳ�汾��DLL */  

	printf("��������˳�...");
	getchar();

}
