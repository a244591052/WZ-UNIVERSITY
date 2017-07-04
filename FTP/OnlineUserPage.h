#if !defined(AFX_ONLINEUSERPAGE_H__E1CBB9C2_29DD_4AF0_969B_00EA201EC4B2__INCLUDED_)
#define AFX_ONLINEUSERPAGE_H__E1CBB9C2_29DD_4AF0_969B_00EA201EC4B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OnlineUserPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COnlineUserPage dialog
#include "resource.h"

class COnlineUserPage : public CDialog
{
// Construction
public:
	void RemoveUser(DWORD nThreadID);
	void AddUser(DWORD ThreadID,LPCTSTR lpszName,LPCTSTR lpszAddress);
	COnlineUserPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COnlineUserPage)
	enum { IDD = IDD_PROPPAGE_ONLINE };
	CListCtrl	m_OnlineUsers;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnlineUserPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//ע���������ҳ���ڴ�С�仯����Ϣ��Ӧ������ON_SIZE������Ȼ���������е������ڴ�С��Ӧ��picture�ؼ��Ĵ�Сʱ
	//����ִ��ڴ�Сû�б仯��
	// Generated message map functions
	//{{AFX_MSG(COnlineUserPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);   //û��������ڴ�С�仯����Ϣ��Ӧ��������ҳ��С���ܱ仯������ListView�ؼ��ı�����б���Ҳ����ֱ����ʾ����Ҫ������
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKickUser();
	afx_msg void OnEditUser();
	afx_msg void OnBlockIp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONLINEUSERPAGE_H__E1CBB9C2_29DD_4AF0_969B_00EA201EC4B2__INCLUDED_)
