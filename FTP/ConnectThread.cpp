// ConnectThread.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "ConnectThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectThread

IMPLEMENT_DYNCREATE(CConnectThread, CWinThread)

CConnectThread::CConnectThread()
{
}

CConnectThread::~CConnectThread()
{
}

BOOL CConnectThread::InitInstance()
{
	try
	{
		m_ConnectSocket.Attach(m_hSocket);
		m_ConnectSocket.m_pThread = this;
		
		CString strIPAddress;
		UINT nPort;
		m_ConnectSocket.GetPeerName(strIPAddress,nPort);
		
		m_pWndServer->SendMessage(WM_THREADSTART,(WPARAM)this,0);
		
		/*if(((CFTPServer *)m_pWndServer)->CheckMaxUsers())
		{
			m_ConnectSocket.SendResponse("421 Too many users are connected,please try again later.");
			PostThreadMessage(WM_QUIT,0,0);
		}
		else if(!((CFTPServer *)m_pWndServer)->IsIPAddressAllowed(strIPAddress))
		{
			m_ConnectSocket.SendResponse("421 Access denied,IP address was rejected by the server.");
			PostThreadMessage(WM_QUIT,0,0);
		}
		else
		{
			//m_ConnectSocket.SendResponse("220 %s",((CFTPServer *)m_pWndServer)->GetWelcomeMessage());
			//m_nTimerID = ::SetTimer(NULL,0,1000,TimerProc);
		}*/
		m_ConnectSocket.SendResponse("220 %s","welcome");//test!!!!
	}
	catch (CException *e)
	{
		e->Delete();
	}
	return TRUE;
}

int CConnectThread::ExitInstance()
{
	CFTPServer *pWnd = (CFTPServer*)m_pWndServer;
	try
	{
		pWnd->m_CriticalSection.Lock();
		//��������ɾ����ǰ�߳�
		POSITION pos = pWnd->m_ThreadList.Find(this);
		if(pos != NULL)
		{
			pWnd->m_ThreadList.RemoveAt(pos);
		}
		pWnd->m_CriticalSection.Unlock();
		//֪ͨ������ѭ��
		pWnd->SendMessage(WM_THREADCLOSE,(WPARAM)this,0);
	}
	catch (CException *e)
	{
		pWnd->m_CriticalSection.Unlock();
		e->Delete();
	}
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CConnectThread, CWinThread)
	//{{AFX_MSG_MAP(CConnectThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_THREADMSG, OnThreadMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectThread message handlers
//�Զ�����ϢWM_THREADMSG����Ӧ����
LRESULT CConnectThread::OnThreadMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case 0: // destroy data socket
			m_ConnectSocket.DestroyDataConnection();
			break;
		case 1: // quit !
			PostThreadMessage(WM_QUIT,0,0);//�ر��߳�
			break;
		default:
			break;
	}
	return 0L;
}
//PostThreadMessage��һ��Windows API�������书���ǽ�һ����Ϣ���루���ͣ���ָ���̵߳���Ϣ��������ȴ��̴߳�����Ϣ�ͷ��ء�

/********************************************************************/
/*																	*/
/* Function name : IncSentBytes										*/		
/* Description   : Increment number of bytes sent by the server.	*/
/*																	*/
/********************************************************************/
void CConnectThread::IncSentBytes(int nBytes)
{
	m_nSentBytes += nBytes;
	// notify server class
	m_pWndServer->PostMessage(WM_THREADMSG, (WPARAM)0, (LPARAM)nBytes);
}


/********************************************************************/
/*																	*/
/* Function name : IncReceivedBytes									*/		
/* Description   : Increment number of bytes received by the server.*/
/*																	*/
/********************************************************************/
void CConnectThread::IncReceivedBytes(int nBytes)
{
	m_nReceivedBytes += nBytes;
	// notify server class
	m_pWndServer->PostMessage(WM_THREADMSG, (WPARAM)1, (LPARAM)nBytes);
}

/********************************************************************/
/*																	*/
/* Function name : UpdateStatistic									*/	
/* Description   : Specific statistics has been changed.			*/
/*																	*/
/********************************************************************/
void CConnectThread::UpdateStatistic(int nType)
{
	// notify server class
	m_pWndServer->PostMessage(WM_THREADMSG, (WPARAM)2, (LPARAM)nType);
}