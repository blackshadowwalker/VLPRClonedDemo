
// VLPRClonedDemoDlg.cpp : 实现文件

/**
* Auth: Karl
* Date: 2014/2/20
* LastUpdate: 2014/2/24
*/

#include "stdafx.h"
#include "VLPRClonedDemo.h"
#include "VLPRClonedDemoDlg.h"

#include "FileUtil.h"
#include "VideoUtil.h"


#pragma comment(lib, "TH_PLATEID.lib")


long  nFrames=0;

//====================================================================
#define WIDTHSTEP(pixels_width)  (((pixels_width) * 24/8 +3) / 4 *4)


HANDLE handleExit = 0;
HANDLE handleVideoThread=0;
HANDLE handleVideoThreadStoped=0;
HANDLE handleLPRThreadStoped=0;
HANDLE hShowVideoFrame=0;
HANDLE hShowFrameAccess=0;

//====================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVLPRClonedDemoDlg 对话框




CVLPRClonedDemoDlg::CVLPRClonedDemoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CVLPRClonedDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(pLocalChinese, 0, 3);
	imageDataForShow = 0;
	imageDataForShowSize = 0;

	handleExit = CreateEvent(NULL, FALSE, FALSE, NULL);
	handleVideoThread = CreateEvent(NULL, FALSE, FALSE, NULL);
	handleVideoThreadStoped = CreateEvent(NULL, FALSE, FALSE, NULL);
	hShowVideoFrame  = CreateEvent(NULL, FALSE, FALSE, NULL);
	handleLPRThreadStoped = CreateEvent(NULL, FALSE, FALSE, NULL);
	hShowFrameAccess  = CreateEvent(NULL, FALSE, FALSE, NULL);

}

void CVLPRClonedDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DIRS, m_listDirs);
	DDX_Control(pDX, IDC_LIST, m_list);
}

BEGIN_MESSAGE_MAP(CVLPRClonedDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_BN_CLICKED(BT_ADD_BROWSER, &CVLPRClonedDemoDlg::OnBnClickedAddBrowser)
	ON_BN_CLICKED(BT_DIRS_CLEAR, &CVLPRClonedDemoDlg::OnBnClickedDirsClear)
	ON_BN_CLICKED(BT_ANAY, &CVLPRClonedDemoDlg::OnBnClickedAnay)
	ON_LBN_DBLCLK(IDC_LIST_DIRS, &CVLPRClonedDemoDlg::OnLbnDblclkListDirs)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CVLPRClonedDemoDlg::OnNMClickList)
	ON_WM_MOUSEMOVE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CVLPRClonedDemoDlg 消息处理程序

