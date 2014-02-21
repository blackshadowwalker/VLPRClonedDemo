
// VLPRClonedDemoDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

//=====  STL ==============
#include <queue>
#include <iostream>
#include <list>
using namespace std;
//=========================

#include "TH_PlateID.h"
#include "public.h"


typedef struct LPR_Image
{
	unsigned char* buffer;
	int  imageWidth;
	int  imageHeight;
	int  imageSize;
	LPR_Image(){
		memset(this, 0, sizeof(LPR_Image));
	}
}LPR_Image;


// CVLPRClonedDemoDlg 对话框
class CVLPRClonedDemoDlg : public CDialog
{
// 构造
public:
	CVLPRClonedDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VLPRCLONEDDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

	CListBox m_listDirs;
	afx_msg void OnBnClickedAddBrowser();
	afx_msg void OnBnClickedDirsClear();

public:
	TH_PlateIDCfg plateConfigTh;

	list<char *> mListPicturesPath;
	bool TH_InitDll(int bMovingImage);
	char pLocalChinese[3];    //本地汉字字符，如果此字符设置为空或者31个省份之外的字，则不使用首汉字

	afx_msg void OnBnClickedAnay();

	list<HANDLE> EventList;
	HANDLE ReginsterMyThread(char *name);

	clock_t timeStart, timeNow;

	queue<LPR_Result*> LPRQueueResult;
	queue<LPR_Image*> imagesQueue;
	queue<LPR_Image*> imagesQueuePlay;
	unsigned char*  imageDataForShow;
	int				imageDataForShowSize;

	void LoadImageFromPath(char * path);
	void startThreads();
};
