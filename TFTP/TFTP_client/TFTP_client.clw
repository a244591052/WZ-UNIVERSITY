; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTFTP_clientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TFTP_client.h"

ClassCount=3
Class1=CTFTP_clientApp
Class2=CTFTP_clientDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TFTP_CLIENT_DIALOG

[CLS:CTFTP_clientApp]
Type=0
HeaderFile=TFTP_client.h
ImplementationFile=TFTP_client.cpp
Filter=N

[CLS:CTFTP_clientDlg]
Type=0
HeaderFile=TFTP_clientDlg.h
ImplementationFile=TFTP_clientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CTFTP_clientDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=TFTP_clientDlg.h
ImplementationFile=TFTP_clientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TFTP_CLIENT_DIALOG]
Type=1
Class=CTFTP_clientDlg
ControlCount=8
Control1=IDC_STATIC,static,1342308352
Control2=IDC_IP,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_target_name,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_local_name,edit,1350631552
Control7=IDC_download,button,1342242816
Control8=IDC_upload,button,1342242816

