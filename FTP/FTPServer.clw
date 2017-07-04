; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CApplicationDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ftpserver.h"
LastPage=0

ClassCount=15
Class1=CAddUserInfoDlg
Class2=CAboutDlg
Class3=CApplicationDlg
Class4=CColorListBox
Class5=CConfigurationPage
Class6=CConnectSocket
Class7=CConnectThread
Class8=CDataSocket
Class9=CFTPServer
Class10=CFTPServerApp
Class11=CListenSocket
Class12=COnlineUserPage
Class13=CStatisticsPage
Class14=CTracePage
Class15=CUserAccountsDlg

ResourceCount=11
Resource1=IDD_ADD_USERINFO
Resource2=IDD_ABOUT
Resource3=IDD_PROPPAGE_CONFIGURATION
Resource4=IDR_TRACE_MENU
Resource5=IDD_APPLICATION
Resource6=IDD_PROPPAGE_STATISTICS
Resource7=IDR_ONLINE_MENU
Resource8=IDD_USERACCOUNTS
Resource9=IDD_PROPPAGE_ONLINE
Resource10=IDD_PROPPAGE_TRACE
Resource11=IDR_TOOLBAR (English (U.S.))

[CLS:CAddUserInfoDlg]
Type=0
BaseClass=CDialog
HeaderFile=AddUserInfoDlg.h
ImplementationFile=AddUserInfoDlg.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ApplicationDlg.cpp
ImplementationFile=ApplicationDlg.cpp

[CLS:CApplicationDlg]
Type=0
BaseClass=CDialog
HeaderFile=ApplicationDlg.h
ImplementationFile=ApplicationDlg.cpp
LastObject=CApplicationDlg
Filter=D
VirtualFilter=dWC

[CLS:CColorListBox]
Type=0
BaseClass=CListBox
HeaderFile=ColorListBox.h
ImplementationFile=ColorListBox.cpp

[CLS:CConfigurationPage]
Type=0
BaseClass=CDialog
HeaderFile=ConfigurationPage.h
ImplementationFile=ConfigurationPage.cpp
Filter=D
VirtualFilter=dWC
LastObject=CConfigurationPage

[CLS:CConnectSocket]
Type=0
BaseClass=CSocket
HeaderFile=ConnectSocket.h
ImplementationFile=ConnectSocket.cpp

[CLS:CConnectThread]
Type=0
BaseClass=CWinThread
HeaderFile=ConnectThread.h
ImplementationFile=ConnectThread.cpp

[CLS:CDataSocket]
Type=0
BaseClass=CAsyncSocket
HeaderFile=DataSocket.h
ImplementationFile=DataSocket.cpp

[CLS:CFTPServer]
Type=0
BaseClass=CWnd
HeaderFile=FTPServer.h
ImplementationFile=FTPServer.cpp

[CLS:CFTPServerApp]
Type=0
BaseClass=CWinApp
HeaderFile=FTPServerApp.h
ImplementationFile=FTPServerApp.cpp

[CLS:CListenSocket]
Type=0
BaseClass=CAsyncSocket
HeaderFile=ListenSocket.h
ImplementationFile=ListenSocket.cpp

[CLS:COnlineUserPage]
Type=0
BaseClass=CDialog
HeaderFile=OnlineUserPage.h
ImplementationFile=OnlineUserPage.cpp

[CLS:CStatisticsPage]
Type=0
BaseClass=CDialog
HeaderFile=StatisticsPage.h
ImplementationFile=StatisticsPage.cpp
LastObject=CStatisticsPage

[CLS:CTracePage]
Type=0
BaseClass=CDialog
HeaderFile=TracePage.h
ImplementationFile=TracePage.cpp

[CLS:CUserAccountsDlg]
Type=0
BaseClass=CDialog
HeaderFile=UserAccountsDlg.h
ImplementationFile=UserAccountsDlg.cpp

[DLG:IDD_ADD_USERINFO]
Type=1
Class=CAddUserInfoDlg
ControlCount=17
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_PATH,edit,1350631552
Control5=IDC_STATIC,button,1342177287
Control6=IDC_BROWSE,button,1342242816
Control7=IDC_DOWNLOAD,button,1342242819
Control8=IDC_UPLOAD,button,1342242819
Control9=IDC_RENAME,button,1342242819
Control10=IDC_DELETE,button,1342242819
Control11=IDC_CREATE_DIR,button,1342242819
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_USERNAME,edit,1350631552
Control15=IDC_EDIT_USERPASS,edit,1350631584
Control16=IDC_STATIC,static,1342308352
Control17=IDC_EDIT_USERPASSAGAIN,edit,1350631584

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_APPLICATION]
Type=1
Class=CApplicationDlg
ControlCount=9
Control1=IDC_DIALOG_AREA,static,1073741832
Control2=IDC_BUTTON_START,button,1342242816
Control3=IDC_BUTTON_STOP,button,1342242816
Control4=IDC_EDIT_SERVER_PORT,edit,1350631552
Control5=IDC_EDIT_SERVER_IP,edit,1350631552
Control6=IDC_BUTTON_USER,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_PROPPAGE_ONLINE]
Type=1
Class=COnlineUserPage
ControlCount=1
Control1=IDC_ONLINE_USERS,SysListView32,1350631433

[DLG:IDD_PROPPAGE_STATISTICS]
Type=1
Class=CStatisticsPage
ControlCount=1
Control1=IDC_STATISTICS,SysListView32,1350631437

[DLG:IDD_PROPPAGE_TRACE]
Type=1
Class=CTracePage
ControlCount=1
Control1=IDC_TRACE,listbox,1352728929

[DLG:IDD_USERACCOUNTS]
Type=1
Class=CUserAccountsDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_USERSLIST,SysListView32,1350631445
Control4=IDC_BTNADDUSERACCOUNT,button,1342242816
Control5=IDC_BTNEDITUSERACCOUNT,button,1342242816
Control6=IDC_BTNEDELUSERACCOUNT,button,1342242816

[DLG:IDD_ABOUT]
Type=1
Class=?
ControlCount=0

[MNU:IDR_TRACE_MENU]
Type=1
Class=?
Command1=ID_TRACE_CLEAR
CommandCount=1

[MNU:IDR_ONLINE_MENU]
Type=1
Class=?
Command1=IDC_KICK_USER
Command2=IDC_EDIT_USER
CommandCount=2

[TB:IDR_TOOLBAR (English (U.S.))]
Type=1
Class=?
Command1=ID_SERVER_START
Command2=ID_SERVER_STOP
Command3=ID_TRACEPAGE
Command4=ID_VIEW_CONFIGURATION
Command5=ID_USER_ACCOUNTS
CommandCount=5

[DLG:IDD_PROPPAGE_CONFIGURATION]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PORT,edit,1350639744
Control3=IDC_APPLY,button,1342242816