BOOL CVLPRClonedDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_list.SetExtendedStyle(dwStyle); //设置扩展风格

	int cols = 0;
	m_list.InsertColumn( cols++, "车牌", LVCFMT_LEFT, 80 );
	m_list.InsertColumn( cols++, "第1时间", LVCFMT_LEFT, 150 );
	m_list.InsertColumn( cols++, "第2时间", LVCFMT_LEFT, 150 );
	m_list.InsertColumn( cols++, "时间差(分钟)", LVCFMT_LEFT, 120 );
	m_list.InsertColumn( cols++, "图片1", LVCFMT_LEFT, 50 );
	m_list.InsertColumn( cols++, "图片2", LVCFMT_LEFT, 50 );

	startThreads();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVLPRClonedDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVLPRClonedDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVLPRClonedDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CVLPRClonedDemoDlg::OnDropFiles(HDROP hDropInfo)
{
	int iFileCount = ::DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	char file_name[MAX_PATH];
	CString strBuffer = "";
	bool bSame=false;
	for(int i=0; i<iFileCount; i++){
		::DragQueryFile(hDropInfo, i, file_name, MAX_PATH);
		if(strlen(file_name)<2)
			continue;
		bSame=false;
		for(int j=0; j<m_listDirs.GetCount(); j++){
			m_listDirs.GetText(j, strBuffer);
			if(strBuffer.Compare(file_name)==0)
				bSame = true;
		}
		if( bSame==false && FileUtil::FindFirstFileExists(file_name, FILE_ATTRIBUTE_DIRECTORY)){
			m_listDirs.AddString(file_name);
		}
	}
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

void CVLPRClonedDemoDlg::OnBnClickedAddBrowser()
{
	UpdateData(true);
	char *path = FileUtil::SelectFolder(this->m_hWnd, "选择文件夹");
	if(path==0)
		return ;
	if(strlen(path)<2)
		return;
	bool bSame=false;
	CString strBuffer;
	for(int j=0; j<m_listDirs.GetCount(); j++){
		m_listDirs.GetText(j, strBuffer);
		if(strBuffer.Compare(path)==0)
			bSame = true;
	}
	if( bSame==false && FileUtil::FindFirstFileExists(path, FILE_ATTRIBUTE_DIRECTORY)){
		m_listDirs.AddString(path);
	}
	UpdateData(false);
}

void CVLPRClonedDemoDlg::OnBnClickedDirsClear()
{
	m_listDirs.ResetContent();
	m_list.DeleteAllItems();
}


//定义第一路识别参数。mem1 和 mem2 内存分配大小将在下一节给出参考值。 
static unsigned char mem1[0x4000]; 
static unsigned char mem2[100<<20];//60M 

bool CVLPRClonedDemoDlg::TH_InitDll(int bMovingImage)
{	
	int r = TH_UninitPlateIDSDK(&plateConfigTh);
	//plateConfigTh = c_defConfig;

	if( imageDataForShow == 0){
		imageDataForShow =  new unsigned char[3000 * 3000 * 3];
		imageDataForShowSize = 3000 * 3000 * 3;
	}

	//设置第一路识别参数 
	plateConfigTh.nMinPlateWidth = 60; 
	plateConfigTh.nMaxPlateWidth = 400; 
	plateConfigTh.nMaxImageWidth = 3000; 
	plateConfigTh.nMaxImageHeight = 3000; 
	plateConfigTh.bVertCompress = 0; 
	plateConfigTh.bIsFieldImage = 0; 
	plateConfigTh.bOutputSingleFrame = 1; 
	plateConfigTh.bMovingImage = bMovingImage;   //视频方式设为 1 
	plateConfigTh.pFastMemory=mem1; 
	plateConfigTh.nFastMemorySize=0x4000; 
	plateConfigTh.pMemory=mem2; 
	plateConfigTh.nMemorySize = 100<<20 ; 

	char error[512]={0};
	int n = TH_InitPlateIDSDK( &plateConfigTh ); 
	if(n!=0){
		sprintf(error, "%s,启动分析失败",  pErrorInfo[n]);
		MessageBox(error);
	}
	int m =TH_SetEnabledPlateFormat(PARAM_TWOROWYELLOW_ON, &plateConfigTh); 
	int k = TH_SetImageFormat(ImageFormatBGR, FALSE, FALSE, &plateConfigTh); 
	char m_LocalProvince[10] = {0}; 
	sprintf(m_LocalProvince, "%s", pLocalChinese);
	int l = TH_SetProvinceOrder(m_LocalProvince, &plateConfigTh); 
	int logo =TH_SetEnableCarLogo(true, &plateConfigTh); 

	if (n!=0||m!=0||k!=0||l!=0 | logo!=0) 
		return false; 

	return true;
}

int filesCount = 0;//文件总量
int curFileIndex = 0;//当前访问文件序号



BITMAPINFO *GetBitMapInfo(int width, int height, int bitCount=24){
	BITMAPINFO *m_bmphdr=0;  
	DWORD dwBmpHdr = sizeof(BITMAPINFO);  
	m_bmphdr = new BITMAPINFO[dwBmpHdr];  
	m_bmphdr->bmiHeader.biBitCount = 24;  
	m_bmphdr->bmiHeader.biClrImportant = 0;  
	m_bmphdr->bmiHeader.biSize = dwBmpHdr;  
	m_bmphdr->bmiHeader.biSizeImage = 0;  
	m_bmphdr->bmiHeader.biWidth = width;  
	m_bmphdr->bmiHeader.biHeight = height;  
	m_bmphdr->bmiHeader.biXPelsPerMeter = 0;  
	m_bmphdr->bmiHeader.biYPelsPerMeter = 0;  
	m_bmphdr->bmiHeader.biClrUsed = 0;  
	m_bmphdr->bmiHeader.biPlanes = 1;  
	m_bmphdr->bmiHeader.biCompression = BI_RGB;  
	return m_bmphdr;
}
//播放视频
int  PlayVideo(unsigned char *pix, int PICTURE_ID, CWnd *cWnd, int imageWidth, int imageHeight, bool revert=true)
{
	BITMAPINFO *m_bmphdr = GetBitMapInfo(imageWidth, imageHeight );

	CRect rc;
	cWnd->GetDlgItem(PICTURE_ID)->GetWindowRect(&rc);

	HDC hdc = cWnd->GetDlgItem(PICTURE_ID)->GetDC()->m_hDC;
	SetStretchBltMode(hdc, STRETCH_HALFTONE); //必加，StretchBlt, StretchDIBits可以对图像数据进行拉伸, 压缩显示失真

	int nResult = 0;
	if(revert){
		nResult = ::StretchDIBits( hdc,  
			0, 0, rc.Width(), rc.Height(), 
			0, imageHeight,  imageWidth, -imageHeight,  
			pix, m_bmphdr,  DIB_RGB_COLORS,  SRCCOPY);  
	}else{
		nResult = ::StretchDIBits( hdc,  
			0, 0, rc.Width(), rc.Height(), 
			0, 0,  imageWidth, imageHeight,  
			pix, m_bmphdr,  DIB_RGB_COLORS,  SRCCOPY);  
	}
	return nResult;
}

#include "LPRDB.h"


//根据图片名称获取图片的时间信息，FORMAT eg: 20140104150218_location.jpg
long getLastUpdateTime(char *filePath, char *lastUpdateTime)
{
	char fileName[256]={0};
	long lTime=0;
	sprintf( fileName, "%s", (strrchr(filePath,'\\')+1) );
	//fileName	0x0aa8f6f8 "20140224152924_京W56Y22_location.bmp"	char [256]
	char *p = strstr(fileName, "_");
	if( p == NULL){
		time_t timer = time(0);
		struct tm *tblock;
		tblock = localtime(&timer);	
		sprintf(lastUpdateTime , "%04d%02d%02d%02d%02d%02d", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec );
		return timer;
	}
	char szDateTime[64]={0};
	if((p - fileName) != 14)
	{
		debug(__FUNCTION__"(%s): %s get datetime error." , __LINE__, fileName);
		return -1;
	}
	int len = p-fileName;
	memcpy(szDateTime, fileName, len);
	struct tm t; 
	t.tm_sec	= atoi(szDateTime+12); szDateTime[12]='\0';
	t.tm_min	= atoi(szDateTime+10); szDateTime[10]='\0';
	t.tm_hour	= atoi(szDateTime+8); szDateTime[8]='\0';
	t.tm_mday		= atoi(szDateTime+6); szDateTime[6]='\0';
	t.tm_mon	= atoi(szDateTime+4); szDateTime[4]='\0';
	t.tm_year	= atoi(szDateTime)- 1900; szDateTime[0]='\0';

	lTime = mktime(&t);
	if( lTime<1 || t.tm_year < 1 || t.tm_mon < 1 || t.tm_mday < 1)
	{
		debug(__FUNCTION__"(%s): %s comvert datetime error." , __LINE__, fileName);
		return -1;
	}
	memcpy(lastUpdateTime, fileName, len);



	return lTime;

}
#include "time.h"

//处理识别结果线程
void ProcessResultThread(void *pParam)
{
	char filename[256]={0};
	CVLPRClonedDemoDlg *dlg = (CVLPRClonedDemoDlg*)pParam;
	HANDLE handleCanExit = dlg->ReginsterMyThread("ProcessResultThread");

	char temp[256]={0};
	unsigned char *pBit = 0;
	debug("ProcessResultThread 启动  handle=0x%x", handleCanExit);

	while(WaitForSingleObject(handleExit,0)!=WAIT_OBJECT_0){
		if(dlg->LPRQueueResult.size()<1){
			Sleep(10);		
			continue;
		}
		LPR_Result *result = dlg->LPRQueueResult.front();
		dlg->LPRQueueResult.pop();
		if(result==0)
			continue;

		release("ProcessResultThread Frame=%d  Plate=%s  (%d,%d)-(%d,%d)", nFrames, result->plate, \
			result->plateRect.left, result->plateRect.top,
			result->plateRect.right, result->plateRect.bottom);

		if( strlen(result->resultPicture) > 5){
			result->time = getLastUpdateTime(result->resultPicture, result->lastUpdateTime);//根据图片名称获取图片的时间信息，FORMAT eg: 2014-1-4_15.02.18_location.jpg

		}else{
			time_t timer;
			timer = time(NULL);
			struct tm *tblock;
			tblock = localtime(&timer);
			result->time = 	timer; //写入时间
			sprintf(temp,"%d/%d/%d %d:%d:%d", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec );
			sprintf(result->lastUpdateTime, "%s", temp);//写入时间

			//20140224152924_location.bmp
			sprintf(temp,"%04d%02d%02d%02d%02d%02d", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec );
			sprintf(filename, "%s\\%s_location_%s.bmp", dlg->m_imageDir, temp, result->plate);

			VideoUtil::write24BitBmpFile(filename, result->imageWidth, result->imageHeight,(unsigned char*)pBit,  WIDTHSTEP(result->imageWidth));//抓拍特写图
			sprintf(result->resultPicture, filename);
		}

		if(result->time > 0 ){

			if(getClonedLpr(result, dlg->LPRClonedList)==1){
				release("getConedLpr @ 0x%x  %s", result, result->plate);
				insertLpr(result);

				//show at list
				int nSize = dlg->LPRClonedList.size();
				for(int i=0; i<nSize; i++)
				{
					LPR_ResultPair *lprPair = dlg->LPRClonedList.front();
					dlg->LPRClonedList.pop_front();
					if(lprPair==0)
						continue;
					int nRow = dlg->m_list.InsertItem(0, "");//
					dlg->m_list.SetItemText(nRow, 0, lprPair->lpr_result[0].plate);//车牌
					dlg->m_list.SetItemText(nRow, 1, lprPair->lpr_result[0].FormatTime());//时间1
					dlg->m_list.SetItemText(nRow, 2, lprPair->lpr_result[1].FormatTime());//时间2
					sprintf(temp, "%d ", abs(lprPair->lpr_result[1].time - lprPair->lpr_result[0].time)/60);
					dlg->m_list.SetItemText(nRow, 3,  temp);//时间差(分钟)
					dlg->m_list.SetItemText(nRow, 4, lprPair->lpr_result[0].resultPicture);//图片1
					dlg->m_list.SetItemText(nRow, 5, lprPair->lpr_result[1].resultPicture);//图片2
					if(lprPair->lpr_result[0].pResultBits)
						delete lprPair->lpr_result[0].pResultBits;
					if(lprPair->lpr_result[1].pResultBits)
						delete lprPair->lpr_result[1].pResultBits;
					delete lprPair;

				}
			}else{
				insertLpr(result);
				debug("ProcessResultThread LPRQueueResult.front 0x%x", result);
				if(result->pResultBits)
					delete result->pResultBits;
				delete result;
			}
		}

	}
end:
	debug("ProcessResultThread 正常退出");
	SetEvent(handleCanExit);//设置可以退出了

}

void PlayThread(void *pParam)
{
	CVLPRClonedDemoDlg *dlg = (CVLPRClonedDemoDlg*)pParam;
	HANDLE handleCanExit = dlg->ReginsterMyThread("PlayThread");
	debug("PlayThread 启动  handle=0x%x", handleCanExit);

	while(WaitForSingleObject(handleExit,0)!=WAIT_OBJECT_0){
		if(WaitForSingleObject(hShowVideoFrame,0)==0)
		{
			if(dlg->imagesQueuePlay.size() > 0){
				LPR_Image* pLpr = dlg->imagesQueuePlay.front();
				debug("PlayThread imagesQueuePlay.front  0x%x  imagesQueuePlaySize = %d", pLpr->buffer,  dlg->imagesQueuePlay.size());
				dlg->imagesQueuePlay.pop();
				PlayVideo( pLpr->buffer, ID_VIDEO_WALL,  dlg, pLpr->imageWidth, pLpr->imageHeight);
				delete pLpr->buffer;
				delete pLpr;
			}
		}else{
			while(dlg->imagesQueuePlay.size() > 0){
				LPR_Image* lpr = dlg->imagesQueuePlay.front();
				dlg->imagesQueuePlay.pop();
				delete lpr->buffer;
				delete lpr;
			}
			Sleep(100);
		}
	}
	debug("PlayThread 正常退出");
	SetEvent(handleCanExit);//设置可以退出了

}


//识别线程
void RecognitionThread(void *pParam)
{
	CVLPRClonedDemoDlg *dlg = (CVLPRClonedDemoDlg*)pParam;
	HANDLE handleCanExit = dlg->ReginsterMyThread("RecognitionThread");

	debug("RecognitionThread 启动  handle=0x%x", handleCanExit);
	nFrames = 0;
	int imageSize = 0;
	clock_t t1,t2;
	LPR_Image *pLprImage=0;
	char temp[256]={0};
	int ret = 0;
	SetEvent(handleLPRThreadStoped);
	while(WaitForSingleObject(handleExit,0)!=WAIT_OBJECT_0){

		if(dlg->imagesQueue.size()<1){
			SetEvent(handleLPRThreadStoped);
			Sleep(10);		
			continue;
		}
		ResetEvent(handleLPRThreadStoped);

		nFrames ++;

		if(dlg->imagesQueue.size()>0)
		{
			if(dlg->imagesQueue.size()<1){
				continue;
			}
			pLprImage = dlg->imagesQueue.front();
			dlg->imagesQueue.pop();
			if(pLprImage==0)
				continue;
			debug("RecognitionThread imagesQueue.front 0x%x nFrames=%d   queue size=%d ", pLprImage, nFrames, dlg->imagesQueue.size());

			imageSize = pLprImage->imageWidth * pLprImage->imageHeight *3;

			LPR_Image *playImage = new LPR_Image();
			memcpy(playImage, pLprImage, sizeof(LPR_Image));
			playImage->buffer = new unsigned char[imageSize];
			memcpy(playImage->buffer , pLprImage->buffer, imageSize);
			dlg->imagesQueuePlay.push( playImage );//For Play
			SetEvent(hShowVideoFrame);

			if(true) { //if(dlg->company == WENTONG && dlg->TH_LPR_canRun) 
				TH_PlateIDResult result[20];        //必须定义数组型 
				memset(&result, 0, sizeof(result)); 
				int nResultNum = 1; 
				TH_RECT rcDetect={0, 0, pLprImage->imageWidth, pLprImage->imageHeight}; 

				t1 = clock();
				ret = -1;
				ret  =  TH_RecogImage( pLprImage->buffer,  pLprImage->imageWidth, pLprImage->imageHeight,  result, &nResultNum, &rcDetect, &dlg->plateConfigTh); 
				t2 = clock();

				if(ret!=0 || nResultNum<=0){
					debug("未识别 ret = %d", ret);
					ret = -1;
				}else{
					ret = 1;
					LPR_Result *r = new LPR_Result();
					r->takesTime = t2-t1;
					sprintf(r->plate, "%s", result[0].license);
					r->confidence = result[0].nConfidence*1.0/100;
					sprintf(r->carLogo, "%s", CarLogo[ result[0].nCarLogo] );
					sprintf(r->plateType, "%s", CarType[result[0].nType]);
					sprintf(r->direct, "%s", TH_Dirction[result[0].nDirection]);
					sprintf(r->carColor1, "%s", CarColor[result[0].nCarColor]);
					r->plateRect.left = result[0].rcLocation.left;
					r->plateRect.top = result[0].rcLocation.top;
					r->plateRect.right = result[0].rcLocation.right;
					r->plateRect.bottom = result[0].rcLocation.bottom;

					r->pResultBits = new unsigned char[imageSize];
					memcpy(r->pResultBits , pLprImage->buffer, imageSize );

					sprintf(r->resultPicture, pLprImage->filePath);
					r->imageWidth = pLprImage->imageWidth;
					r->imageHeight = pLprImage->imageHeight;

					//					debug("车牌: %s  车标: %s", result[0].license, CarLogo[ result[0].nCarLogo] );

					dlg->LPRQueueResult.push(r);
					debug("RecognitionThread LPRQueueResult.push  0x%x", r);
				}
			}
			delete pLprImage->buffer;
			delete pLprImage;

			dlg->timeNow = clock();
			sprintf(temp, "Rate:%d fps", nFrames*1000/(dlg->timeNow - dlg->timeStart));
			debug("RecognitionThread %s",temp );

			sprintf(temp, "文件总量: %d, 已处理: %3g %% ( %d/ %d)", filesCount, nFrames*1.0/filesCount *100, nFrames, filesCount);
			dlg->GetDlgItem(ID_PROCESS_STATUS)->SetWindowText(temp);
		}
	}

end:
	debug("RecognitionThread 正常退出");
	SetEvent(handleCanExit);//设置可以退出了

}

unsigned char* GetImageDataByPath(const char *pathIn, int *width, int *height, Rect *rectIn=0)
{
	Bitmap* image = 0;
	image = KLoadBitmap( pathIn );
	if(image==0)
		return  0 ;

	Rect rect(0,0, image->GetWidth(), image->GetHeight());
	if(rectIn!=0){
		memcpy(&rect, rectIn, sizeof(Rect));
	}
	*width = rect.Width ; 
	*height = rect.Height;

	int BufferLen =  rect.Width * rect.Height * 3;
	unsigned char* buffer = new unsigned char[BufferLen];

	BitmapData bitmapData={0};
	bitmapData.Stride = WIDTHSTEP( rect.Width );
	bitmapData.Scan0 = new BYTE[bitmapData.Stride * rect.Height];


	image->LockBits(&rect, ImageLockModeRead | ImageLockModeUserInputBuf, PixelFormat24bppRGB, &bitmapData); 

	memcpy(buffer, (BYTE*)bitmapData.Scan0, BufferLen); 

	delete bitmapData.Scan0;
	image->UnlockBits( &bitmapData);
	delete image;

	return buffer;

}

void CVLPRClonedDemoDlg::LoadImageFromPath(char * path)
{
	Bitmap* image = 0;
	image = KLoadBitmap( path );
	//	DrawImg2Hdc(image, ID_VIDEO_WALL, this);
	if(image==0)
		return ;

	LPR_Image *pLpr = new LPR_Image();
	sprintf(pLpr->filePath,"%s", path );
	pLpr->imageWidth = image->GetWidth();
	pLpr->imageHeight = image->GetHeight();
	pLpr->imageSize = pLpr->imageWidth * pLpr->imageHeight * 3;
	pLpr->buffer = new unsigned char[pLpr->imageSize];

	long BufferLen;
	BufferLen= pLpr->imageSize;

	BitmapData bitmapData={0};
	bitmapData.Stride = WIDTHSTEP(pLpr->imageWidth);
	bitmapData.Scan0 = new BYTE[bitmapData.Stride * pLpr->imageHeight];

	Rect rect(0,0, pLpr->imageWidth, pLpr->imageHeight);
	image->LockBits(&rect, ImageLockModeRead | ImageLockModeUserInputBuf, PixelFormat24bppRGB, &bitmapData); 

	memcpy(pLpr->buffer, (BYTE*)bitmapData.Scan0, BufferLen); 

	delete bitmapData.Scan0;
	image->UnlockBits( &bitmapData);

	debug("LPRFromImage imagesQueue.size = %d", imagesQueue.size());

	delete image;

	imagesQueue.push(pLpr);//放入队列进行处理
}

void PictureThread(void *pParam)
{
	CVLPRClonedDemoDlg *dlg = (CVLPRClonedDemoDlg*)pParam;
	HANDLE handleCanExit = dlg->ReginsterMyThread("PictureThread");
	debug("PictureThread 启动  handle=0x%x", handleCanExit);

	char temp[512]={0};
	curFileIndex = 0;

	while(WaitForSingleObject(handleExit,0)!=WAIT_OBJECT_0  && dlg->mListPicturesPath.size() > 0)
	{
		curFileIndex ++;
		char *path = dlg->mListPicturesPath.front();
		if(path!=0)
		{
			debug("path=%s",path);
			dlg->LoadImageFromPath(path);

			dlg->mListPicturesPath.pop_front();
			delete path;

			sprintf(temp, "文件总量: %d, 已加载: %3g %% ", filesCount, curFileIndex*1.0/filesCount *100 );
			dlg->GetDlgItem(ID_STATUS)->SetWindowText(temp);
		}
	}
	
end:
	dlg->GetDlgItem(BT_ANAY)->EnableWindow(true);
	debug("PictureThread 正常退出");
	SetEvent(handleCanExit);//设置可以退出了
}

void ListFilesThread(void *pParam)
{

}

void CVLPRClonedDemoDlg::OnBnClickedAnay()
{
	//情况之前的图片文件列表
	while(mListPicturesPath.size() > 0 )
	{
		char *p = mListPicturesPath.front();
		mListPicturesPath.pop_front();
		delete p;
	}
	CString path;
	for(int i=0; i<m_listDirs.GetCount(); i++){
		m_listDirs.GetText(i, path);
		FileUtil::ListFiles(path.GetBuffer(path.GetLength()), mListPicturesPath, "*.jpg", true);
		FileUtil::ListFiles(path.GetBuffer(path.GetLength()), mListPicturesPath, "*.bmp", true);
		FileUtil::ListFiles(path.GetBuffer(path.GetLength()), mListPicturesPath, "*.png", true);
	}
	filesCount = mListPicturesPath.size();
	curFileIndex = 0;
	path.Format("文件总量: %d,已处理: %3d %%   %d/ %d", filesCount, curFileIndex*1.0/filesCount*100, curFileIndex, filesCount);
	GetDlgItem(ID_PROCESS_STATUS)->SetWindowText(path);

	if(TH_InitDll(0)){
		nFrames = 0;
		_beginthread(PictureThread, 0 ,this);
		GetDlgItem(BT_ANAY)->EnableWindow(false);
	}
}


//每个线程都要注册，退出的时候会进行一一检测
//返回：分配给该线程的CEvent
HANDLE CVLPRClonedDemoDlg::ReginsterMyThread(char *name)
{
	// 创建事件
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, name);
	SetEvent(hEvent);
	EventList.push_back(hEvent);
	debug("EventList.size = %d", EventList.size());
	return hEvent;

}
void CVLPRClonedDemoDlg::startThreads()
{
	_beginthread(RecognitionThread, 0 ,this);
	_beginthread(PlayThread, 0 ,this);
	_beginthread(ProcessResultThread, 0 ,this);

}
void CVLPRClonedDemoDlg::OnLbnDblclkListDirs()
{
	//双机 移除
	int index = m_listDirs.GetCurSel();
	m_listDirs.DeleteString(index);
}

void CVLPRClonedDemoDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int nItem = m_list.GetNextSelectedItem(pos);
	CString temp;
	if(nItem>=0){
		temp = m_list.GetItemText(nItem, 4 );
		Bitmap* imagePlate = KLoadBitmap(temp.GetBuffer(temp.GetLength()));
		if(imagePlate)
		{
			DrawImg2Hdc(imagePlate, ID_VIDEO_WALL, this);
			delete imagePlate;
		}

		temp = m_list.GetItemText(nItem, 5 );
		Bitmap* imageScreen = KLoadBitmap(temp.GetBuffer(temp.GetLength()));
		if(imageScreen)
		{
			DrawImg2Hdc(imageScreen, ID_PICTURE, this);
			delete imageScreen;
		}
	}
	*pResult = 0;
}


void CVLPRClonedDemoDlg::OnMouseMove(UINT nFlags, CPoint pointIn)
{
	CRect r1, r2, r3;
	GetDlgItem(ID_VIDEO_WALL)->GetWindowRect(&r1);
	GetDlgItem(ID_PICTURE)->GetWindowRect(&r2);
	GetDlgItem(ID_LPR_PICTURE)->GetWindowRect(&r3);
	ScreenToClient(&r1);
	ScreenToClient(&r2);

	//debug("OnMouseMove:  point(%d, %d) ", point.x, point.y );

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	int nItem = m_list.GetNextSelectedItem(pos);
	CString temp;

	if(r1.PtInRect(pointIn) || r2.PtInRect(pointIn) )
	{
		//	debug("PlayVideo:  r1.PtInRect(%d)  r2.PtInRect(%d) ", r1.PtInRect(point),  r2.PtInRect(point));
		if(nItem>=0){
			int width = 0;
			int height =0;

			CPoint point;
			CPoint pt;
			float bix=0.0, biy=0.0;
			if(r1.PtInRect(pointIn)){
				temp = m_list.GetItemText(nItem, 4 );
				Image *image = KLoadBitmap( temp.GetBuffer(temp.GetLength()) );
				if( image == 0)
					goto end;
				width = image->GetWidth();
				height = image->GetHeight();
				delete image;
				point.x = pointIn.x - r1.left;
				point.y = pointIn.y - r1.top;

				bix = width*1.0/r1.Width();
				biy = height*1.0/r1.Height();
			}
			else if(r2.PtInRect(pointIn)){
				temp = m_list.GetItemText(nItem, 5 );
				Image *image = KLoadBitmap( temp.GetBuffer(temp.GetLength()) );
				if( image == 0)
					goto end;
				width = image->GetWidth();
				height = image->GetHeight();
				delete image;
				point.x = pointIn.x - r2.left;
				point.y = pointIn.y - r2.top;

				bix = width*1.0/r2.Width();
				biy = height*1.0/r2.Height();
			}

			pt.x = bix * point.x;
			pt.y = biy * point.y;

			//	debug("pt before (%d,%d)", pt.x, pt.y);
			//set pt.x
			pt.x -= r3.Width()/2;
			if(pt.x<0)
				pt.x = 0;
			else if(pt.x> (width - r3.Width()) )
				pt.x = width - r3.Width();

			//set pt.y
			pt.y -= r3.Height()/2;
			if(pt.y<0)
				pt.y = 0;
			else if(pt.y> (height - r3.Height()) )
				pt.y = height - r3.Height();

			//	debug("pt after (%d,%d)", pt.x, pt.y);

			int w=0, h=0;
			Rect rectIn( pt.x, pt.y, r3.Width(), r3.Height());
			unsigned char* imageData = GetImageDataByPath( temp.GetBuffer(temp.GetLength()), &w, &h, &rectIn);

			if(imageData)
			{
				//	debug("PlayVideo: %d x %d ",  width, height);
				PlayVideo(imageData, ID_LPR_PICTURE, this, w, h);
				delete imageData;
			}
		}
	}
end:
	CDialog::OnMouseMove(nFlags, pointIn);
}


