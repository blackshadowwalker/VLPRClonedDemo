/*
* Date: 2013/8/19 15:22
* Author: Karl
*/
//public.h

#pragma once
#ifndef HEADER_PUBLIC
#define HEADER_PUBLIC

typedef struct LPR_Result
{
	char plate[32];//车牌
	float confidence;//置信度
	CRect plateRect;//车牌区域
	char  plateType[32];//车牌类型
//	char  plateColor[32];//车牌颜色
	
	char  carLogo[32];//车标
	char  carColor1[32];//车辆颜色
	char  carColor2[32];//车辆次颜色
	char  direct[32];//运动方向

	unsigned char * pResultBits;

	char  takesTime;//耗时
	
	LPR_Result(){
		memset(this, 0, sizeof(LPR_Result));
	}

}LPR_Result;

static int	lastErrorCode =0;
static char lastError[256]={0};
char*		getLastError();
int			setLastError(char *info);
int			getLastErrorCode();
int			setLastErrorCode(int code);

char *getThisPath(CWinApp *app,char *path=0);

void alert(char *text);

Image* KLoadImage(CString filename);
Bitmap* KLoadBitmap(CString filename);
void DrawImg2Hdc(Image *image, UINT id, CWnd* cWnd);
/*************************************************/


TCHAR* g_GetIniPath(void);
char* ConvertInfo(char* strText);
CString ConvertString(CString strText);
//Set static text in dialogue box (English->current language)
void g_SetWndStaticText(CWnd * pWnd);
TCHAR* g_GetSetPath(void);

void EventLog(char* info);//历史记录文件

//注册表相关操作
bool		OpenReg(HKEY &hKEY, char *data_Set);
BYTE*		QueryReg(char *key, DWORD dataLen, HKEY hKEY);
long		CreateReg(HKEY hKEY,const char *subkey);
bool		WriteReg(HKEY &hKEY, char *data_Set);
bool		SetReg(char *key, BYTE* value, DWORD dataLen, HKEY hKEY);
//\**************************************************************/

//******** date time classes **************
char *formatTime(time_t* t);
char *formatTMTime(struct tm *time);
void Encryption(BYTE *data, long lenData,  BYTE *key, long lenKey);
void Decryption(BYTE *data, long lenData,  BYTE *key, long lenKey);

/**********  math *************/



/**********************************/


#endif