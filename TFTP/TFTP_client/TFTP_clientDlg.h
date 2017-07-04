// TFTP_clientDlg.h : header file
//

#if !defined(AFX_TFTP_CLIENTDLG_H__89EE3231_5B76_445D_8F6E_0740A69B5F41__INCLUDED_)
#define AFX_TFTP_CLIENTDLG_H__89EE3231_5B76_445D_8F6E_0740A69B5F41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTFTP_clientDlg dialog

class CTFTP_clientDlg : public CDialog
{
// Construction
public:
	CTFTP_clientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTFTP_clientDlg)
	enum { IDD = IDD_TFTP_CLIENT_DIALOG };
	CString	m_target_name;
	CString	m_local_name;
	CString	m_dest_ip;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTFTP_clientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTFTP_clientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Ondownload();
	afx_msg void Onupload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TFTP_CLIENTDLG_H__89EE3231_5B76_445D_8F6E_0740A69B5F41__INCLUDED_)
