// ApplicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FTPServerApp.h"
#include "ApplicationDlg.h"
#include "FTPServer.h"
#include "UserAccountsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFTPServerApp theApp;
extern CFTPServer theServer;//�����ö������ڱ𴦶���ģ�(��CFTPServerApp���cpp�ļ��ж����)
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CApplicationDlg dialog

CApplicationDlg::CApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CApplicationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CApplicationDlg)
	m_ip = _T("");
	m_port = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_FTPSERVER);
}

void CApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApplicationDlg)
	DDX_Text(pDX, IDC_EDIT_SERVER_IP, m_ip);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CApplicationDlg, CDialog)
	//{{AFX_MSG_MAP(CApplicationDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_SERVER_START, OnServerStart)
	ON_COMMAND(ID_SERVER_STOP, OnServerStop)
	ON_COMMAND(ID_TRACEPAGE, OnViewTracePage)
	ON_COMMAND(ID_VIEW_ONLINEUSERS, OnViewOnlineUserPage)
	ON_COMMAND(ID_VIEW_STATISTICS, OnViewStatisticsPage)
	ON_COMMAND(ID_VIEW_CONFIGURATION, OnViewConfigurationsPage)
	ON_COMMAND(ID_USER_ACCOUNTS, OnUserAccounts)
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_USER, OnButtonUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApplicationDlg message handlers

BOOL CApplicationDlg::OnInitDialog()
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
	int s, t, b, r, l;

	if (AfxGetApp()->GetProfileInt("Settings", "SavePosition", 1))
	{
		// only restore if there is a previously saved position
		if ( -1 != (s = AfxGetApp()->GetProfileInt("Settings", "FrameStatus",   -1)) &&
			 -1 != (t = AfxGetApp()->GetProfileInt("Settings", "FrameTop",      -1)) &&
			 -1 != (l = AfxGetApp()->GetProfileInt("Settings", "FrameLeft",     -1)) &&
			 -1 != (b = AfxGetApp()->GetProfileInt("Settings", "FrameBottom",   -1)) &&
			 -1 != (r = AfxGetApp()->GetProfileInt("Settings", "FrameRight",    -1))
		   ) 
		{
			WINDOWPLACEMENT wp;
			
			// restore the window's status
			wp.showCmd = s;

			// restore the window's width and height
			wp.rcNormalPosition.bottom = b;
			wp.rcNormalPosition.right = r;

			// the following correction is needed when the taskbar is
			// at the left or top and it is not "auto-hidden"
			RECT workArea;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
			l += workArea.left;
			t += workArea.top;

			// make sure the window is not completely out of sight
			int max_x = GetSystemMetrics(SM_CXSCREEN) -
							GetSystemMetrics(SM_CXICON);
			int max_y = GetSystemMetrics(SM_CYSCREEN) -
							GetSystemMetrics(SM_CYICON);
			wp.rcNormalPosition.top = min(t, max_y);
			wp.rcNormalPosition.left = min(l, max_x);

			SetWindowPlacement(&wp);
		}
	}
	char ip[20] = {0};  
    findIP(ip, sizeof(ip));

	CEdit* c_serverport=(CEdit*)GetDlgItem(IDC_EDIT_SERVER_PORT);
	c_serverport->SetWindowText("21");
	GetDlgItem(IDC_EDIT_SERVER_IP)->SetWindowText(ip);

	//CreateToolbar();   //��ʾ������

	// create property pages
	m_TracePage.Create(IDD_PROPPAGE_TRACE, this);
	m_OnlineUserPage.Create(IDD_PROPPAGE_ONLINE, this);
	m_ConfigurationPage.Create(IDD_PROPPAGE_CONFIGURATION, this);
	m_StatisticsPage.Create(IDD_PROPPAGE_STATISTICS, this);

	// activate main page 
	ActivatePage(0);

	// get registry values
	//m_ConfigurationPage.m_nPort = AfxGetApp()->GetProfileInt("Settings", "Port", 21);
	//m_ConfigurationPage.m_nMaxUsers = AfxGetApp()->GetProfileInt("Settings", "MaxUsers", 10);
	//m_ConfigurationPage.m_nTimeout = AfxGetApp()->GetProfileInt("Settings", "Timeout", 5);
	//m_ConfigurationPage.m_strWelcomeMessage = AfxGetApp()->GetProfileString("Settings", "WelcomeMsg", "Welcome to BCY FTP Server"); 
	//m_ConfigurationPage.m_strGoodbyeMessage = AfxGetApp()->GetProfileString("Settings", "GoodbyeMsg", "Bye"); 
	//m_ConfigurationPage.m_bLaunchOnStartup = AfxGetApp()->GetProfileInt("Settings", "LaunchOnStartup", 0);
	//m_ConfigurationPage.m_bStartInTray = AfxGetApp()->GetProfileInt("Settings", "StartInTray", 0);
	//m_ConfigurationPage.m_bAutoActivate = AfxGetApp()->GetProfileInt("Settings", "AutoActivate", 0);
	//m_ConfigurationPage.m_nLogLevel = AfxGetApp()->GetProfileInt("Settings", "LogLevel", 1);
	//m_ConfigurationPage.m_strApplicationName = AfxGetApp()->GetProfileString("Settings", "AppName", "BCY FTP Server");

	//m_ConfigurationPage.UpdateData(FALSE);

	theServer.Initialize(this);//��һ�����Ҫ�������Ǽ��˰�!

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CApplicationDlg::OnPaint() 
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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CApplicationDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::ActivatePage					*/
/* Description   : Called when an icon on the outlookbar is pressed.*/
/*																	*/
/********************************************************************/
void CApplicationDlg::ActivatePage(int nIndex)
{
	switch(nIndex)
	{
		case 0:
			m_OnlineUserPage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);
			m_StatisticsPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_SHOW);
			break;
		case 1:
			m_TracePage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);	
			m_StatisticsPage.ShowWindow(SW_HIDE);		
			m_OnlineUserPage.ShowWindow(SW_SHOW);
			break;
		case 2:
			m_OnlineUserPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_HIDE);	
			m_StatisticsPage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_SHOW);	
			break;
		case 3:
			m_OnlineUserPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);		
			m_StatisticsPage.ShowWindow(SW_SHOW);	
			break;
		case 4:
			m_OnlineUserPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);	
			m_StatisticsPage.ShowWindow(SW_HIDE);	
			break;
		default:
			break;
	}
	MoveChilds();//û����������ĺ�������������ҳ���ڻ�����ڲ˵��������棬���Ҳ�����picture�ؼ���С��ֻ������Դ�������д����Ĵ�С
}

