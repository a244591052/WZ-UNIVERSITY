#define _VC 
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include <conio.h>
#ifdef _VC
#pragma comment( lib, "Wsock32.lib" )
#endif

#include "makepack.h"
#include "log.h"
#include "time.h"

void download(struct sockaddr_in sour_addr,char buffer[]);
void upload(struct sockaddr_in sour_addr,char buffer[]);

SOCKET sock = INVALID_SOCKET;		//#define INVALID_SOCKET  (SOCKET)(~0)
char *desthost = "127.0.0.1";	//目的主机地址，默认为本机
int  filemode = TFTP_OCTET;			//发送模式设置为octet

void findIP(char *ip, int size)   
{  
    WORD v = MAKEWORD(1, 1);  
    WSADATA wsaData;  
    WSAStartup(v, &wsaData); // 加载套接字库  
  
    struct hostent *phostinfo = gethostbyname("");  
    char *p = inet_ntoa (* ((struct in_addr *)(*phostinfo->h_addr_list)) );                
    strncpy(ip, p, size - 1);  
    ip[size - 1] = '\0';  
    WSACleanup( );  
} 

int main(int argc, char* argv[])
{
	char ip[20] = {0};  
    findIP(ip, sizeof(ip));  
    //printf("%s\n", ip);  
	printf("---------------------------------------------------------\n\n\
	   TFTP_Server  (IP: %s)\n\n\
---------------------------------------------------------\n\n",ip);

    int ret = 0; 
	sockaddr_in addr;
	//初始化Winsock库
	WSADATA stWSAData;
	if(WSAStartup(WSA_VERSION, &stWSAData)!=0)
	{	
		printf("启动Socket失败 \n");
		exit(0);
	}

    //创建套接字
    sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock==INVALID_SOCKET)
	{
		printf("创建socket失败\n");
		exit(0);
	}
	
	//初始化本地主机地址信息
	addr.sin_family = AF_INET;
	addr.sin_port = htons(69);  // TFTP服务器默认端口号69
	addr.sin_addr.s_addr  = INADDR_ANY;
	
	//绑定套接字
	if(bind(sock,(struct sockaddr *)&addr,sizeof(addr))!=0)
	{
		printf("不能绑定套接字\n");
		exit(0);
	}

	//初始化远程地址信息
	struct sockaddr_in sour_addr;
	sour_addr.sin_family = AF_INET;
	sour_addr.sin_port = htons(INADDR_ANY);
	sour_addr.sin_addr.s_addr = inet_addr(desthost); // 远程IP地址

	while(true) {
		char recv_buffer[1024] = {0};	// 接收的数据缓冲区			
		int sour_len = 0;
		char *fi=NULL;
		fd_set  fdr;
		FD_ZERO(&fdr);
		FD_SET(sock, &fdr);

		//确定一个或多个套接口的状态,返回处于就绪状态并且已经包含在fd_set结构中的描述字总数
		//如果超时则返回0；否则的话，返回SOCKET_ERROR错误	
		ret = select(sock, &fdr, NULL,NULL, NULL);
				
		if(SOCKET_ERROR==ret)
		{				
			printf("Socket 错误\n");
			return 0;
		}
		else
		{
			if(FD_ISSET(sock,&fdr))
			{
				//接收数据
				sour_len = sizeof(sockaddr);
				ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);
				fi=&recv_buffer[2];

				//读请求
				if(recv_buffer[1]==TFTP_RRQ)
				{	

					printf("收到来自%s的下载文件请求\n",inet_ntoa(sour_addr.sin_addr));	
					desthost = inet_ntoa(sour_addr.sin_addr);
					record(3,&sour_addr,fi);
					download(sour_addr,recv_buffer);
				}

				//写请求
				 if(TFTP_WRQ==recv_buffer[1])
				 {	

					printf("收到来自%s的上传文件请求\n",inet_ntoa(sour_addr.sin_addr));
					desthost = inet_ntoa(sour_addr.sin_addr);
					record(1,&sour_addr,fi);
					upload(sour_addr,recv_buffer);
				}
			}
		}
	}
	return 0;
}

