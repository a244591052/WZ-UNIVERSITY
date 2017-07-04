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
char *desthost = "127.0.0.1";	//Ŀ��������ַ��Ĭ��Ϊ����
char *filemode = "octet";			//����ģʽΪoctet

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

	//��ʼ��Winsock��
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
		AfxMessageBox("����SOCKETʧ��\n");
		exit(0);
	}

	/*��ʼ����ַ��Ϣ
	struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
	};  
	����bind��connect��recvfrom��sendto�Ⱥ����Ĳ�����ָ����ַ��Ϣ*/

	addr.sin_family = AF_INET;
	//�����ͱ����������ֽ�˳��ת��������ֽ�˳��
	addr.sin_port = htons(INADDR_ANY);
	addr.sin_addr.s_addr  = INADDR_ANY;
	//��socket����ĳһ�˿�
	if(bind(sock,(struct sockaddr *)&addr,sizeof(addr))!=0)
	{ 
		AfxMessageBox("��ʧ��\n");
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
		//����ͼƬ
		CPaintDC dc(this);
		CBitmap   bitmap;
		bitmap.LoadBitmap(IDB_BITMAP3);    //���IDB_BITMAP1Ҫ�Լ����
		CBrush   brush;
		brush.CreatePatternBrush(&bitmap);
		CBrush*   pOldBrush   =   dc.SelectObject(&brush);
		dc.Rectangle(0,0,350,300);   // ��Щ�������Ե���ͼƬ���λ�úʹ�С
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
	switch(error_id){// recv_buffer[3]�д�Ŵ�����Ϣ 
		case Not_defined:
			AfxMessageBox("Not defined");//δ����
			return;
		case File_not_found:
			AfxMessageBox("File not found");//û���ִ��ļ� 
			return;
		case Access_violation:
			AfxMessageBox("Access violation");// Υ������ 
			return;
		case Disk_full:
			AfxMessageBox("Disk full");//Ӳ������ 
			return;
		case Illegal_TFTP_operation:
			AfxMessageBox("Illegal TFTP operation");//�Ƿ����� 
			return;
		case Unknown_port:
			AfxMessageBox("Unknown port");//δ֪�˿� 
			return;
		case File_already_exists:
			AfxMessageBox("file already exists");//�ļ��Ѵ��� 
			return;
		case No_such_user:
			AfxMessageBox("No such user");//û�д��û� 
			return;
		case Time_out:
			AfxMessageBox("Time out");//��ʱ 
			return;
		case Read_file_Error:
			AfxMessageBox("Read file error");//���ļ����� 
			return;
		case Cannot_create_file:
			AfxMessageBox("Can't create file");//�޷������ļ� 
			return;
		default :
			AfxMessageBox("unknown error");//�޷�Ԥ�ϵĴ��� 
			return;
	}


}

