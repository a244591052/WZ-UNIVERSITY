// ApplicationDlg.h : header file
//

#if !defined(AFX_APPLICATIONDLG_H__758971CE_7890_4761_9172_5BE4486E1569__INCLUDED_)
#define AFX_APPLICATIONDLG_H__758971CE_7890_4761_9172_5BE4486E1569__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CApplicationDlg dialog
#include "TracePage.h"
#include "OnlineUserPage.h"
#include "ConfigurationPage.h"
#include "StatisticsPage.h"
#include "FTPEventSink.h"

class CApplicationDlg : public CDialog , CFTPEventSink
{
// Construction
public:
	CApplicationDlg(CWnd* pParent = NULL);	// standard constructor

	CTracePage m_TracePage;
	COnlineUserPage m_OnlineUserPage;
	CConfigurationPage m_ConfigurationPage;
	CStatisticsPage m_StatisticsPage;

// Dialog Data
	//{{AFX_DATA(CApplicationDlg)
	enum { IDD = IDD_APPLICATION };
	CString	m_ip;
	int		m_port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApplicationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	void ActivatePage(int nIndex);//����nIndex�����Ի�������ʾָ��������ҳ
	void MoveChilds();//������ҳ���ڵĴ�С�����������ڵ�picture�ռ��С����Ӧ

	// Generated message map functions
	//{{AFX_MSG(CApplicationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnServerStart();
	afx_msg void OnServerStop();
	afx_msg void OnViewTracePage();
	afx_msg void OnViewOnlineUserPage();
	afx_msg void OnViewStatisticsPage();
    afx_msg void OnViewConfigurationsPage();
	afx_msg void OnUserAccounts();
	afx_msg void OnAbout();
	afx_msg void OnClose();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonUser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void OnFTPUserConnected(DWORD nThreadID, LPCTSTR lpszUser, LPCSTR lpszAddress);
	virtual void OnFTPUserDisconnected(DWORD nThreadID, LPCTSTR lpszUser);
	virtual void OnFTPStatusChange(int nType, LPCTSTR lpszText);
	virtual void OnFTPReceivedBytesChange(int nBytes);
	virtual void OnFTPSentBytesChange(int nBytes);
	virtual void OnFTPStatisticChange(int nType, int nValue);
	char* FormatSize(DWORD dwSizeLow, DWORD dwSizeHigh);

private:
	CToolBar m_wndToolBar;

	CImageList m_imageToolBar;
	CImageList m_imageToolBarDisable;
	CImageList m_imageToolBarHot;

	void InitToolBar();//������ʹ�����
	BOOL CreateToolbar();//��ʾ������
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APPLICATIONDLG_H__758971CE_7890_4761_9172_5BE4486E1569__INCLUDED_)