//下载函数
void download(struct sockaddr_in sour_addr,char buffer[])
{
	char send_buffer[1024] = {0};
	char recv_buffer[1024] = {0};
	char data_buffer[1024] = {0};
	sockaddr_in dest_addr;

	fd_set  fdr;

	int retry = 0;
	int sour_len = 0;
	int ret = 0;
	int len = 0 ;
	int flen = 0;

	struct  timeval timeout = {10,0};
	int stat = TFTP_WSTAT_NEXTACK;

	unsigned short lastack= 0;
	unsigned short blocknum = 0;
	size_t rlen = 0;
	FILE *file;
	char filename[256];

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = sour_addr.sin_port;
	dest_addr.sin_addr.s_addr = inet_addr(desthost);
	//获取文件名
	strcpy(filename,buffer+2);					
	//打开文件
	if((file=fopen(filename,"rb"))==NULL)
	{
		printf("没有找到名为%s的文件\n",filename);		
		record(5,&sour_addr,filename);
		return;
	}

	//读文件
	rlen = fread(data_buffer,1,512,file);		
	flen = flen + rlen;

	if(rlen<512 && feof(file))
	{
		stat = TFTP_WSTAT_LASTACK;
	}
	else{
		if(ferror(file))
		{
			fclose(file);
			record(5,&sour_addr,filename);
			return;
		}
	}
	blocknum++;

	//填充DATA包
	len = makedata(blocknum,data_buffer,rlen,send_buffer,sizeof(send_buffer));
	//发送数据
	ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
	printf("%d byte has sended\n",flen);

	while(true)
	{
		FD_ZERO(&fdr);
		FD_SET(sock, &fdr);
		ret = select(sock, &fdr, NULL,NULL, &timeout);

		if(SOCKET_ERROR==ret)
		{
			printf("Socket错误\n");
			fclose(file);
			record(5,&sour_addr,filename);
			return;
		}
		else{
			if(0==ret)
			{
				//若超过最大重传次数
				if(MAX_RETRY==retry)
				{
					printf("超时\n");
					fclose(file);
					
					//发送error包
					len = makeerr(Time_out,send_buffer);
					sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
					record(5,&sour_addr,filename);
					return;
				}
				//重传
				sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
				retry++;  //重传次数加一
			}
			else{
				retry = 0;
				//接收数据
				sour_len = sizeof(sockaddr);
				ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);
				//如果收到的是ACK包
				if(TFTP_ACK==recv_buffer[1])   
				{
					lastack = MAKEWORD(recv_buffer[2],recv_buffer[3]); //得到块号
					switch(stat){
						case TFTP_WSTAT_NEXTACK:
							//如果块号正确
							if(blocknum==lastack)  
							{
								//读文件
								rlen = fread(data_buffer,1,512,file);
								flen = flen + rlen;
								//如果是最后一个数据块
								if(rlen<512 && feof(file))
								{
									stat = TFTP_WSTAT_LASTACK;
								}
								//文件未读到最后
								else
								{
									if(ferror(file))
									{
										printf("读取文件错误\n");
										fclose(file);
										//发送error包
										len = makeerr(Read_file_Error,send_buffer);
										sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));										
										record(5,&sour_addr,filename);
										return;
									}
								}
								blocknum++;
								if(blocknum > 65535)
									blocknum = 0;
								//发送数据包
								len = makedata(blocknum,data_buffer,rlen,send_buffer,sizeof(send_buffer));
								sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
								printf("已发送 %d 字节\n",flen);
							}
							else
							{
								//重传上一次的包
								printf("ACK块号错误.\n");
								sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
							}
							break;
						case TFTP_WSTAT_LASTACK:
							if(lastack==blocknum)
							{
								printf("*****传输结束,共发送 %d 字节*****\n",flen);
								record(4,&sour_addr,filename);
								return;
							}
							else
							{
								//重新发送最后一个包
								printf("ACK块号错误.\n");
								sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));								
							}
							break;
						default	:
							break;
					}
				}
			}
		}
	}
}

//上传函数
void upload(struct sockaddr_in sour_addr,char buffer[])
{
	char send_buffer[1024] = {0};
	char recv_buffer[1024] = {0};
	sockaddr_in dest_addr;

	struct  timeval timeout = {10,0};
	int sour_len = 0;
	int ret = 0;
	int len = 0;
	int flen = 0;

	fd_set  fdr;

	unsigned short lastdata = 0;
	unsigned short blocknum = 0;

	FILE *file;
	char filename[256];	
	//获取文件名
	strcpy(filename,buffer+2);	

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = sour_addr.sin_port;
	dest_addr.sin_addr.s_addr = inet_addr(desthost);//
	//如果本地存在同名文件
	if((file=fopen(filename,"rb"))!=NULL)
	{
		//发送一个error包,报告存在同名文件
		printf("***存在同名文件***");
		len = makeerr(File_already_exists,send_buffer);
		ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
		record(5,&sour_addr,filename);
		return; 
	}
	//建立文件
	if((file=fopen(filename,"w+b"))==NULL)
	{
		//如果失败,发送error包
		printf("创建文件失败\n");
		len = makeerr(Cannot_create_file,send_buffer);
		ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
		record(5,&sour_addr,filename);
		return;
	}
	//发送ACK
	len = makeack(blocknum,send_buffer,sizeof(send_buffer));
	ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
	blocknum++;	
	while(true){
		FD_ZERO(&fdr);
		FD_SET(sock, &fdr);
		ret = select(sock, &fdr, NULL,NULL, &timeout);
		
		if(SOCKET_ERROR==ret)
		{		
			printf("Socket 错误\n");
			fclose(file);
			record(5,&sour_addr,filename);
			return;
		}
		else
		{ 
			if(0==ret)
			{
				printf("超时\n");
				fclose(file);
				record(5,&sour_addr,filename);
				return;
			}
			else
			{
				if (FD_ISSET(sock,&fdr))
				{
					//接收数据包
					sour_len = sizeof(sockaddr);
					ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);
					//如果是数据包
					if(TFTP_DATA==recv_buffer[1])
					{				
						lastdata = MAKEWORD(recv_buffer[3],recv_buffer[2]); //块号
						//如果块号正确
						if(lastdata == blocknum)
						{
							//发送ACK包
							len = makeack(blocknum,send_buffer,sizeof(send_buffer));
							sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
							blocknum++;
							if(blocknum > 65535)
								blocknum = 0;
							//最后一包
							if(ret < TFTP_NOTEND_DATALEN)
							{
								//写入文件
								fwrite(&recv_buffer[4],1,ret-4,file);
								flen = flen + ret -4;
								printf("*****传输结束,共收到 %d 字节*****\n",flen);
								fclose(file);
								record(2,&sour_addr,filename);
								return;
							}
							else
							{
								fwrite(&recv_buffer[4],1,512,file);
								flen = flen + 512;
								printf("已收到 %d 字节\n",flen);
							}
						}
						else
						{
							//重新发送ACK包
							printf("数据包块号错误.\n");
							sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
						}
					}
				}
			}
		}
	}
}