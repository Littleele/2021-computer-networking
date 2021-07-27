#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include<iostream>
#include<process.h>
using namespace std;
#define	BUFLEN 2000                  // 缓冲区大小
#define FNAME_LEN 300
#define WSVERS		MAKEWORD(2, 0)        // 指明版本2.0 
#pragma comment(lib,"ws2_32.lib")         // 使用winsock 2.0 Llibrary

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
	printf("目标文件夹未找到！\n");
	printf("按任意键继续...");
	getchar();
	getchar();
	exit(1);
	}
    
     fwrite(fileName,FNAME_LEN,1,destfile);//写入文件名
     __int64 filesize=0;

     while(1){
          int cc;
          while((cc = recv(sock, buf, BUFLEN, 0))>0){	                       // ensure null-termination
               filesize+=cc;
               fwrite(buf, 1, cc, destfile);
 

          } 
          if(cc == SOCKET_ERROR){
               printf("Error: %d.\n",GetLastError());     //出错。其后必须关闭套接字sock。
               return 0;
          }
          else if(cc==0){
               cout<<"下载结束"<<endl;
               closesocket(sock); // 关闭监听套接字
			WSACleanup();	  // 卸载winsock library
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
         cout<<buf;                        // 显示所接收的字符串
    }                
    else if(cc == SOCKET_ERROR)                          // 出错。其后必须关闭套接字sock
         printf("Error: %d.\n",GetLastError());
     else if(cc == 0) {                             // 对方正常关闭
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
     sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
														  //返回：要监听套接字的描述符或INVALID_SOCKET
    memset(&sin, 0, sizeof(sin));						  // 从&sin开始的长度为sizeof(sin)的内存清0
    sin.sin_family = AF_INET;							  // 因特网地址簇
    sin.sin_addr.s_addr = inet_addr(host);                // 服务器IP地址(32位)
    sin.sin_port = htons((u_short)port);       
    int ret=connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // 连接到服务器，第二个参数指向存放服务器地址的结构，第三个参数为该结构的大小，返回值为0时表示无错误发生，返回SOCKET_ERROR表示出错，应用程序可通过WSAGetLastError()获取相应错误代码。
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
	WSAStartup(WSVERS, &wsadata);						  //加载winsock library。WSVERS为请求的版本，wsadata返回系统实际支持的最高版本
  
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);	  //创建套接字，参数：因特网协议簇(family)，流套接字，TCP协议
														  //返回：要监听套接字的描述符或INVALID_SOCKET
    memset(&sin, 0, sizeof(sin));						  // 从&sin开始的长度为sizeof(sin)的内存清0
    sin.sin_family = AF_INET;							  // 因特网地址簇
    sin.sin_addr.s_addr = inet_addr(host);                // 服务器IP地址(32位)
    sin.sin_port = htons((u_short)21);       
    int ret=connect(sock, (struct sockaddr *)&sin, sizeof(sin));  // 连接到服务器，第二个参数指向存放服务器地址的结构，第三个参数为该结构的大小，返回值为0时表示无错误发生，返回SOCKET_ERROR表示出错，应用程序可通过WSAGetLastError()获取相应错误代码。
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

     int port;                          //建立数据链接
     port=dataport(buf,host);
     datasock=dataconnet(port,host);

     sprintf(msg, "retr %s\r\n", src);
     cout<<msg;
     len=strlen(msg);
     sendlen=send(sock,msg,len+1,0);
     recvthread(sock);
     recvthread(sock);
     
     cout<<"开始下载文件...\n\r";
     saveFile(datasock,filename,des);

     sprintf(msg, "quit\r\n");
     cout<<msg;
     len=strlen(msg);
     sendlen=send(sock,msg,len+1,0);
     recvthread(sock);


     closesocket(sock);                             // 关闭监听套接字
     WSACleanup();                                  // 卸载winsock library
     
     return 0;									// 等待任意按键
}