/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::MoveChilds						*/		
/* Description   : Move child windows into place holder area.		*/
/*																	*/
/********************************************************************/
//��������ҳ���ڴ�С�ʺ���picture�ؼ���С�����ŵ�picture����ȥ��
void CApplicationDlg::MoveChilds()
{
	// position property pages 
	CRect rcDlgs;
	
	// get dialog area rect
	GetDlgItem(IDC_DIALOG_AREA)->GetWindowRect(rcDlgs);
	
	ScreenToClient(rcDlgs);
	
	m_ConfigurationPage.MoveWindow(rcDlgs); 
	m_TracePage.MoveWindow(rcDlgs); 
	m_OnlineUserPage.MoveWindow(rcDlgs); 
	m_StatisticsPage.MoveWindow(rcDlgs); 
}
//����ʹ������ڴ�С��picture�ؼ���С����Ӧ�����Ǵ��ڵĿؼ���û����֮�仯����Ҫ�ڸ��������������Ӧ��
//��ON_SIZE��Ϣ��Ӧ����������Ϣ��Ӧ�����е��������ؼ��Ĵ�С���˷��������ڴ���ֻ�к��ٵĿؼ�������кܶ�Ŀؼ�
//�ǲ�ʵ�õġ�


//theServer��CFTPServerApp���cpp�����ж��壬��һ��ȫ�ֱ���
void CApplicationDlg::OnServerStart() 
{
	m_ConfigurationPage.UpdateData();
	theServer.SetPort(m_ConfigurationPage.m_nPort);//��CFTPServerApp.cpp�ж���
	//theServer.SetMaxUsers(m_ConfigurationPage.m_nMaxUsers);
	//theServer.SetTimeout(m_ConfigurationPage.m_nTimeout);
	//theServer.SetWelcomeMessage(m_ConfigurationPage.m_strWelcomeMessage);
	//theServer.SetGoodbyeMessage(m_ConfigurationPage.m_strGoodbyeMessage);
	theServer.SetStatisticsInterval(5000);

	theServer.Start();
}

