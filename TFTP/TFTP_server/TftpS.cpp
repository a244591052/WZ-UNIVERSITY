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
char *desthost = "127.0.0.1";	//Ŀ��������ַ��Ĭ��Ϊ����
int  filemode = TFTP_OCTET;			//����ģʽ����Ϊoctet

void findIP(char *ip, int size)   
{  
    WORD v = MAKEWORD(1, 1);  
    WSADATA wsaData;  
    WSAStartup(v, &wsaData); // �����׽��ֿ�  
  
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
	//��ʼ��Winsock��
	WSADATA stWSAData;
	if(WSAStartup(WSA_VERSION, &stWSAData)!=0)
	{	
		printf("����Socketʧ�� \n");
		exit(0);
	}

    //�����׽���
    sock = socket(AF_INET,SOCK_DGRAM,0);
	if(sock==INVALID_SOCKET)
	{
		printf("����socketʧ��\n");
		exit(0);
	}
	
	//��ʼ������������ַ��Ϣ
	addr.sin_family = AF_INET;
	addr.sin_port = htons(69);  // TFTP������Ĭ�϶˿ں�69
	addr.sin_addr.s_addr  = INADDR_ANY;
	
	//���׽���
	if(bind(sock,(struct sockaddr *)&addr,sizeof(addr))!=0)
	{
		printf("���ܰ��׽���\n");
		exit(0);
	}

	//��ʼ��Զ�̵�ַ��Ϣ
	struct sockaddr_in sour_addr;
	sour_addr.sin_family = AF_INET;
	sour_addr.sin_port = htons(INADDR_ANY);
	sour_addr.sin_addr.s_addr = inet_addr(desthost); // Զ��IP��ַ

	while(true) {
		char recv_buffer[1024] = {0};	// ���յ����ݻ�����			
		int sour_len = 0;
		char *fi=NULL;
		fd_set  fdr;
		FD_ZERO(&fdr);
		FD_SET(sock, &fdr);

		//ȷ��һ�������׽ӿڵ�״̬,���ش��ھ���״̬�����Ѿ�������fd_set�ṹ�е�����������
		//�����ʱ�򷵻�0������Ļ�������SOCKET_ERROR����	
		ret = select(sock, &fdr, NULL,NULL, NULL);
				
		if(SOCKET_ERROR==ret)
		{				
			printf("Socket ����\n");
			return 0;
		}
		else
		{
			if(FD_ISSET(sock,&fdr))
			{
				//��������
				sour_len = sizeof(sockaddr);
				ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);
				fi=&recv_buffer[2];

				//������
				if(recv_buffer[1]==TFTP_RRQ)
				{	

					printf("�յ�����%s�������ļ�����\n",inet_ntoa(sour_addr.sin_addr));	
					desthost = inet_ntoa(sour_addr.sin_addr);
					record(3,&sour_addr,fi);
					download(sour_addr,recv_buffer);
				}

				//д����
				 if(TFTP_WRQ==recv_buffer[1])
				 {	

					printf("�յ�����%s���ϴ��ļ�����\n",inet_ntoa(sour_addr.sin_addr));
					desthost = inet_ntoa(sour_addr.sin_addr);
					record(1,&sour_addr,fi);
					upload(sour_addr,recv_buffer);
				}
			}
		}
	}
	return 0;
}

