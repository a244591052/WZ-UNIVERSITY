#include "time.h"
#include "stdlib.h"
#include "stdio.h"

char log[100];     //��־
char datetime[20]; //��¼ʱ�����
int  timeout=2,retran=3;  //����������

void record(int a,struct sockaddr_in *sin,char *file)
{
	char tem[60];
	time_t t=time(0);   //��ʼ������ʱ��
	strftime(datetime,sizeof(datetime),"%y/%m/%d %X",localtime(&t));//��ʱ���ʽ��
	strcat(log,datetime);//��ʱ��д���¼

	//���ַ�����ʽ��
	ZeroMemory(&tem,sizeof(tem));
	if(a==1)
		sprintf(tem," �յ����� %s �ϴ��ļ� %s ������\n",inet_ntoa(sin->sin_addr),file);
	if(a==2)
		sprintf(tem," %s �ϴ��ļ� %s ��ϡ�\n",inet_ntoa(sin->sin_addr),file);
	if(a==3)
		sprintf(tem," �յ����� %s �����ļ� %s������\n",inet_ntoa(sin->sin_addr),file);
	if(a==4)
		sprintf(tem," %s �����ļ� %s ��ϡ�\n",inet_ntoa(sin->sin_addr),file);
	if(a==5)
		sprintf(tem," ���ֳ��������жϡ�\n",inet_ntoa(sin->sin_addr),file);
	//��������Ϣд���¼
	strcat(log,tem);
	
	FILE *write;
	if((write=fopen("log.txt","a+"))==NULL)
		printf("�򿪼�¼�ļ�ʧ��\n");
	//����¼д���ļ�
	fwrite(&log,strlen(log),1,write);
	fclose(write);
	ZeroMemory(&log,sizeof(log));
}