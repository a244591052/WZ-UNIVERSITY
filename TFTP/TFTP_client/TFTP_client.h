// TFTP_client.h : main header file for the TFTP_CLIENT application
//

#if !defined(AFX_TFTP_CLIENT_H__7AF644D8_0FDB_44B9_9CBF_FB9D714658BE__INCLUDED_)
#define AFX_TFTP_CLIENT_H__7AF644D8_0FDB_44B9_9CBF_FB9D714658BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTFTP_clientApp:
// See TFTP_client.cpp for the implementation of this class
//

class CTFTP_clientApp : public CWinApp
{
public:
	CTFTP_clientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTFTP_clientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTFTP_clientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TFTP_CLIENT_H__7AF644D8_0FDB_44B9_9CBF_FB9D714658BE__INCLUDED_)
