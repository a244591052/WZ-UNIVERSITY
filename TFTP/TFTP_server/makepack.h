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
int  makedata(unsigned short num,char *data,int datasize,char *buffer,int bufsize);
int	 makeerr(unsigned short num,char *buffer);

//ACK°üÌî³ä
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

	//·â×°¿éºÅ
	buffer[pos] = (char)(num >> 8);//¿éºÅ2¸ö×Ö½Ú
	pos++;
	buffer[pos] = (char)num;
	pos++;*/
	return pos;
}
//Data°üÌî³ä
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
	buffer[pos] = (char)(num>>8);//¿éºÅ
	pos++;
	buffer[pos] = (char)num;
	pos++;
	memcpy(&buffer[pos],data,datasize); //data
	pos = pos + datasize;*/
	return pos;
}

//ERROR°üÌî³ä
int makeerr(unsigned short num,char *buffer)
{
	PTFTP_PACKAGE tpe;
	tpe = (PTFTP_PACKAGE)buffer;
	//optcode
	int pos = 4;
	tpe->opcode = htons(TFTP_ERROR);
	tpe->th_u.errnum = htons(num);

	
	return pos;
}