void CTFTP_clientDlg::Ondownload() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//��ȡ������IP���ļ���
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

	fd_set  fdr;	//����һ���׽��ּ��ϱ���	
	FILE *file;	
	
	INT_PTR nRes;
	
	if((file=fopen(m_target_name,"rb"))!=NULL){  //�Ѿ�����ͬ���ļ�
		nRes = MessageBox(_T("�ļ��Ѵ��ڣ��Ƿ񸲸ǣ�"), _T("�ļ��Ѵ���"), MB_OKCANCEL | MB_ICONQUESTION); 
		if (IDCANCEL == nRes){
			fclose(file);
			return;
		}
		if (IDOK == nRes){
			fclose(file);
		}

	}
	if((file=fopen(m_target_name,"w+b"))==NULL){
		AfxMessageBox("�����ļ�ʧ��\n");
		return;
	}
	//��������ֶΣ�������������� 
	len = makereq(TFTP_RRQ, filemode, file_name,send_buffer,sizeof(send_buffer));

	
	//���ô�������
	dest_addr.sin_family =AF_INET;
	sour_addr.sin_family =AF_INET;
	dest_addr.sin_port = htons(69);
	dest_addr.sin_addr.s_addr = inet_addr(desthost);//Զ��IP��ַ 

	//��������
	ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
	
	while(true){
		//�ڵ���select()֮ǰ������Ҫ����׽��ּ��ϱ���
		FD_ZERO(&fdr);

		//���׽���s��ӵ�fdr
		FD_SET(sock, &fdr);
		
		//����select()���������ȴ�������ɣ�����ֻ�Ǽ��s�Ƿ������ݿɶ�
		ret = select(sock, &fdr, NULL,NULL, &timeout);

		//�����ʱ�򷵻�0������д�����SOCKET_ERROR����
		
		if(SOCKET_ERROR==ret){	
			AfxMessageBox("Socket error");
			fclose(file);
			return;
		}
		else{ 
			if(0==ret){   //��ʱ�ش� 
				if(MAX_RETRY==retry){
					AfxMessageBox("Time Out");
					fclose(file);
					return;
				}
				sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
				retry++;
			}
			else{
				if (FD_ISSET(sock,&fdr)){// �鿴sock�Ƿ������ˣ��ļ��������Ƿ��ڸü�����
					retry = 0;
                    
					//��������
					sour_len = sizeof(sockaddr);
					ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);

				   // �ɹ��򷵻ؽ��յ����ַ���,ʧ�ܷ���-1.
					if(TFTP_ERROR==recv_buffer[1]){
						ErrorHandle(recv_buffer[3]);
					}

					//���������˵�������ɶ˿�����ΪĿ�Ķ˿�
					if(0==stat){
						dest_addr.sin_port = sour_addr.sin_port ;
						stat = 1;
					}

					if(TFTP_DATA==recv_buffer[1]){//���������ļ� 
						lastdata = MAKEWORD(recv_buffer[2],recv_buffer[3]);
                        // makeword�ǽ�����byte�ͺϲ���һ��word�ͣ�һ���ڸ�8λ��һ���ڵ�8λ

						//����ACK
						len = makeack(lastdata,send_buffer,sizeof(send_buffer));
						sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
						
						if(ret < TFTP_NOTEND_DATALEN){
							fwrite(&recv_buffer[4],1,ret-4,file);

							flen = flen + ret -4;
							fclose(file);

							//printf("*****�������,���յ� %d �ֽ�*****\n",flen);
							AfxMessageBox("���سɹ�");
							return;
						}
						else{
							fwrite(&recv_buffer[4],1,512,file);//д�ļ� 
							flen = flen + 512;
							//printf("�Ѿ��յ� %d �ֽ�\n",flen);
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
		AfxMessageBox("û���ҵ������ļ�");
		return;
	}

	//��������
	len = makereq(TFTP_WRQ,filemode,remote_name,send_buffer,sizeof(send_buffer));
	
	/*��ʼ����ַ��Ϣ*/ 
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(69);
	dest_addr.sin_addr.s_addr = inet_addr(desthost);

	//���������
	ret = sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));

	if((file=fopen(local_name,"rb"))==NULL){
		AfxMessageBox("���ļ�ʧ��");
		return;
	}

	while(true){
		FD_ZERO(&fdr);
		FD_SET(sock, &fdr);
		ret = select(sock, &fdr, NULL,NULL, &timeout);

		if(SOCKET_ERROR==ret){
			AfxMessageBox("Socket ����");
			fclose(file);
			return;
		}
		else{
			if(0==ret){//��ʱ�ش� 
				if(MAX_RETRY==retry){
					AfxMessageBox("��ʱ");
					fclose(file);
					return;
				}
				//�ش�
				sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
				retry++;
			}
			else{//�������� 
				retry = 0;
				sour_len = sizeof(sockaddr);
				ret = recvfrom(sock,recv_buffer,sizeof(recv_buffer),0,(sockaddr *)&sour_addr,&sour_len);
				
				if(TFTP_ERROR==recv_buffer[1]){
					ErrorHandle(recv_buffer[3]);
					}
				if(TFTP_ACK==recv_buffer[1]){
					lastack = MAKEWORD(recv_buffer[3],recv_buffer[2]);//��ǰ��ack 

					switch(stat){//stat��ֵΪ0 
						case TFTP_WSTAT_FIRSTACK:
							if(0==lastack){
								stat = TFTP_WSTAT_NEXTACK;

								//��Ŀ��˿ڻ�������������˿�
								dest_addr.sin_port = sour_addr.sin_port;
				
								rlen = fread(data_buffer,1,512,file);
								flen = flen + rlen;
								if(rlen<512 && feof(file)){ // ��СС��512�ֽ��Ҽ�⵽�ļ�������
									stat = TFTP_WSTAT_LASTACK;
								}
								else{
									if(ferror(file)){
										AfxMessageBox("��ȡ�ļ�ʧ��");
										fclose(file);
										return;
									}
								}
								blocknum++;//��ǰӦ�ý��ܵĿ�� 

								if(blocknum > 65535)
									blocknum = 0;
								
								len = makedata(blocknum,data_buffer,rlen,send_buffer,sizeof(send_buffer));

								sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
								//printf("�ѷ��� %d �ֽ�\n",flen);
							}
							else{
								fclose(file);
								AfxMessageBox("ACK ��Ŵ���");
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
										AfxMessageBox("�����ļ�ʧ��");
										fclose(file);
										return;
									}
								}
								blocknum++;
								if(blocknum > 65535)//Խ�� 
									blocknum = 0;

								len = makedata(blocknum,data_buffer,rlen,send_buffer,sizeof(send_buffer));
								sendto(sock,send_buffer,len,0,(sockaddr *)&dest_addr,sizeof(dest_addr));
								//printf("�ѷ��� %d �ֽ�\n",flen);
							}
							else{
								fclose(file);
								AfxMessageBox("ACK ��Ŵ���");
								return;
							}
							break;
						case TFTP_WSTAT_LASTACK:
							if(lastack==blocknum){
								//printf("*****�������,������ %d �ֽ�*****\n",flen);
								AfxMessageBox("�ϴ����");
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
