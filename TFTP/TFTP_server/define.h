#ifndef MAKEWORD		
#define MAKEWORD(l,h) ((WORD)(((BYTE)(l))|(((WORD)(BYTE)(h))<<8)))
#endif
#define WSA_MAJOR_VERSION 1
#define WSA_MINOR_VERSION 1
#define WSA_VERSION MAKEWORD(WSA_MAJOR_VERSION, WSA_MINOR_VERSION)

#define TFTP_OCTET			1
#define TFTP_WSTAT_FIRSTACK		0
#define TFTP_WSTAT_NEXTACK		1
#define TFTP_WSTAT_LASTACK		2

#define TFTP_RRQ   1   //读请求 
#define TFTP_WRQ   2   //写请求 
#define TFTP_DATA  3   //数据  
#define TFTP_ACK   4   //ACK   
#define TFTP_ERROR 5   //Error

#define MAX_RETRY		3             	//最大重复次数
#define TFTP_NOTEND_DATALEN		512+2+2	//数据块长度

//错误种类
#define Not_defined					0
#define File_not_found				1
#define Access_violation			2
#define Disk_full					3
#define Illegal_TFTP_operation		4
#define Unknown_port				5
#define File_already_exists			6
#define No_such_user				7
#define Time_out					8
#define Read_file_Error				9
#define Cannot_create_file			10