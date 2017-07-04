#include "time.h"
#include "stdlib.h"
#include "stdio.h"

char log[100];     //日志
char datetime[20]; //记录时间变量
int  timeout=2,retran=3;  //服务器参数

void record(int a,struct sockaddr_in *sin,char *file)
{
	char tem[60];
	time_t t=time(0);   //初始化日历时间
	strftime(datetime,sizeof(datetime),"%y/%m/%d %X",localtime(&t));//将时间格式化
	strcat(log,datetime);//将时间写入记录

	//将字符串格式化
	ZeroMemory(&tem,sizeof(tem));
	if(a==1)
		sprintf(tem," 收到来自 %s 上传文件 %s 的请求。\n",inet_ntoa(sin->sin_addr),file);
	if(a==2)
		sprintf(tem," %s 上传文件 %s 完毕。\n",inet_ntoa(sin->sin_addr),file);
	if(a==3)
		sprintf(tem," 收到来自 %s 下载文件 %s的请求。\n",inet_ntoa(sin->sin_addr),file);
	if(a==4)
		sprintf(tem," %s 下载文件 %s 完毕。\n",inet_ntoa(sin->sin_addr),file);
	if(a==5)
		sprintf(tem," 出现出错，操作中断。\n",inet_ntoa(sin->sin_addr),file);
	//将具体信息写入记录
	strcat(log,tem);
	
	FILE *write;
	if((write=fopen("log.txt","a+"))==NULL)
		printf("打开记录文件失败\n");
	//将记录写入文件
	fwrite(&log,strlen(log),1,write);
	fclose(write);
	ZeroMemory(&log,sizeof(log));
}