
// VLPRClonedDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VLPRClonedDemo.h"
#include "VLPRClonedDemoDlg.h"

#include "FileUtil.h"


#pragma comment(lib, "TH_PLATEID.lib")

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

	TH_InitDll(0);

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
	
	int n = TH_InitPlateIDSDK( &plateConfigTh ); 
	if(n!=0)
		MessageBox( pErrorInfo[n]);

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



//处理识别结果线程
void ProcessResultThread(void *pParam)
{
	/*
	char filename[256]={0};
	CVLPRDemoDlg *dlg = (CVLPRDemoDlg*)pParam;
	HANDLE handleCanExit = dlg->ReginsterMyThread("ProcessResultThread");

	char temp[256]={0};
	debug("ProcessResultThread 启动  handle=0x%x", handleCanExit);

	while(WaitForSingleObject(handleExit,0)!=WAIT_OBJECT_0){
		if(dlg->LPRQueueResult.size()<1){
			Sleep(10);		
			continue;
		}
		LPR_Result *result = dlg->LPRQueueResult.front();
		debug("ProcessResultThread Frame=%d  Plate=%s  (%d,%d)-(%d,%d)", nFrames, result->plate, \
			result->plateRect.left, result->plateRect.top,
			result->plateRect.right, result->plateRect.bottom);

		dlg->m_list.SetItemState(0, 0, LVIS_SELECTED|LVIS_FOCUSED); //取消选中
		int column = 1;
		dlg->GetDlgItem(ID_LPR)->SetWindowText(result->plate);
		int count = dlg->m_list.GetItemCount();
		int nRow = dlg->m_list.InsertItem(0, "");//车牌图片
		dlg->m_list.SetItemText(nRow, column++, result->plate);//车牌
		sprintf(temp, "%g", result->confidence);
		dlg->m_list.SetItemText(nRow, column++, temp);//置信度
		sprintf(temp, "%3dms", result->takesTime);
		dlg->m_list.SetItemText(nRow, column++, temp);//耗时
		sprintf(temp, "%s", result->plateType);
		dlg->m_list.SetItemText(nRow, column++, temp);//车牌类型

		sprintf(temp, "%s", result->carLogo);
		dlg->m_list.SetItemText(nRow, column++, temp);//车标

		sprintf(temp, "%s", result->carColor1 );
		dlg->m_list.SetItemText(nRow, column++, temp);//车身主颜色
		sprintf(temp, "%s", result->carColor2 );
		dlg->m_list.SetItemText(nRow, column++, temp);//车身次颜色
		sprintf(temp, "%s", result->direct);
		dlg->m_list.SetItemText(nRow, column++, temp);//运动方向

		time_t timer;
		timer = time(NULL);
		struct tm *tblock;
		tblock = localtime(&timer);
		sprintf(temp,"%d-%d-%d %d:%d:%d", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec );
		dlg->m_list.SetItemText(nRow, column++, temp);//时间

		dlg->m_list.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED); //选中

		dlg->ShowPicture(result->pResultBits);

		//get Plate
		int w = result->plateRect.right - result->plateRect.left;
		int h = result->plateRect.bottom - result->plateRect.top;
		int linestep = WIDTHSTEP(w);
		unsigned char *pBit = 0;

		pBit = result->pResultBits;//图片

		if(dlg->LPRQueueResult.size()>0)
			dlg->LPRQueueResult.pop();
		if(pBit!=NULL)
		{
			memset(plate, 0 , w*h*3);
			for(int i=0; i<h; i++){
				memcpy( (plate + i * linestep ),
					(pBit + result->plateRect.left*3 + (result->plateRect.top+i) * WIDTHSTEP(dlg->imageWidth)),
					linestep );
			}
			dlg->ShowPlatePicture(plate, w, h );

			{
				time_t timer;
				timer = time(NULL);
				struct tm *tblock;
				tblock = localtime(&timer);
				sprintf(temp,"%d-%d-%d_%d.%d.%d", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec );
			}
			sprintf(filename, "%s/%s_%s_plate.bmp", dlg->m_imageDir, temp, result->plate);
		debug(filename);
			VideoUtil::write24BitBmpFile(filename, w, h,(unsigned char*)plate, true, linestep);//车牌图片
			dlg->m_list.SetItemText(nRow, column++, filename);//车牌位置  column = 10
			
			//	delete plate;
			sprintf(filename, "%s/%s_%s.bmp", dlg->m_imageDir, temp, result->plate);
			VideoUtil::write24BitBmpFile(filename,dlg->imageWidth, dlg->imageHeight,(unsigned char*)pBit,  WIDTHSTEP(dlg->imageWidth));//抓拍特写图
			dlg->m_list.SetItemText(nRow, column++, filename);//特写图位置  column = 11
		debug(filename);

			delete pBit;
		}
		debug("ProcessResultThread LPRQueueResult.front 0x%x", result);
		delete result;
		
	}
	debug("ProcessResultThread 正常退出");
	SetEvent(handleCanExit);//设置可以退出了
	SetEvent(handleExit); //第1次SetEvent(hEvent)设置事件有信号并WaitForSingleObject后,第2次再用,要重新在SetEvnet一次,否则返回超时
	*/
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

long  nFrames=0;
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
				}
				else{
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

					debug("车牌: %s  车标: %s", result[0].license, CarLogo[ result[0].nCarLogo] );

				//	dlg->LPRQueueResult.push(r);
					if(r->pResultBits)
						delete r->pResultBits;
					delete r;
			//		debug("RecognitionThread LPRQueueResult.push  0x%x", r);
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
	SetEvent(handleExit); 
	
}

void CVLPRClonedDemoDlg::LoadImageFromPath(char * path)
{
	Bitmap* image = 0;
	image = KLoadBitmap( path );
//	DrawImg2Hdc(image, ID_VIDEO_WALL, this);

	LPR_Image *pLpr = new LPR_Image();
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
	
	if(image!=0)
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
	while(dlg->mListPicturesPath.size() > 0)
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

	nFrames = 0;
	_beginthread(PictureThread, 0 ,this);
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
	
}