//ֹͣ������
void CApplicationDlg::OnServerStop() 
{
	theServer.Stop();
}

//�û��������Ӧ���룬���ᵯ���û�����Ի���
void CApplicationDlg::OnUserAccounts() 
{
	CUserAccountsDlg dlg;

	theServer.m_UserManager.GetUserAccountList(dlg.m_UsersArray);
	if(dlg.DoModal() == IDOK)
	{
		theServer.m_UserManager.UpdateUserAccountList(dlg.m_UsersArray);//�����ݱ��浽�ļ���
	}
}

//FTP Status changed.	
void CApplicationDlg::OnFTPStatusChange(int nType, LPCTSTR lpszText)
{
	m_TracePage.AddTraceLine(nType, lpszText);
	
	switch(nType)
	{
		case 3:
			theApp.m_LogFile << error_lvl << date << time << lpszText << endl;
			break;
		default:
			theApp.m_LogFile << trace_lvl << date << time << lpszText << endl;
			break;
	}
}

//A user has connected to our server.	
void CApplicationDlg::OnFTPUserConnected(DWORD nThreadID, LPCTSTR lpszUser, LPCSTR lpszAddress)
{
	m_OnlineUserPage.AddUser(nThreadID, lpszUser, lpszAddress);
}

//A user has disconnected from our server.
void CApplicationDlg::OnFTPUserDisconnected(DWORD nThreadID, LPCTSTR lpszUser)
{
	m_OnlineUserPage.RemoveUser(nThreadID);
}

/********************************************************************/
/*																	*/
/* Function name : OnFTPReceivedBytesChange							*/		
/* Description   : Number of received bytes has changed.			*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnFTPReceivedBytesChange(int nBytes) 
{
	m_StatisticsPage.SetValue(6, FormatSize(nBytes, 0));
}


/********************************************************************/
/*																	*/
/* Function name : OnFTPSentBytesChange								*/		
/* Description   : Number of sent bytes has changed.				*/	
/*																	*/
/********************************************************************/
void CApplicationDlg::OnFTPSentBytesChange(int nBytes)
{
	m_StatisticsPage.SetValue(7, FormatSize(nBytes, 0));
}

void CApplicationDlg::OnFTPStatisticChange(int nType, int nValue)
{
	m_StatisticsPage.SetValue(nType, nValue);
}

/********************************************************************/
/*																	*/
/* Function name : FormatSize										*/
/* Description   : Format size, the way explorer diplays it			*/
/*																	*/
/********************************************************************/
char* CApplicationDlg::FormatSize(DWORD dwSizeLow, DWORD dwSizeHigh)
{
	static char szBuff[100];

	unsigned __int64 nFileSize = ((unsigned __int64)(((DWORD)(dwSizeLow)) | 
								 ((unsigned __int64)((DWORD)(dwSizeHigh))) << 32));
	unsigned __int64 kb = 0;

	if (nFileSize > 1024)
	{
		kb = nFileSize / 1024;
		if (nFileSize % 1024)
			kb++;
	}

	// make it a string
	_ui64tot(kb, szBuff, 10);

	// add thousand seperators
	int nLength = lstrlen(szBuff);
	if (nLength > 3)
	{
		LPCTSTR ptr = szBuff;
		ptr += (nLength-1);

		char szTemp[100];

		LPTSTR ptrTemp = szTemp;
		for(int i=0; i<nLength; i++)
		{
			if (i && ((i % 3) == 0)) 
			{
				if (*ptrTemp != ',')
				{
					*ptrTemp = ',';
					ptrTemp = _tcsinc(ptrTemp);
				}
			}
			*ptrTemp = *ptr;
			ptrTemp = _tcsinc(ptrTemp);
			ptr = _tcsdec(szBuff, ptr);
		}
		// terminate string
		*ptrTemp = '\0';
		// reverse string
		_tcsrev(szTemp);
		lstrcpy(szBuff, szTemp);
	}
	// add 'KB' to it
	lstrcat(szBuff, " KB");
	return szBuff;
}

