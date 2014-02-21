//TFLPRecognition.h

#pragma once

#ifndef TF_LPRECOGNITION_H
#define TF_LPRECOGNITION_H
#include <windows.h>

#define  TF_OK               0   //初始化正确
#define  TF_KEY_ERROR       -1   //未找到加密狗
#define  TF_THREAD_ERROR    -2   //调用线程超出数目
#define  TF_PROCESS_ERROR   -3   //当前进程不允许识别

//车牌类型
typedef enum
{
	E_TF_NOTKNOWN,     //未知类型
	E_TF_NORMAL_BLUE,  //普通蓝牌
	E_TF_NORMAL_BLACK, //普通黑牌
	E_TF_NORMAL_YELLOW,//单层黄牌
	E_TF_DOUBLE_YELLOW,//双层黄牌
	E_TF_POLICE,       //白色警牌
	E_TF_WJ,           //白色武警
	E_TF_ARMY,         //白色军牌
	E_TF_OTHER,        //其类型
}TF_LPlateType;


typedef enum
{
	E_TF_BGR,
	E_TF_RGB,
}TF_ImageType;

typedef struct
{
	char cVersion[64];     //版本号
	char cComplieDate[64]; //编译时间
	char cCorpName[256];   //公司名称
}TF_SDK_Details;


//车身颜色
typedef enum
{
	E_TF_VC_NOTKNOWN, //未知
	E_TF_VC_WHITE,    //白
	E_TF_VC_SILVER,   //灰
	E_TF_VC_YELLOW,   //黄
	E_TF_VC_RED,	  //红
	E_TF_VC_GREEN,    //绿
	E_TF_VC_BLUE,     //蓝
	E_TF_VC_BLACK,    //黑
}TF_LVehicleColor;

typedef struct  
{
	int iLeft;
	int iTop;
	int iRight;
	int iBottom;
}TF_Rect;

typedef struct TF_RecParma
{
	TF_ImageType eImageType;  //图像类型
	int iImageMode;           //0：为帧模式 1：为场模式
	int iRecMode;             //识别模式，0：单张图片识别，1:视频检测+识别，2：视频多帧融合识别(注：1模式比2模式快，但识别率和捕获率比2低)
	int iResultNum;           //输出结果参数，最多支持输出8个结果
	int iMinPlateWidth;       //识别最小车牌宽度60
	int iMaxPlateWidth;       //识别最大车牌宽度400
	int iImageWidth;          //图片宽度 只针对视频识别模式设置(模式1 模式2)
	int iImageHeight;         //图片高度 只针对视频识别模式设置(模式1 模式2)
	char pLocalChinese[3];    //本地汉字字符，如果此字符设置为空或者31个省份之外的字，则不使用首汉字

	TF_RecParma(){
		eImageType  = E_TF_BGR;
	}
}TF_RecParma;

typedef struct
{
	int iYear;
	int iMonth;
	int iDay;
	int iHour;
	int iMinute;
	int iSecond;
	int iMilliseconds;
}TF_Time;

typedef struct  TF_Result
{
	char number[20];            //识别结果
	TF_Rect PlateRect;         //车牌位置
	float fConfidence;          //车牌置信度
	TF_LPlateType ePlateType;  //车牌类型
	int iMoveDir;               //车辆运动方向 -1：未知, 0：朝近运动，1：朝远运动，
	TF_LVehicleColor eVColor1; //车身主颜色
	TF_LVehicleColor eVColor2; //车身次颜色
	unsigned char *pResultBits; //视plateColor频识别模式中抓拍的图片,视频模式识别下有效，需要开辟
	TF_Time sTime;             //视频识别模式中抓拍此张图的时间     
	long	takesTime; //耗时
	char	nCarLogo[20]; //车标

    TF_Result(){
		memset(this, 0, sizeof(TF_Result));
	}
}TF_Result;


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

extern "C"  __declspec (dllexport) int   TFLPR_ThreadInit();  //线程初始化，只调用一次
extern "C"  __declspec (dllexport) void* TFLPR_Init(TF_RecParma RecParma);  //初始化,每个线程都必须调用一次
extern "C"  __declspec (dllexport) int   TFLPR_RecImage(const unsigned char *pBits, int iWidth, int iHeight ,TF_Result* pLprResult,TF_Rect* recROI,void* pInstance);//识别
extern "C"  __declspec (dllexport) void  TFLPR_Uninit(void* pInstance);       //释放
extern "C"  __declspec (dllexport) void  TFLPR_GetSDKdetails(TF_SDK_Details* SDK_Details);      //获得版本信息


#endif