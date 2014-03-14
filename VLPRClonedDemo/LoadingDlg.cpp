// LoadingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VLPRClonedDemo.h"
#include "LoadingDlg.h"


// CLoadingDlg 对话框

IMPLEMENT_DYNAMIC(CLoadingDlg, CDialog)

CLoadingDlg::CLoadingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadingDlg::IDD, pParent)
	, msg(_T(""))
{

	waitTime = 0;
}

CLoadingDlg::~CLoadingDlg()
{
}

void CLoadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, ID_MSG, msg);
}


BEGIN_MESSAGE_MAP(CLoadingDlg, CDialog)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CLoadingDlg 消息处理程序

void CLoadingDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1){
		this->OnOK();
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CLoadingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(waitTime>0)
		SetTimer(1, waitTime*1000, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
