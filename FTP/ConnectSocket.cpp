// ConnectSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "ConnectSocket.h"
#include "DataSocket.h"

extern CFTPServer theServer;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectSocket

CConnectSocket::CConnectSocket()
{
	m_bLoggedon = FALSE;
	m_bPassiveMode = FALSE;
	m_strRemoteHost = "";
	m_nRemotePort = -1;
	m_dwRestartOffset = 0;
	m_pDataSocket = NULL;//�������ӵ�socketҪ�ǵó�ʼ��
}

CConnectSocket::~CConnectSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CConnectSocket, CSocket)
//{{AFX_MSG_MAP(CConnectSocket)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

#define BUFFERSIZE 4096
/////////////////////////////////////////////////////////////////////////////
// CConnectSocket member functions

void CConnectSocket::OnReceive(int nErrorCode) 
{
	TCHAR buff[BUFFERSIZE];
	int nRead = Receive(buff,BUFFERSIZE);
	
	switch(nRead)
	{
	case 0:
		Close();
		break;
	case SOCKET_ERROR:
		if(GetLastError() != WSAEWOULDBLOCK)
		{
			TCHAR szError[256];
			wsprintf(szError,"OnReceive error:%d",GetLastError());
			AfxMessageBox(szError);
		}
		break;
	default:
		if(nRead != SOCKET_ERROR && nRead != 0)
		{
			//((CConnectThread*)AfxGetThread())->IncReceivedBytes(nRead);
			buff[nRead] = 0;//�ٽ��յ������ݺ��������' ',0��' '��ASCII��ֵ�����ܵ�����еĴ���������������ַ���\r(13)\n(10).
			m_RxBuffer += CString(buff);
			GetRxLine();
		}
		break;
	}
	CSocket::OnReceive(nErrorCode);
}

//��������������
void CConnectSocket::GetRxLine()
{
	CString strTemp;
	int nIndex;
	while(!m_RxBuffer.IsEmpty())
	{
		//Ѱ�һ��з�
		nIndex = m_RxBuffer.Find("\r\n");
		if(nIndex != -1)
		{
			strTemp = m_RxBuffer.Left(nIndex);
			m_RxBuffer = m_RxBuffer.Mid(nIndex+2);
			if(!strTemp.IsEmpty())
			{
				m_strCommands.AddTail(strTemp);
				//������ִ������
				ParseCommand();
			}
		}
		else 
			break;
	}
}

//FireStatusMessageͨ���������������TracePageҳ����ʾ��¼��Ϣ�ġ�
//�ӻ������л������
BOOL CConnectSocket::GetRxCommand(CString &strCommand, CString &strArguments)
{
	if(!m_strCommands.IsEmpty())
	{
		CString strBuff = m_strCommands.RemoveHead();
		int nIndex = strBuff.Find(" ");
		if(nIndex != -1)
		{
			CString strPassword = strBuff;
			strPassword.MakeUpper();
			//�����벻�ɼ�
			if(strPassword.Left(5) == "PASS")
			{
				for(int i=5; i<strPassword.GetLength(); i++)
				{
					strPassword.SetAt(i,'*');
				}
				FireStatusMessage(strPassword,1);
			}
			else
			{
				FireStatusMessage(strBuff,1);
			}
			strCommand = strBuff.Left(nIndex);
			strArguments = strBuff.Mid(nIndex+1);
		}
		else
		{
			FireStatusMessage(strBuff,1);
			strCommand = strBuff;
		}
		
		if(strCommand != "")
		{
			strCommand.MakeUpper();
			
			if(strCommand.Right(4) == "ABOR")
			{
				strCommand = "ABOR";
			}
			TRACE2("COMMAND: %s, ARGS: %s\n", strCommand, strArguments);
			return TRUE;
		}
	}
	return FALSE;
}