//���غ���
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
	//��ȡ�ļ���
	strcpy(filename,buffer+2);					
	//���ļ�
	if((file=fopen(filename,"rb"))==NULL)
	{
		printf("û���ҵ���Ϊ%s���ļ�\n",filename);		
		record(5,&sour_addr,filename);
		return;
	}

	//���ļ�
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

	//���DATA��
	len = makedata(blocknum,data_buffer,rlen,send_buffer,sizeof(send_buffer));
	//��������
	ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
	printf("%d byte has sended\n",flen);

	while(true)
	{
		FD_ZERO(&fdr);
		FD_SET(sock, &fdr);
		ret = select(sock, &fdr, NULL,NULL, &timeout);

		if(SOCKET_ERROR==ret)
		{
			printf("Socket����\n");
			fclose(file);
			record(5,&sour_addr,filename);
			return;
		}
		else{
			if(0==ret)
			{
				//����������ش�����
				if(MAX_RETRY==retry)
				{
					printf("��ʱ\n");
					fclose(file);
					
					//����error��
					len = makeerr(Time_out,send_buffer);
					sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
					record(5,&sour_addr,filename);
					return;
				}
				//�ش�
				sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
				retry++;  //�ش�������һ
			}
			else{
				retry = 0;
				//��������
				sour_len = sizeof(sockaddr);
				ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);
				//����յ�����ACK��
				if(TFTP_ACK==recv_buffer[1])   
				{
					lastack = MAKEWORD(recv_buffer[2],recv_buffer[3]); //�õ����
					switch(stat){
						case TFTP_WSTAT_NEXTACK:
							//��������ȷ
							if(blocknum==lastack)  
							{
								//���ļ�
								rlen = fread(data_buffer,1,512,file);
								flen = flen + rlen;
								//��������һ�����ݿ�
								if(rlen<512 && feof(file))
								{
									stat = TFTP_WSTAT_LASTACK;
								}
								//�ļ�δ�������
								else
								{
									if(ferror(file))
									{
										printf("��ȡ�ļ�����\n");
										fclose(file);
										//����error��
										len = makeerr(Read_file_Error,send_buffer);
										sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));										
										record(5,&sour_addr,filename);
										return;
									}
								}
								blocknum++;
								if(blocknum > 65535)
									blocknum = 0;
								//�������ݰ�
								len = makedata(blocknum,data_buffer,rlen,send_buffer,sizeof(send_buffer));
								sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
								printf("�ѷ��� %d �ֽ�\n",flen);
							}
							else
							{
								//�ش���һ�εİ�
								printf("ACK��Ŵ���.\n");
								sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
							}
							break;
						case TFTP_WSTAT_LASTACK:
							if(lastack==blocknum)
							{
								printf("*****�������,������ %d �ֽ�*****\n",flen);
								record(4,&sour_addr,filename);
								return;
							}
							else
							{
								//���·������һ����
								printf("ACK��Ŵ���.\n");
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

//�ϴ�����
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
	//��ȡ�ļ���
	strcpy(filename,buffer+2);	

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = sour_addr.sin_port;
	dest_addr.sin_addr.s_addr = inet_addr(desthost);//
	//������ش���ͬ���ļ�
	if((file=fopen(filename,"rb"))!=NULL)
	{
		//����һ��error��,�������ͬ���ļ�
		printf("***����ͬ���ļ�***");
		len = makeerr(File_already_exists,send_buffer);
		ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
		record(5,&sour_addr,filename);
		return; 
	}
	//�����ļ�
	if((file=fopen(filename,"w+b"))==NULL)
	{
		//���ʧ��,����error��
		printf("�����ļ�ʧ��\n");
		len = makeerr(Cannot_create_file,send_buffer);
		ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
		record(5,&sour_addr,filename);
		return;
	}
	//����ACK
	len = makeack(blocknum,send_buffer,sizeof(send_buffer));
	ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
	blocknum++;	
	while(true){
		FD_ZERO(&fdr);
		FD_SET(sock, &fdr);
		ret = select(sock, &fdr, NULL,NULL, &timeout);
		
		if(SOCKET_ERROR==ret)
		{		
			printf("Socket ����\n");
			fclose(file);
			record(5,&sour_addr,filename);
			return;
		}
		else
		{ 
			if(0==ret)
			{
				printf("��ʱ\n");
				fclose(file);
				record(5,&sour_addr,filename);
				return;
			}
			else
			{
				if (FD_ISSET(sock,&fdr))
				{
					//�������ݰ�
					sour_len = sizeof(sockaddr);
					ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);
					//��������ݰ�
					if(TFTP_DATA==recv_buffer[1])
					{				
						lastdata = MAKEWORD(recv_buffer[3],recv_buffer[2]); //���
						//��������ȷ
						if(lastdata == blocknum)
						{
							//����ACK��
							len = makeack(blocknum,send_buffer,sizeof(send_buffer));
							sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
							blocknum++;
							if(blocknum > 65535)
								blocknum = 0;
							//���һ��
							if(ret < TFTP_NOTEND_DATALEN)
							{
								//д���ļ�
								fwrite(&recv_buffer[4],1,ret-4,file);
								flen = flen + ret -4;
								printf("*****�������,���յ� %d �ֽ�*****\n",flen);
								fclose(file);
								record(2,&sour_addr,filename);
								return;
							}
							else
							{
								fwrite(&recv_buffer[4],1,512,file);
								flen = flen + 512;
								printf("���յ� %d �ֽ�\n",flen);
							}
						}
						else
						{
							//���·���ACK��
							printf("���ݰ���Ŵ���.\n");
							sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
						}
					}
				}
			}
		}
	}
}