//������ʹ�����
BOOL CApplicationDlg::CreateToolbar()
{
	// Create the Toolbar and attach the resource
	if(!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_TOOLBAR))
	{
		TRACE0("Failed to Create Dialog Toolbar\n");
		return FALSE;	
	}

	InitToolBar();  //������ʹ�����

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	
	// Make it flat.
	m_wndToolBar.ModifyStyle(0, m_wndToolBar.GetStyle()|TBSTYLE_FLAT);
	
	// And position the control bars
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
    return TRUE;
}

//��ʾ������
void CApplicationDlg::InitToolBar()
{
	CBitmap bm;

	//��Ĺ�����
	m_imageToolBar.Create(32,32,TRUE|ILC_COLOR24,8,0);
	bm.LoadBitmap(IDB_TOOLBARHOT);
	m_imageToolBar.Add(&bm,(CBitmap*)NULL);
	bm.Detach();
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imageToolBar);

	//���ƵĹ�����
	m_imageToolBarDisable.Create(32,32,TRUE|ILC_COLOR24,8,0);
	bm.LoadBitmap(IDB_TOOLBARDISABLE);
	m_imageToolBarDisable.Add(&bm,(CBitmap*)NULL);
	bm.Detach();
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_imageToolBarDisable);

	//��ǰ�Ĺ�����
	m_imageToolBarHot.Create(32,32,TRUE|ILC_COLOR24,8,0);
	bm.LoadBitmap(IDB_TOOLBARHOT);
	m_imageToolBarHot.Add(&bm,(CBitmap*)NULL);
	bm.Detach();
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&m_imageToolBarHot);
}

//��ӦWM_CLOSE��Ϣ�ĺ���
void CApplicationDlg::OnClose() 
{
	if (AfxGetApp()->GetProfileInt("Settings", "SavePosition", 1))
	{
		// Save main window position
		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);
		AfxGetApp()->WriteProfileInt("Settings", "FrameStatus", wp.showCmd);
		AfxGetApp()->WriteProfileInt("Settings", "FrameTop",    wp.rcNormalPosition.top);
		AfxGetApp()->WriteProfileInt("Settings", "FrameLeft",   wp.rcNormalPosition.left);
		AfxGetApp()->WriteProfileInt("Settings", "FrameBottom", wp.rcNormalPosition.bottom);
		AfxGetApp()->WriteProfileInt("Settings", "FrameRight",  wp.rcNormalPosition.right);
	}	
	CDialog::OnClose();
}

void CApplicationDlg::OnViewTracePage()
{
	ActivatePage(0);
}

void CApplicationDlg::OnViewOnlineUserPage()
{
	ActivatePage(1);
}

void CApplicationDlg::OnViewStatisticsPage()
{
	ActivatePage(3);
}

void CApplicationDlg::OnViewConfigurationsPage()
{
	ActivatePage(2);
}
	
void CApplicationDlg::OnAbout()
{
}



void CApplicationDlg::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	//CString str;
	//int port;
	CFTPServerApp *pApp=(CFTPServerApp *)AfxGetApp( );
	pApp->server_ip = m_ip;
	
	theServer.SetPort(m_port);//��CFTPServerApp.cpp�ж���
	//theServer.SetMaxUsers(m_ConfigurationPage.m_nMaxUsers);
	//theServer.SetTimeout(m_ConfigurationPage.m_nTimeout);
	//theServer.SetWelcomeMessage(m_ConfigurationPage.m_strWelcomeMessage);
	//theServer.SetGoodbyeMessage(m_ConfigurationPage.m_strGoodbyeMessage);
	theServer.SetStatisticsInterval(5000);

	theServer.Start();
	
}

void CApplicationDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	theServer.Stop();
}

void CApplicationDlg::OnButtonUser() 
{
	// TODO: Add your control notification handler code here
	CUserAccountsDlg dlg;

	theServer.m_UserManager.GetUserAccountList(dlg.m_UsersArray);
	if(dlg.DoModal() == IDOK)
	{
		theServer.m_UserManager.UpdateUserAccountList(dlg.m_UsersArray);//�����ݱ��浽�ļ���
	}
	
}
