#include "define.h"

typedef struct __tftp_hdr 
{
	unsigned short opcode;
	union
	{
		unsigned short blocknum;
		unsigned short errnum;
		char stuff[1];
	} th_u;
	char data[0];
}TFTP_PACKAGE,*PTFTP_PACKAGE;

int  makeack(unsigned short num,char *buffer,int size );
int  makereq(char type,int mode,char *filename,char *buffer,int size);
int  makedata(unsigned short num,char *data,int datasize,char *buffer,int bufsize);

//ACK包填充
int makeack(unsigned short num,char *buffer,int size )
{
	PTFTP_PACKAGE tps;
	tps = (PTFTP_PACKAGE)buffer;
	//optcode
	int pos = 4;
	tps->opcode = htons(TFTP_ACK);
	tps->th_u.blocknum = htons(num);



	/*
	int pos = 0;
	buffer[pos] = 0;
	pos++;
	buffer[pos] = TFTP_ACK;    //opcode=04
	pos++;

	//封装块号
	buffer[pos] = (char)(num >> 8);//块号2个字节
	pos++;
	buffer[pos] = (char)num;
	pos++;*/
	return pos;
}
//填充请求包
int makereq(char type,char *mode,char *filename,char *buffer,int size)
{
	PTFTP_PACKAGE ptk;
	ptk = (PTFTP_PACKAGE)buffer;
	//optcode
	int pos = 2;
	ptk->opcode = htons(type);

	char *pt;
	pt=ptk->th_u.stuff;
	sprintf_s(pt,strlen(filename)+1,"%s",filename);
	pt += strlen(filename)+1;
	pos += strlen(filename)+1;
	sprintf_s(pt,6,"%s","octet");
	pos += 6;
	/*
	int pos = 0;
	unsigned int i = 0;

	buffer[pos] = 0;
	pos++;
	buffer[pos] = type;// Opcode = 01(RRQ) or 02(WRQ)
	pos++;

	//filename
	for(i=0;i<strlen(filename);i++){  // &buffer[3] = Filename
		buffer[pos] = filename[i];
		pos++;
	}
	buffer[pos] = 0;   //0
	pos++;

	//mode
	for(i=0;i<strlen(mode);i++){  //mode
		buffer[pos] = mode[i];
		pos++;
	}
	buffer[pos] = 0;//0

	pos++;*/
	return pos;   //返回请求包长度
}

//Data包填充
int makedata(unsigned short num,char *data,int datasize,char *buffer,int bufsize)
{
	PTFTP_PACKAGE tpr;
	tpr = (PTFTP_PACKAGE)buffer;
	//optcode
	int pos = 4;
	tpr->opcode = htons(TFTP_DATA);
	tpr->th_u.blocknum = htons(num);

	memcpy(tpr->data,data,datasize); //data
	pos = pos + datasize;
	//char *pt;
	//pt=tpr->data;
	//sprintf_s(pt,strlen(data)+1,"%s",data);
	//pos += strlen(data);




	/*
	int pos = 0;
	buffer[pos] = 0;
	pos++;
	buffer[pos] = TFTP_DATA;//opcode=3
	pos++;
	buffer[pos] = (char)(num>>8);//块号
	pos++;
	buffer[pos] = (char)num;
	pos++;
	memcpy(&buffer[pos],data,datasize); //data
	pos = pos + datasize;*/
	return pos;
}
