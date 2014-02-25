// LoadingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VLPRClonedDemo.h"
#include "LoadingDlg.h"


// CLoadingDlg 对话框

IMPLEMENT_DYNAMIC(CLoadingDlg, CDialog)

CLoadingDlg::CLoadingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadingDlg::IDD, pParent)
{

}

CLoadingDlg::~CLoadingDlg()
{
}

void CLoadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoadingDlg, CDialog)
END_MESSAGE_MAP()


// CLoadingDlg 消息处理程序