void LoadingThread(void* pParam)
{
	CVLPRClonedDemoDlg *dlg = (CVLPRClonedDemoDlg*)pParam;
	dlg->loading.DoModal();
//	MessageBox(0, "正在退出...", "", 0);
}


int CVLPRClonedDemoDlg::CloseThread()
{
	int times=0;
	while(EventList.size()>0 && times<100)
	{
		times ++;
		for(list<HANDLE>::iterator it = EventList.begin(); it != EventList.end();)
		{
			SetEvent(handleExit);
			Sleep(10);
			HANDLE h = (HANDLE)*it;
			int ret =WaitForSingleObject(h, 500);
			bool bSigned=false;
			switch(ret)
			{
			case WAIT_FAILED:
				debug(" WAIT_FAILED");
				break;
			case WAIT_TIMEOUT:
				debug("times=%d WAIT_TIMEOUT", times);
				break;
			case WAIT_OBJECT_0:
				debug(" WAIT_OBJECT_0 handle=0x%x  singed , so can exit", h);
				it = EventList.erase(it);
				bSigned = true;
				break;
			default :
				debug("ret=%d, 0x%x  !=WAIT_OBJECT_0  LastError=",ret, h, GetLastError());
				break;
			}
			if(bSigned==false)
				it++;
		}
	}
	return EventList.size();
}

void CVLPRClonedDemoDlg::OnClose()
{
	_beginthread(LoadingThread, 0, this);

	//停止

	if( CloseThread()>0)
		debug("非正常退出 @ CDialog::OnClose()  线程未全部关闭");
	else
		debug("马上正常退出 @ CDialog::OnClose()");
	Sleep(500);

	CDialog::OnClose();
}
