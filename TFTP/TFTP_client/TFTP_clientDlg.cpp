// TFTP_clientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TFTP_client.h"
#include "TFTP_clientDlg.h"
#include "string.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#include "define.h"
#include "makepack.h"



SOCKET sock = INVALID_SOCKET;		
char *desthost = "127.0.0.1";	//目的主机地址，默认为本机
char *filemode = "octet";			//发送模式为octet

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTFTP_clientDlg dialog

CTFTP_clientDlg::CTFTP_clientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTFTP_clientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTFTP_clientDlg)
	m_target_name = _T("");
	m_local_name = _T("");
	m_dest_ip = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTFTP_clientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTFTP_clientDlg)
	DDX_Text(pDX, IDC_target_name, m_target_name);
	DDX_Text(pDX, IDC_local_name, m_local_name);
	DDX_Text(pDX, IDC_IP, m_dest_ip);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTFTP_clientDlg, CDialog)
	//{{AFX_MSG_MAP(CTFTP_clientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_download, Ondownload)
	ON_BN_CLICKED(IDC_upload, Onupload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTFTP_clientDlg message handlers

BOOL CTFTP_clientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	sockaddr_in addr;

	//初始化Winsock库
	WSADATA stWSAData;
	if(WSAStartup(WSA_VERSION, &stWSAData)!=0)
	{	
		//printf("Can't start Socket \n");
		AfxMessageBox("Can't start Socket \n");
		exit(0);
	}
    
    sock = socket(AF_INET,SOCK_DGRAM,0);	
	if(sock==INVALID_SOCKET)
	{
		AfxMessageBox("创建SOCKET失败\n");
		exit(0);
	}

	/*初始化地址信息
	struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
	};  
	用做bind、connect、recvfrom、sendto等函数的参数，指明地址信息*/

	addr.sin_family = AF_INET;
	//将整型变量从主机字节顺序转变成网络字节顺序
	addr.sin_port = htons(INADDR_ANY);
	addr.sin_addr.s_addr  = INADDR_ANY;
	//将socket绑定在某一端口
	if(bind(sock,(struct sockaddr *)&addr,sizeof(addr))!=0)
	{ 
		AfxMessageBox("绑定失败\n");
		exit(0);
	}
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTFTP_clientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTFTP_clientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//背景图片
		CPaintDC dc(this);
		CBitmap   bitmap;
		bitmap.LoadBitmap(IDB_BITMAP3);    //这个IDB_BITMAP1要自己添加
		CBrush   brush;
		brush.CreatePatternBrush(&bitmap);
		CBrush*   pOldBrush   =   dc.SelectObject(&brush);
		dc.Rectangle(0,0,350,300);   // 这些参数可以调整图片添加位置和大小
		dc.SelectObject(pOldBrush);
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTFTP_clientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void ErrorHandle(char error_id)
{
	switch(error_id){// recv_buffer[3]中存放错误信息 
		case Not_defined:
			AfxMessageBox("Not defined");//未定义
			return;
		case File_not_found:
			AfxMessageBox("File not found");//没发现此文件 
			return;
		case Access_violation:
			AfxMessageBox("Access violation");// 违法进入 
			return;
		case Disk_full:
			AfxMessageBox("Disk full");//硬盘已满 
			return;
		case Illegal_TFTP_operation:
			AfxMessageBox("Illegal TFTP operation");//非法操作 
			return;
		case Unknown_port:
			AfxMessageBox("Unknown port");//未知端口 
			return;
		case File_already_exists:
			AfxMessageBox("file already exists");//文件已存在 
			return;
		case No_such_user:
			AfxMessageBox("No such user");//没有此用户 
			return;
		case Time_out:
			AfxMessageBox("Time out");//超时 
			return;
		case Read_file_Error:
			AfxMessageBox("Read file error");//读文件错误 
			return;
		case Cannot_create_file:
			AfxMessageBox("Can't create file");//无法创建文件 
			return;
		default :
			AfxMessageBox("unknown error");//无法预料的错误 
			return;
	}


}

void CTFTP_clientDlg::Ondownload() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//获取服务器IP和文件名
	desthost = m_dest_ip.GetBuffer(0);
	//AfxMessageBox(desthost);
	char *file_name = m_target_name.GetBuffer(0);
	//MessageBox(file_name,NULL,MB_OK);
	char send_buffer[1024] = {0};
	char recv_buffer[1024] = {0};
	sockaddr_in dest_addr;
	sockaddr_in sour_addr;

	struct  timeval timeout = {5,0};
	int sour_len = 0;
	int ret = 0;
	int len = 0 ;
	int retry = 0;
	int flen = 0;
	int stat = 0;
	unsigned short lastdata = 0;

	fd_set  fdr;	//定义一个套接字集合变量	
	FILE *file;	
	
	INT_PTR nRes;
	
	if((file=fopen(m_target_name,"rb"))!=NULL){  //已经存在同名文件
		nRes = MessageBox(_T("文件已存在，是否覆盖？"), _T("文件已存在"), MB_OKCANCEL | MB_ICONQUESTION); 
		if (IDCANCEL == nRes){
			fclose(file);
			return;
		}
		if (IDOK == nRes){
			fclose(file);
		}

	}
	if((file=fopen(m_target_name,"w+b"))==NULL){
		AfxMessageBox("创建文件失败\n");
		return;
	}
	//填充请求字段，返回请求包长度 
	len = makereq(TFTP_RRQ, filemode, file_name,send_buffer,sizeof(send_buffer));

	
	//设置传输配置
	dest_addr.sin_family =AF_INET;
	sour_addr.sin_family =AF_INET;
	dest_addr.sin_port = htons(69);
	dest_addr.sin_addr.s_addr = inet_addr(desthost);//远程IP地址 

	//发送请求
	ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
	
	while(true){
		//在调用select()之前，总是要清除套接字集合变量
		FD_ZERO(&fdr);

		//将套接字s添加到fdr
		FD_SET(sock, &fdr);
		
		//调用select()函数，并等待它的完成，这里只是检查s是否有数据可读
		ret = select(sock, &fdr, NULL,NULL, &timeout);

		//如果超时则返回0；如果有错，返回SOCKET_ERROR错误
		
		if(SOCKET_ERROR==ret){	
			AfxMessageBox("Socket error");
			fclose(file);
			return;
		}
		else{ 
			if(0==ret){   //超时重传 
				if(MAX_RETRY==retry){
					AfxMessageBox("Time Out");
					fclose(file);
					return;
				}
				sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
				retry++;
			}
			else{
				if (FD_ISSET(sock,&fdr)){// 查看sock是否被设置了，文件描述符是否在该集合中
					retry = 0;
                    
					//接收数据
					sour_len = sizeof(sockaddr);
					ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);

				   // 成功则返回接收到的字符数,失败返回-1.
					if(TFTP_ERROR==recv_buffer[1]){
						ErrorHandle(recv_buffer[3]);
					}

					//将服务器端的随机生成端口设置为目的端口
					if(0==stat){
						dest_addr.sin_port = sour_addr.sin_port ;
						stat = 1;
					}

					if(TFTP_DATA==recv_buffer[1]){//若是数据文件 
						lastdata = MAKEWORD(recv_buffer[2],recv_buffer[3]);
                        // makeword是将两个byte型合并成一个word型，一个在高8位，一个在低8位

						//发送ACK
						len = makeack(lastdata,send_buffer,sizeof(send_buffer));
						sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
						
						if(ret < TFTP_NOTEND_DATALEN){
							fwrite(&recv_buffer[4],1,ret-4,file);

							flen = flen + ret -4;
							fclose(file);

							//printf("*****传输结束,共收到 %d 字节*****\n",flen);
							AfxMessageBox("下载成功");
							return;
						}
						else{
							fwrite(&recv_buffer[4],1,512,file);//写文件 
							flen = flen + 512;
							//printf("已经收到 %d 字节\n",flen);
						}
					}
					
				}
			}
		}
	}
	
}

