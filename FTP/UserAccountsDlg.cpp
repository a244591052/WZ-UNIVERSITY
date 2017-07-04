// UserAccountsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ftpserver.h"
#include "UserAccountsDlg.h"

extern CFTPServer theServer;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserAccountsDlg dialog


CUserAccountsDlg::CUserAccountsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserAccountsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserAccountsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUserAccountsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserAccountsDlg)
	DDX_Control(pDX, IDC_USERSLIST, m_UsersList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserAccountsDlg, CDialog)
	//{{AFX_MSG_MAP(CUserAccountsDlg)
	ON_BN_CLICKED(IDC_BTNADDUSERACCOUNT, OnBtnadduseraccount)
	ON_BN_CLICKED(IDC_BTNEDITUSERACCOUNT, OnBtnedituseraccount)
	ON_BN_CLICKED(IDC_BTNEDELUSERACCOUNT, OnBtndeluseraccount)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserAccountsDlg message handlers
BOOL CUserAccountsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//List����ؼ��и��ط����������ˣ��Ǿ����ڱ�����ͼ��ѡ��һ��ʱ����ֻ��������ߵ�һ����Ŀ��
	//��List�ؼ�����һ��LVM_SETEXTENDEDLISTVIEWSTYLE��Ϣ���Խ���������
	::SendMessage(m_UsersList.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT); 

   	m_UsersList.InsertColumn(0, "�û���",  LVCFMT_CENTER, 70);
	m_UsersList.InsertColumn(1, "����·��", LVCFMT_CENTER, 100);
	m_UsersList.InsertColumn(2, "����", LVCFMT_CENTER, 40);
	m_UsersList.InsertColumn(3, "�ϴ�", LVCFMT_CENTER, 40);
	m_UsersList.InsertColumn(4, "����", LVCFMT_CENTER, 40);
	m_UsersList.InsertColumn(5, "ɾ��", LVCFMT_CENTER, 40);
	m_UsersList.InsertColumn(6, "����Ŀ¼", LVCFMT_CENTER, 60);	

	m_UsersArray.RemoveAll();//��һ��ȡ���û��б�ʱҪ��գ�����ͻ����������ǰ�����ݵĻ������������
	theServer.m_UserManager.GetUserAccountList(m_UsersArray);

	for (int i=0; i < m_UsersArray.GetSize(); i++)
	{
		int nItem = m_UsersList.InsertItem(i, m_UsersArray[i].m_strName);

		m_UsersList.SetItemText(nItem, 1, m_UsersArray[i].m_strDir);
		m_UsersList.SetItemText(nItem, 2, m_UsersArray[i].m_bAllowDownload ? "��" : "��");
		m_UsersList.SetItemText(nItem, 3, m_UsersArray[i].m_bAllowUpload ? "��" : "��");
		m_UsersList.SetItemText(nItem, 4, m_UsersArray[i].m_bAllowRename ? "��" : "��");
		m_UsersList.SetItemText(nItem, 5, m_UsersArray[i].m_bAllowDelete ? "��" : "��");
		m_UsersList.SetItemText(nItem, 6, m_UsersArray[i].m_bAllowCreateDirectory ? "��" : "��");

		m_UsersList.SetItemData(nItem, i);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserAccountsDlg::OnBtnadduseraccount() 
{
	CAddUserInfoDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		for (int i=0; i<m_UsersList.GetItemCount(); i++)
		{
			CString strName;
			strName = m_UsersList.GetItemText(i, 0);
			if (strName.CompareNoCase(dlg.m_strUserName) == 0)
			{
				AfxMessageBox("Sorry, this user already exists!");
				return;
			}
		}

		CUserAccount useraccount;

		useraccount.m_strName = dlg.m_strUserName;
		useraccount.m_strPassword = dlg.m_strUserPass;
		useraccount.m_strDir = dlg.m_strPath;
		useraccount.m_bAllowDownload = dlg.m_bAllowDownload;
		useraccount.m_bAllowUpload = dlg.m_bAllowUpload;
		useraccount.m_bAllowRename = dlg.m_bAllowRename;
		useraccount.m_bAllowDelete = dlg.m_bAllowDelete;
		useraccount.m_bAllowCreateDirectory = dlg.m_bAllowCreateDirectory;

		int index = m_UsersArray.Add(useraccount);

		int nItem = m_UsersList.InsertItem(0,useraccount.m_strName);
		m_UsersList.SetItemText(nItem, 1, useraccount.m_strDir);
		m_UsersList.SetItemText(nItem, 2, useraccount.m_bAllowDownload ? "Y" : "N");
		m_UsersList.SetItemText(nItem, 3, useraccount.m_bAllowUpload ? "Y" : "N");
		m_UsersList.SetItemText(nItem, 4, useraccount.m_bAllowRename ? "Y" : "N");
		m_UsersList.SetItemText(nItem, 5, useraccount.m_bAllowDelete ? "Y" : "N");
		m_UsersList.SetItemText(nItem, 6, useraccount.m_bAllowCreateDirectory ? "Y" : "N");
		
		m_UsersList.SetItemData(nItem, index);//������ݵ����ú���Ҫ���Ժ�Ѱ���б��ϵ����ݶ�Ӧ�����ݽṹ��ʱҪ�õ�

		m_UsersList.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CUserAccountsDlg::OnOK() 
{
	theServer.m_UserManager.UpdateUserAccountList(m_UsersArray);
	//m_UsersArray.RemoveAll();
	CDialog::OnOK();
}

void CUserAccountsDlg::OnBtnedituseraccount() 
{
		//ȡ��ѡ����û�
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nSelIndex == -1)
        return;

	int nUserIndex = m_UsersList.GetItemData(nSelIndex);
	
	CAddUserInfoDlg dlg;

	dlg.m_strTitle = "�޸��û�";

	dlg.m_strUserName = m_UsersArray[nUserIndex].m_strName;
	dlg.m_strUserPass = m_UsersArray[nUserIndex].m_strPassword;
	dlg.m_strUserPassAgain = m_UsersArray[nUserIndex].m_strPassword;
	dlg.m_strPath = m_UsersArray[nUserIndex].m_strDir;
	dlg.m_bAllowCreateDirectory = m_UsersArray[nUserIndex].m_bAllowCreateDirectory;
	dlg.m_bAllowDelete = m_UsersArray[nUserIndex].m_bAllowDelete;
	dlg.m_bAllowDownload = m_UsersArray[nUserIndex].m_bAllowDownload;
	dlg.m_bAllowRename = m_UsersArray[nUserIndex].m_bAllowRename;
	dlg.m_bAllowUpload = m_UsersArray[nUserIndex].m_bAllowUpload;


	if (dlg.DoModal() == IDOK)
	{
		m_UsersArray[nUserIndex].m_strName = dlg.m_strUserName;
		m_UsersArray[nUserIndex].m_strPassword = dlg.m_strUserPass;
		m_UsersArray[nUserIndex].m_strDir = dlg.m_strPath;
		m_UsersArray[nUserIndex].m_bAllowCreateDirectory = dlg.m_bAllowCreateDirectory;
		m_UsersArray[nUserIndex].m_bAllowDelete = dlg.m_bAllowDelete;
		m_UsersArray[nUserIndex].m_bAllowDownload = dlg.m_bAllowDownload;
		m_UsersArray[nUserIndex].m_bAllowRename = dlg.m_bAllowRename;
		m_UsersArray[nUserIndex].m_bAllowUpload = dlg.m_bAllowUpload;

		m_UsersList.SetItemText(nUserIndex, 0, dlg.m_strUserName);
		m_UsersList.SetItemText(nUserIndex, 1, dlg.m_strPath);
		m_UsersList.SetItemText(nUserIndex, 2, dlg.m_bAllowDownload ? "Y" : "N");
		m_UsersList.SetItemText(nUserIndex, 3, dlg.m_bAllowUpload ? "Y" : "N");
		m_UsersList.SetItemText(nUserIndex, 4, dlg.m_bAllowRename ? "Y" : "N");
		m_UsersList.SetItemText(nUserIndex, 5, dlg.m_bAllowDelete ? "Y" : "N");
		m_UsersList.SetItemText(nUserIndex, 6, dlg.m_bAllowCreateDirectory ? "Y" : "N");
	}
	
}

void CUserAccountsDlg::OnBtndeluseraccount() 
{
	int nSelIndex = m_UsersList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    if(nSelIndex == -1)
        return;

    // get index of selected user item
	int nUserIndex = m_UsersList.GetItemData(nSelIndex);
    
	
	// delete item from list
	m_UsersList.DeleteItem(nUserIndex);

	m_UsersArray.RemoveAt(nUserIndex);

	for (int i=0; i < m_UsersList.GetItemCount(); i++)
	{
		int nItemData = m_UsersList.GetItemData(i);
		if (nItemData > nUserIndex)
		{
			m_UsersList.SetItemData(i, nItemData-1);
		}
	}
	
}

void CUserAccountsDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
