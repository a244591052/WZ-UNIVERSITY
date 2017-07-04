#if !defined(AFX_FTPSERVER_H__FC164D4C_4EA3_448B_A911_D952FB5E2EB3__INCLUDED_)
#define AFX_FTPSERVER_H__FC164D4C_4EA3_448B_A911_D952FB5E2EB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FTPServer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFTPServer window
#include "ConnectThread.h"
#include "ListenSocket.h"
#include "UserManager.h"	// Added by ClassView
#include "FTPEventSink.h"

class CFTPServer : public CWnd
{
// Construction
public:
	CFTPServer();

	void SetGoodbyeMessage(LPCTSTR lpszText);
	void SetWelcomeMessage(LPCTSTR lpszText);
	void SetTimeout(int nValue);
	void SetPort(int nValue);
	//void SetMaxUsers(int nValue);

	//BOOL CheckMaxUsers();

// Attributes
public:
 	CTypedPtrList<CObList, CConnectThread*> m_ThreadList;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFTPServer)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Initialize(CFTPEventSink *pEventSink);
	void AddTraceLine(int nType, LPCTSTR pstrFormat, ...);
	CUserManager m_UserManager;
	CCriticalSection m_CriticalSection;
	BOOL Start();
	void Stop();
	virtual ~CFTPServer();
	CFTPEventSink * m_pEventSink; //����ֻ�������ﶨ����һ�����еı���

    void SetStatisticsInterval(int nValue);//���ö೤ʱ��ȥ�ı�һ��ͳ��ҳ������
	
	int GetPort() { return m_nPort;	};
	//int GetMaxUsers() {	return m_nMaxUsers;	}
	int GetTimeout() { return m_nTimeout; }
	int GetConnectionCount() { return m_nConnectionCount; }
	CString GetWelcomeMessage() { return m_strWelcomeMessage; };
	CString GetGoodbyeMessage() { return m_strGoodbyeMessage; };

	// Generated message map functions
protected:
	int m_nStatisticsInterval;
	//{{AFX_MSG(CFTPServer)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	LRESULT OnThreadClose(WPARAM wParam, LPARAM lParam);
	LRESULT OnThreadStart(WPARAM wParam, LPARAM);
	LRESULT OnThreadMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	//CFTPEventSink * m_pEventSink;//����������˽�б������ǳ�����֪��Ϊ�Σ����Ǳ���˵���ܷ���CFTPServer���е�˽�б�����
	BOOL m_bRunning;
	CListenSocket m_ListenSocket;

	//��������
	int		m_nPort;
	// int		m_nMaxUsers;
	CString	m_strWelcomeMessage;
	CString	m_strGoodbyeMessage;
	int		m_nTimeout;

	//ͳ������
	DWORD m_dwTotalReceivedBytes;
	DWORD m_dwTotalSentBytes;
	int	m_nConnectionCount;
	int m_nTotalConnections;
	int m_nFilesDownloaded;
	int m_nFilesUploaded;
	int m_nFailedDownloads;
	int m_nFailedUploads;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPSERVER_H__FC164D4C_4EA3_448B_A911_D952FB5E2EB3__INCLUDED_)