void CTFTP_clientDlg::Onupload() 
{
	// TODO: Add your control notification handler code here
	
	char send_buffer[1024] = {0};
	char recv_buffer[1024] = {0};
	char data_buffer[1024] = {0};
	sockaddr_in dest_addr;
	sockaddr_in sour_addr;

	UpdateData(TRUE);
	desthost = m_dest_ip.GetBuffer(0);
	//AfxMessageBox(m_target_name);
	char *remote_name = m_target_name.GetBuffer(0);
	char *local_name = m_local_name.GetBuffer(0);

	int retry = 0;
	int sour_len = 0;
	int ret = 0;
	int len = 0;
	int flen = 0;

	fd_set  fdr;
	
	struct  timeval timeout = {5,0};
	int stat = TFTP_WSTAT_FIRSTACK;

	unsigned short lastack = 0;
	unsigned short blocknum = 0;

	FILE *file;
	size_t rlen = 0;


	if((file=fopen(local_name,"rb"))==NULL){
		AfxMessageBox("没有找到本地文件");
		return;
	}

	//填充请求包
	len = makereq(TFTP_WRQ,filemode,remote_name,send_buffer,sizeof(send_buffer));
	
	/*初始化地址信息*/ 
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(69);
	dest_addr.sin_addr.s_addr = inet_addr(desthost);

	//发送请求包
	ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));

	if((file=fopen(local_name,"rb"))==NULL){
		AfxMessageBox("打开文件失败");
		return;
	}

	while(true){
		FD_ZERO(&fdr);
		FD_SET(sock, &fdr);
		ret = select(sock, &fdr, NULL,NULL, &timeout);

		if(SOCKET_ERROR==ret){
			AfxMessageBox("Socket 错误");
			fclose(file);
			return;
		}
		else{
			if(0==ret){//超时重传 
				if(MAX_RETRY==retry){
					AfxMessageBox("超时");
					fclose(file);
					return;
				}
				//重传
				sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
				retry++;
			}
			else{//正常接收 
				retry = 0;
				sour_len = sizeof(sockaddr);
				ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);
				
				if(TFTP_ERROR==recv_buffer[1]){
					ErrorHandle(recv_buffer[3]);
					}
				if(TFTP_ACK==recv_buffer[1]){
					lastack = MAKEWORD(recv_buffer[3],recv_buffer[2]);//当前的ack 

					switch(stat){//stat初值为0 
						case TFTP_WSTAT_FIRSTACK:
							if(0==lastack){
								stat = TFTP_WSTAT_NEXTACK;

								//将目标端口换至服务器随机端口
								dest_addr.sin_port = sour_addr.sin_port;
				
								rlen = fread(data_buffer,1,512,file);
								flen = flen + rlen;
								if(rlen<512 && feof(file)){ // 大小小于512字节且检测到文件结束符
									stat = TFTP_WSTAT_LASTACK;
								}
								else{
									if(ferror(file)){
										AfxMessageBox("读取文件失败");
										fclose(file);
										return;
									}
								}
								blocknum++;//当前应该接受的块号 

								if(blocknum > 65535)
									blocknum = 0;
								
								len = makedata(blocknum,data_buffer,rlen,send_buffer,sizeof(send_buffer));

								sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
								//printf("已发送 %d 字节\n",flen);
							}
							else{
								fclose(file);
								AfxMessageBox("ACK 块号错误");
								return;
							}
							break;
						case TFTP_WSTAT_NEXTACK:
							if(lastack==blocknum){
								rlen = fread(data_buffer,1,512,file);
								flen = flen + rlen;
								if(rlen<512 && feof(file)){
									stat = TFTP_WSTAT_LASTACK;
								}
								else{
									if(ferror(file)){
										AfxMessageBox("读入文件失败");
										fclose(file);
										return;
									}
								}
								blocknum++;
								if(blocknum > 65535)//越界 
									blocknum = 0;

								len = makedata(blocknum,data_buffer,rlen,send_buffer,sizeof(send_buffer));
								sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
								//printf("已发送 %d 字节\n",flen);
							}
							else{
								fclose(file);
								AfxMessageBox("ACK 块号错误");
								return;
							}
							break;
						case TFTP_WSTAT_LASTACK:
							if(lastack==blocknum){
								//printf("*****传输结束,共发送 %d 字节*****\n",flen);
								AfxMessageBox("上传完成");
								return;
							}
							else{
								fclose(file);
								AfxMessageBox("Error Ack Number");
								return;
							}
							break;
					}//end switch
				}
			}
		}
	}
	
}