void CConnectSocket::ParseCommand()
{
	static CFTPCommand commandList[] = 
	{
		{TOK_ABOR,	"ABOR", FALSE,	"Abort transfer: ABOR"}, 
		{TOK_BYE,	"BYE",  FALSE,	"Logout or break the connection: BYE"},
		{TOK_CDUP,	"CDUP", FALSE,	"Change to parent directory: CDUP"},
		{TOK_CWD,	"CWD",	TRUE,	"Change working directory: CWD [directory-name]"},
		{TOK_DELE,	"DELE", TRUE ,	"Delete file: DELE file-name"},
		{TOK_DIR,	"DIR",  FALSE,	"Get directory listing: DIR [path-name]"},
		{TOK_HELP,	"HELP",  FALSE, "Show help: HELP [command]"},
		{TOK_LIST,	"LIST", FALSE,	"Get directory listing: LIST [path-name]"}, 
		{TOK_MKD,	"MKD",	TRUE,	"Make directory: MKD path-name"},
		{TOK_NOOP,	"NOOP", FALSE,	"Do nothing: NOOP"},
		{TOK_PASS,	"PASS", TRUE,	"Supply a user password: PASS password"},
		{TOK_PASV,	"PASV", FALSE,	"Set server in passive mode: PASV"},
		{TOK_PORT,	"PORT", TRUE,	"Specify the client port number: PORT a0,a1,a2,a3,a4,a5"},
		{TOK_PWD,	"PWD",	FALSE,	"Get current directory: PWD"},
		{TOK_QUIT,	"QUIT",  FALSE,	"Logout or break the connection: QUIT"},
		{TOK_REST,	"REST", TRUE,	"Set restart transfer marker: REST marker"},
		{TOK_RETR,	"RETR", TRUE,	"Get file: RETR file-name"},
		{TOK_RMD,	"RMD",	TRUE,	"Remove directory: RMD path-name"},
		{TOK_RNFR,	"RNFR", TRUE,	"Specify old path name of file to be renamed: RNFR file-name"},
		{TOK_RNTO,	"RNTO", TRUE,	"Specify new path name of file to be renamed: RNTO file-name"},
		{TOK_SIZE,	"SIZE", TRUE,	"Get filesize: SIZE file-name"},
		{TOK_STOR,	"STOR", TRUE,	"Store file: STOR file-name"},
		{TOK_SYST,	"SYST", FALSE,	"Get operating system type: SYST"},
		{TOK_TYPE,	"TYPE", TRUE,	"Set filetype: TYPE [A | I]"},
		{TOK_USER,	"USER", TRUE,	"Supply a username: USER username"},
		{TOK_ERROR,	"",		FALSE,  ""},
	};
	
	CString strCommand,strArguments;
	if(!GetRxCommand(strCommand,strArguments))
	{
		return;
	}
	int nCommand;
	
	for(nCommand = TOK_ABOR; nCommand < TOK_ERROR; nCommand++)
	{
		if(strCommand == commandList[nCommand].m_pszName)
		{
			//if(commandList[nCommand].m_bHasArguments && (strArguments.IsEmpty()))
			//{
				//SendResponse("501 Syntax error:Invalid number of parameters.");
				//AfxMessageBox(_T("501 Syntax error:Invalid number of parameters."));
				//return;
			//}
			break;
		}
	}
	
	if(nCommand == TOK_ERROR)
	{
		SendResponse("501 Syntax error:Command not understood.");
		//AfxMessageBox(_T("501 Syntax error:Command not understood."));
		return;
	}
	
	if((nCommand != TOK_USER && nCommand != TOK_PASS) && !m_bLoggedon)
	{
		SendResponse("530 Please login with USER and PASS.");
		//AfxMessageBox(_T("530 Please login with USER and PASS."));
		return;
	}
	
	switch(nCommand)
	{
		//ָ���û���
	case TOK_USER:
		{
			strArguments.MakeLower();
			m_bLoggedon = FALSE;
			m_strUserName = strArguments;
			
			CString strPeerAddress;
			UINT nPeerPort;
			GetPeerName(strPeerAddress,nPeerPort);
			
			// ֪ͨFTP������һ���µ��û��Ѿ�����
			CConnectThread *pThread = (CConnectThread*)m_pThread;
			((CFTPServer *)pThread->m_pWndServer)->m_pEventSink->OnFTPUserConnected(m_pThread->m_nThreadID, m_strUserName, strPeerAddress);
			SendResponse("331 User name ok,need password.");
			//AfxMessageBox(_T("331 User name ok,need password."));//test!!!
		}
		break;
		
		//ָ������
	case TOK_PASS:
		{
			if(m_bLoggedon)
			{
				SendResponse("503 Login with USER first.");
				//AfxMessageBox(_T("503 Login with USER first."));//test
			}
			else
			{
				CUserAccount useraccount;
				if(theServer.m_UserManager.GetUserAccount(m_strUserName,useraccount))
				{
					if(!useraccount.m_strPassword.Compare(strArguments) || useraccount.m_strPassword.IsEmpty())
					{
						m_strCurrentDir = "/";
						m_bLoggedon = TRUE;
						SendResponse("230 User sucessfully logged in.");
						break;
					}
				}
				SendResponse("530 Not logged in,user or password incorrect!");
			}
			break;
			
			// �ı䴫��ģʽ
	case TOK_TYPE:
		{
			SendResponse("200 Type set to %s", strArguments);
		}
		break;
		
		//��ӡ��ǰ·��
	case TOK_PWD:
		{
			SendResponse("257 \"%s\" is current directory.", m_strCurrentDir);
		}
		break;
		
		//������������
	case TOK_REST:
		{
			if(!IsNumeric(strArguments.GetBuffer(strArguments.GetLength())))
			{
				strArguments.ReleaseBuffer();
				SendResponse("501 Invalid numeric!");
			}
			else
			{
				strArguments.ReleaseBuffer();
				m_dwRestartOffset = atol(strArguments);
				SendResponse("350 Restarting at %d.", m_dwRestartOffset);
			}
		}
		break;
		
		//�ı䵽����ģʽ
	case TOK_PASV:
		{ 
			//ɾ����ǰ����������
			DestroyDataConnection();
			//�����µ������׽�������
			m_pDataSocket = new CDataSocket(this,-1);
			
			if(!m_pDataSocket->Create())
			{
				DestroyDataConnection();
				SendResponse("421 Failed to create socket.");
				break;
			}
			//��ʼ����
			m_pDataSocket->Listen();
			m_pDataSocket->AsyncSelect();
			
			CString strIP,strTmp;
			UINT nPort;
			//��ȡ���ǵ�IP��ַ
			GetSockName(strIP,nPort);//strIP�Ǳ�����IP��ַ��nPort��21
			//��ȡ�µļ����׽��ֵĶ˿ں�
			m_pDataSocket->GetSockName(strTmp,nPort);//strTmp��0.0.0.0��nPort�ǻ����Զ������
			//ԭ��������ֻ�Ǹ�����socket��û�пͻ������ӣ�����ͻ��������˵Ļ����Ϳ��Ի�ñ�����IP��ַ�ˡ�
			//������������ô��ֵ�������롣Ӧ���������ģ��д���֤���Լ������ٺ٣�����
			//�滻"."
			strIP.Replace(".",",");
			//֪ͨ�ͻ���Ҫ���ӵ�IP��ַ�Ͷ˿�
			SendResponse("227 Entering Passive Mode(%s,%d,%d).",strIP,nPort/256,nPort%256);
			m_bPassiveMode = TRUE;
			break;
		}
		
		//�г���ǰ·��
	case TOK_LIST:
	case TOK_DIR:
		{
			//������Ǳ���ģʽ����������ҪPORT����ָ���˿�
			if(!m_bPassiveMode && (m_strRemoteHost == "" || m_nRemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
				//AfxMessageBox(_T("503 Bad sequence of commands."));
			}
			else
			{
				//����û�û��ָ��·�����õ�ǰ·��
				if(strArguments == "")
				{
					strArguments = m_strCurrentDir;
				}
				
				else
				{
					//���������ļ�������·��
					CString strResult;
					int nResult = theServer.m_UserManager.CheckFileName(m_strUserName,strArguments,m_strCurrentDir,FTP_LIST,strResult);
					if(nResult == 0)
					{
						strArguments = strResult;
					}
				}
				
				CString strListing;
				int nResult = theServer.m_UserManager.GetDirectoryList(m_strUserName,strArguments,strListing);
				switch(nResult)
				{
				case 1:
					SendResponse("550 Permission denied.");
					break;
				case 2:
					SendResponse("550 Directory not found.");
					break;
				default:
					//�����׽������ӻ��·���б�
					if(!CreateDataConnection(0,strListing))
					{
						DestroyDataConnection();
					}
					break;
				}
			}
		}
		break;
		
		// �ı��ϲ�Ŀ¼
	case TOK_CDUP:
		strArguments = "..";
		
		//�ı乤��Ŀ¼
	case TOK_CWD:
		{
			//�ı䵽ָ��Ŀ¼
			int nResult = theServer.m_UserManager.ChangeDirectory(m_strUserName,m_strCurrentDir,strArguments);
			
			switch(nResult)
			{
			case 0:
				SendResponse("250 CWD command successful.\"%s\" is current directory.",m_strCurrentDir);
				break;
			case 1:
				SendResponse("550 CWD command failed.\"%s\":Permission denied.",strArguments);
				break;
			default:
				SendResponse("550 CWD command failed.\"%s\":Directory not found.",strArguments);
				break;
			}
		}
		break;
		
    // ָ��IP��ַ�Ͷ˿�
	case TOK_PORT:
		{
			CString strSub;
			int nCount=0;
			
			while (AfxExtractSubString(strSub, strArguments, nCount++, ','))
			{
				switch(nCount)
				{
				case 1:	
					m_strRemoteHost = strSub;
					m_strRemoteHost += ".";
					break;
				case 2:	
					m_strRemoteHost += strSub;
					m_strRemoteHost += ".";
					break;
				case 3:	
					m_strRemoteHost += strSub;
					m_strRemoteHost += ".";
					break;
				case 4:	
					m_strRemoteHost += strSub;
					break;
				case 5:	
					m_nRemotePort = 256*atoi(strSub);
					break;
				case 6:	
					m_nRemotePort += atoi(strSub);
					break;
				}
			}
			m_bPassiveMode = FALSE;
			SendResponse("200 Port command successful.");
			break;
		}
		
		//����ļ��������ļ�
	case TOK_RETR:
		{
			//������Ǳ���ģʽ����������ҪPORT����ָ���˿�
			if(!m_bPassiveMode && (m_strRemoteHost == "" || m_nRemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
				break;
			}
			
			CString strResult;
			int nResult = theServer.m_UserManager.CheckFileName(m_strUserName,strArguments,m_strCurrentDir,FTP_DOWNLOAD,strResult);

			switch(nResult)
			{
			case 1:
				SendResponse("550 Permission denied.");
				break;
			case 2:
				SendResponse("550 File not found.");
				break;
			default:
				//Ϊ�ļ����䴴��һ���׽���
				if(!CreateDataConnection(1,strResult))
				{
					DestroyDataConnection();
				}
				break;
			}
			break;
		}
		
		//�ͻ����ϴ��ļ�
	case TOK_STOR:
		{
			//������Ǳ���ģʽ����������ҪPORT����ָ���˿�
			if(!m_bPassiveMode && (m_strRemoteHost == "" || m_nRemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
				break;
			}
			
			CString strResult;
			int nResult = theServer.m_UserManager.CheckFileName(m_strUserName,strArguments,m_strCurrentDir,FTP_UPLOAD,strResult);

			switch(nResult)
			{
			case 1:
				SendResponse("550 Permission denied.");
				break;
			case 2:
				SendResponse("550 Filename invalid.");
				break;
			default:
				//Ϊ�ļ����䴴��һ���׽���
				if(!CreateDataConnection(2,strResult))
				{
					DestroyDataConnection();
				}
				break;
			}
		}
		break;
		
		//ɾ���ļ�
		//ɾ���ļ��������Ҫ�ڷ���PASV�����������������Ҫ��ͻ�����̫�����ݣ�ֻ��Ҫ����һ����Ӧ�����ˣ�����Ҫ�ٿ�һ������
		//���ӵ�SOCKET.
	case TOK_DELE:
		{
			CString strResult;
			int nResult = theServer.m_UserManager.CheckFileName(m_strUserName,strArguments,m_strCurrentDir,FTP_DELETE,strResult);

			switch(nResult)
			{
			case 1:
				SendResponse("550 Permission denied.");
				break;
			case 2:
				SendResponse("550 File not found.");
				break;
			default:
				//ɾ���ļ�
				if(!DeleteFile(strResult))
				{
					SendResponse("450 Internal error deleting the file:\"%s\".",strArguments);
				}
				else 
				{
					SendResponse("250 File \"%s\" wsa deleted successfully.",strArguments);
				}
				break;
			}
		}
		break;
		
		//ɾ��Ŀ¼
		//����Ŀ¼ɾ�������⣬Ŀ¼����Ҳ����ɾ����ֱ��ɾ��һ���յ�Ŀ¼��û�������,�����������ļ���Ŀ¼�ǲ���ɾ���ġ�������Э������⣬Ҳ������·��������.
	case TOK_RMD:
		{
			CString strResult;
			int nResult = theServer.m_UserManager.CheckDirectory(m_strUserName,strArguments,m_strCurrentDir,FTP_DELETE,strResult);

			switch(nResult)
			{
			case 1:
				SendResponse("550 Permission denied.");
				break;
			case 2:
				SendResponse("550 Directory not found.");
				break;
			default:
				//ɾ��Ŀ¼
				if(!RemoveDirectory(strResult))
				{
					if(GetLastError() == ERROR_DIR_NOT_EMPTY)
					{
						SendResponse("550 Directory not empty.");
					}
					else
					{
						SendResponse("450 Internal error deleting the directory.");
					}
				}
				else
				{
					SendResponse("250 Directory deleted successfully.");
				}
				break;
			}
		}
		break;
		
		//����Ŀ¼
	case TOK_MKD:
		{
			CString strResult;
			int nResult = theServer.m_UserManager.CheckDirectory(m_strUserName,strArguments,m_strCurrentDir,FTP_CREATE_DIR,strResult);
			//ע�ⰲȫ���ƣ�������ڿͻ��˷�������������(С��/С��)������ȷ����Ŀ¼
			//����ҷ���������(С��\С��)�ͻ���ִ��������Ҫ�ڷ������˽���ת��
			//��CheckDirectory�Ĺ���������ˡ�

			switch(nResult)
			{
			case 0:
				SendResponse("550 Directory already exists.");
				break;
			case 1:
				SendResponse("550 Can't create directory.Permission denied.");
				break;
			default:
				//����Ŀ¼
				if(!MakeSureDirectoryPathExists(strResult))
				{
					SendResponse("450 Internal error creating the directory.");
				}
				else
				{
					SendResponse("250 Directory created successfully.");
				}
				break;
			}
		}
		break;
		
		//�������ļ�������·��
	case TOK_RNFR:
		{
			//�û����������ļ�
			CString strResult;
			int nResult = theServer.m_UserManager.CheckFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);

			if (nResult == 0)
			{
				m_strRenameFile = strResult;
				m_bRenameFile = TRUE;
				SendResponse("350 File exists, ready for destination name.");
				break;
			}
			else
			{
				// �û���������Ŀ¼
				nResult = theServer.m_UserManager.CheckDirectory(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);
	
				switch(nResult)
				{
				case 0:
					m_strRenameFile = strResult;
					m_bRenameFile = FALSE;
					SendResponse("350 Directory exists, ready for destination name.");
					break;
				case 1:
					SendResponse("550 Permission denied.");
					break;
				default: 
					SendResponse("550 File/directory not found.");
					break;
				}
			}	
		}
		break;
		
		// �������ļ�����Ŀ¼
	case TOK_RNTO:
		{
			if (m_strRenameFile.IsEmpty())
			{
				SendResponse("503 Bad sequence of commands.");
				break;
			}
			if (m_bRenameFile)
			{
				CString strResult;
				// ���Ŀ���ļ���
				int nResult = theServer.m_UserManager.CheckFileName(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);

				switch(nResult)
				{
				case 0:
					SendResponse("550 File already exists.");
					break;
				case 1:
					SendResponse("550 Permission denied.");
					break;
				default:
					// �������ļ�
					if (!MoveFile(m_strRenameFile, strResult))
					{
						SendResponse("450 Internal error renaming the file: \"%s\".", m_strRenameFile);
					}
					else
					{
						SendResponse("250 File \"%s\" renamed successfully.", m_strRenameFile);
					}
					break;
				}
			}
			else
			{
				CString strResult;
				// ���Ŀ��·����
				int nResult = theServer.m_UserManager.CheckDirectory(m_strUserName, strArguments, m_strCurrentDir, FTP_RENAME, strResult);
	
				switch(nResult)
				{
				case 0:
					SendResponse("550 Directory already exists.");
					break;
				case 1:
					SendResponse("550 Permission denied.");
					break;
				case 3:
					SendResponse("550 Directory invalid.");
					break;
				default:
					// ������·��
					if (!MoveFile(m_strRenameFile, strResult))
					{
						SendResponse("450 Internal error renaming the directory: \"%s\".", m_strRenameFile);
					}
					else
					{
						SendResponse("250 Directory \"%s\" renamed successfully.", m_strRenameFile);
					}
					break;
				}
			}		
		}
		break;
		
		//ȡ������
	case TOK_ABOR:
		{
			if(m_pDataSocket)
			{
				if(m_pDataSocket->GetStatus() != XFERMODE_IDLE)//��������е���ࣿ��������
				{
					SendResponse("426 Data connection closed.");
				}
				//������������
				m_pThread->PostThreadMessage(WM_THREADMSG,0,0);
			}
			SendResponse("226 ABOR command successful.");
			break;
		}
		
		//���ϵͳ��Ϣ
	case TOK_SYST:
		{
			SendResponse("215 Windows FTPServer by SiXing Bai.");
			break;
		}
		
		//�ر�����
	case TOK_QUIT:
	case TOK_BYE:
		{
			//�����ټ���Ϣ���ͻ���
			CConnectThread *pThread = (CConnectThread*)m_pThread;
			SendResponse("220 %s",((CFTPServer*)pThread->m_pWndServer)->GetGoodbyeMessage());
			SendResponse("220 GoodBye!");//test!!
			Close();
			//֪ͨ�߳��Ѿ������ر�����
			m_pThread->PostThreadMessage(WM_THREADMSG,1,0);
			break;
		}
		
		//��ʾ������Ϣ
	case TOK_HELP:
		{
			//����ͻ��˲�ָ����������ʾ��������
			if(strArguments == "")
			{
				CString strResponse = "214-The following commands are recognized:\r\n";
				//�������б��в�������
				for(int i=TOK_ABOR; i<TOK_ERROR; i++)
				{
					strResponse += commandList[i].m_pszName;
					strResponse += "\r\n";
				}
				strResponse += "214 HELP command successful.";
				SendResponse(strResponse);
			}
			else
			{
				int nHelpCmd;
				//�������б��в�������
				for(nHelpCmd = TOK_ABOR; nHelpCmd<TOK_ERROR; nHelpCmd++)
				{
					//�ж��Ƿ��ҵ�����
					if(strArguments.CompareNoCase(commandList[nHelpCmd].m_pszName)==0)
					{
						break;
					}
				}
				if(nHelpCmd != TOK_ERROR)
				{
					//��ʾ���������Ϣ
					SendResponse("214 %s",commandList[nHelpCmd].m_pszDescription);
				}
				else
				{
					SendResponse("501 Unknown command %s",strArguments);
				}
			}
		}
		break;
		
		//������
	case TOK_NOOP:
		{
			SendResponse("220 ok");
		}
		break;
		
	default:
		SendResponse("502 Command not implemented - Try HELP.");
		break;
	}
	
}
}//��������һ�����ţ���������ĺ����ڱ���ʱ�����ֲ���������Ƿ����쳣

BOOL CConnectSocket::CreateDataConnection(int nTransferType, LPCTSTR lpszData)
{
	if(!m_bPassiveMode)
	{
		m_pDataSocket = new CDataSocket(this,nTransferType);
		if(m_pDataSocket->Create())
		{
			m_pDataSocket->AsyncSelect();
			m_pDataSocket->SetRestartOffset(m_dwRestartOffset);
			m_pDataSocket->SetData(lpszData);
			
			if(m_pDataSocket->Connect(m_strRemoteHost,m_nRemotePort))
			{
				if(GetLastError() != WSAEWOULDBLOCK)
				{
					SendResponse("425 Can't open data connection.");
					//AfxMessageBox(_T("425 Can't open data connection."));
					return FALSE;
				}
			}
			
			switch(nTransferType)
			{
			case 0:
				SendResponse("150 Opening ASCII mode data connection for directory list.");
				//AfxMessageBox(_T("150 Opening ASCII mode data connection for directory list."));
				break;
			case 1:
			case 2:
				SendResponse("150 Opening BINARY mode data connection for file transfer.");
				//AfxMessageBox(_T("150 Opening BINARY mode data connection for file transfer."));
				break;
			}
		}
		else
		{
			//SendResponse("421 Failed to create data connection socket.");
			AfxMessageBox(_T("421 Failed to create data connection socket."));
			return FALSE;
		}
	}
	else
	{
		m_pDataSocket->SetRestartOffset(m_dwRestartOffset);
		m_pDataSocket->SetData(lpszData);
		m_pDataSocket->SetTransferType(nTransferType,TRUE);
		//��3��ִ����Ż�ȥִ��m_pDataSocket��OnAccept()������������SetTransferType��������
		//��Ҫ�ж��ڱ���ģʽ�������������ͻ�����ģʽ���£��Ƿ�ͻ��������ˣ�������жϵĻ���
		//OnAccept()��������ִ�У���û�пͻ��������ӣ���Ҳ�ǿ�ʼһֱ�޷������ݷ���ȥ��ԭ��!!
	}
	return TRUE;
}

void CConnectSocket::DestroyDataConnection()
{
	if (!m_pDataSocket)
		return;
	
	delete m_pDataSocket;
	
	// reset transfer status
	m_pDataSocket = NULL;
	m_strRemoteHost = "";
	m_nRemotePort = -1;
	m_dwRestartOffset = 0;
	m_bPassiveMode = FALSE;
}


BOOL CConnectSocket::SendResponse(LPCTSTR pstrFormat, ...)
{
	CString str;
	
	// ��ʽ������
	va_list args;
	va_start(args, pstrFormat);
	str.FormatV(pstrFormat, args);
	
	//�ж������Ƿ��ڻ״̬
	/*if (HasConnectionDropped())
	{
	FireStatusMessage("Could not send reply, disconnected.", 2);	
	
	  Close();
	  // ֪ͨ�߳��Ѿ�ֹͣ���ر�����
	  m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);
	  return FALSE;
}*/
	
	int nBytes = CSocket::Send(str + "\r\n", str.GetLength()+2);
	if (nBytes == SOCKET_ERROR)
	{
		Close();
		FireStatusMessage("Could not send reply, disconnected.", 2);	
		
		//֪ͨ�߳������Ѿ��ر�
		m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);
		
		return FALSE;
	}
	
	FireStatusMessage(str, 2);
	
   	//((CConnectThread *)AfxGetThread())->IncSentBytes(nBytes);
	return TRUE;
}

//Fire status message.	
//ͨ����SendResponse�����е���
//FireStatusMessage(str, 2);
//�����ļ���д����־������־��������ʾ���������û���������
void CConnectSocket::FireStatusMessage(LPCTSTR lpszStatus, int nType)
{
	CConnectThread *pThread = (CConnectThread *)m_pThread;
	((CFTPServer *)pThread->m_pWndServer)->AddTraceLine(nType, "[%d] %s", m_pThread->m_nThreadID, lpszStatus);
}

/********************************************************************/
/*																	*/
/* Function name : OnClose											*/		
/* Description   : Send WM_QUIT message to the thread containing	*/
/*				   the socket to shutdown once the connection is	*/
/*                 closed.											*/
/*																	*/
/********************************************************************/
void CConnectSocket::OnClose(int nErrorCode) 
{
	Close();
	// destroy connection
	m_pThread->PostThreadMessage(WM_THREADMSG, 1, 0);
	TRACE("CConnectSocket() OnClose()\n");

	CSocket::OnClose(nErrorCode);
}