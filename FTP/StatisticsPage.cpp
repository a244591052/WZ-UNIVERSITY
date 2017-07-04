// StatisticsPage.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "StatisticsPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsPage dialog


CStatisticsPage::CStatisticsPage(CWnd* pParent /*=NULL*/)
	: CDialog(CStatisticsPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStatisticsPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsPage)
	DDX_Control(pDX, IDC_STATISTICS, m_StatisticsCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsPage, CDialog)
	//{{AFX_MSG_MAP(CStatisticsPage)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsPage message handlers

BOOL CStatisticsPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_StatisticsCtrl.InsertColumn(0, "ͳ��");		
	m_StatisticsCtrl.InsertColumn(1, "ֵ");	

	m_StatisticsCtrl.InsertItem(0, "�ܵ�������");
	m_StatisticsCtrl.SetItemText(0, 1, "0");
	m_StatisticsCtrl.InsertItem(1, "��ǰ������");
	m_StatisticsCtrl.SetItemText(1, 1, "0");
	m_StatisticsCtrl.InsertItem(2, "�ļ�����");
	m_StatisticsCtrl.SetItemText(2, 1, "0");
	m_StatisticsCtrl.InsertItem(3, "�ļ��ϴ�");
	m_StatisticsCtrl.SetItemText(3, 1, "0");
	m_StatisticsCtrl.InsertItem(4, "ʧ�ܵ�����");
	m_StatisticsCtrl.SetItemText(4, 1, "0");
	m_StatisticsCtrl.InsertItem(5, "ʧ�ܵ��ϴ�");
	m_StatisticsCtrl.SetItemText(5, 1, "0");
	m_StatisticsCtrl.InsertItem(6, "�յ����ֽ�����");
	m_StatisticsCtrl.SetItemText(6, 1, "0");
	m_StatisticsCtrl.InsertItem(7, "���͵��ֽ�����");
	m_StatisticsCtrl.SetItemText(7, 1, "0");
	
	DWORD dwStyle = m_StatisticsCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
    m_StatisticsCtrl.SetExtendedStyle(dwStyle);

	return TRUE;
}

void CStatisticsPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_STATISTICS)))
	{
		CRect rect;
		GetClientRect(rect);
		m_StatisticsCtrl.MoveWindow(rect);
		m_StatisticsCtrl.SetColumnWidth(0, rect.Width()/3-2);
		m_StatisticsCtrl.SetColumnWidth(1, rect.Width()/3-2);
		m_StatisticsCtrl.SetColumnWidth(2, rect.Width()/3-2);
	}	
}

void CStatisticsPage::SetValue(int nType, int nValue)
{
	CString strValue;
	strValue.Format("%d",nValue);
	m_StatisticsCtrl.SetItemText(nType,1,strValue);
}

void CStatisticsPage::SetValue(int nType, LPCTSTR lpszValue)
{
	m_StatisticsCtrl.SetItemText(nType,1,lpszValue);
}
