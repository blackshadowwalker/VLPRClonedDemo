// ***************************************************************
//  TH_PlateID.h   version:  4.0     date: 2010.4.12
//  -------------------------------------------------------------
//  清华大学智能图文信息处理研究室。版权所有。
//  -------------------------------------------------------------
//  Center for Intelligent Image and Document Information Processing
//  -------------------------------------------------------------
//  Copyright (C) 2007 - All Rights Reserved
// ***************************************************************
//   Author: Liu CS, Zhou J
// ***************************************************************
//		Revision history:
//			2010.4.12: v4.0, TH_PlateID车牌识别SDK的接口
// ***************************************************************

#ifndef __TH_PLATEID_H_INCLUDE__
#define __TH_PLATEID_H_INCLUDE__

#if !defined(WIN32) && !defined(__stdcall)
#define __stdcall
#endif

#include "TH_ErrorDef.h"

#ifdef __cplusplus
extern "C" {
#endif


//车牌颜色
	static char *plateColor[6]={"未知","蓝色","黄色","白色","黑色","绿色"};
#define LC_UNKNOWN  0	// 未知
#define LC_BLUE   1		// 蓝色
#define LC_YELLOW 2		// 黄色
#define LC_WHITE  3		// 白色
#define LC_BLACK  4		// 黑色
#define LC_GREEN  5		// 绿色
	
//车牌类型
	static char *plateType[15]={"未知","蓝牌小汽车","黑牌小汽车","单排黄牌","双排黄牌","警车车牌","武警车牌","个性化车牌","单排军车","双排军车","使馆牌","香港牌","拖拉机","澳门牌",""};
#define LT_UNKNOWN  0   //未知车牌
#define LT_BLUE     1   //蓝牌小汽车
#define LT_BLACK    2   //黑牌小汽车
#define LT_YELLOW   3   //单排黄牌
#define LT_YELLOW2  4   //双排黄牌（大车尾牌，农用车）
#define LT_POLICE   5   //警车车牌
#define LT_ARMPOL   6   //武警车牌
#define LT_INDIVI   7   //个性化车牌
#define LT_ARMY     8   //单排军车
#define LT_ARMY2    9   //双排军车
#define LT_EMBASSY  10  //使馆牌
#define LT_HONGKONG 11  //香港牌
#define LT_TRACTOR  12  //拖拉机
#define LT_MACAU    13  //澳门牌

//车辆颜色
		static char *CarColor[9]={"白","灰(银)","黄","粉","红","绿","蓝","棕","黑"};
#define LGRAY_DARK	0	//深
#define LGRAY_LIGHT	1	//浅

#define LCOLOUR_WHITE	0	//白	
#define LCOLOUR_SILVER	1	//灰(银)
#define LCOLOUR_YELLOW	2	//黄
#define LCOLOUR_PINK	3	//粉
#define LCOLOUR_RED		4	//红
#define LCOLOUR_GREEN	5	//绿
#define LCOLOUR_BLUE	6	//蓝
#define LCOLOUR_BROWN	7	//棕 
#define LCOLOUR_BLACK	8	//黑


//运动方向
#define DIRECTION_LEFT	1
#define DIRECTION_RIGHT	2
#define DIRECTION_UP	3
#define DIRECTION_DOWN	4

//图像格式（TH_SetImageFormat函数的cImageFormat参数）
#define ImageFormatRGB		0			//RGBRGBRGB...
#define ImageFormatBGR		1			//BGRBGRBGR...
#define ImageFormatYUV422	2			//YYYY...UU...VV..	(YV16)
#define ImageFormatYUV420COMPASS 3		//YYYY...UV...		(NV12)
#define ImageFormatYUV420	4			//YYYY...U...V...	(YU12)
#define ImageFormatUYVY	    5			//UYVYUYVYUYVY...	(UYVY)
#define ImageFormatNV21		6			//YYYY...VU...		(NV21)
#define ImageFormatYV12		7			//YYYY...V...U		(YV12)
#define ImageFormatNV16     8           //YYYY...UVUV...    (NV16或YUV422SP) UV列方向抽样，行方向不变 

//车牌类型支持设置定义
//（TH_SetEnabledPlateFormat函数的dFormat参数）
#define PARAM_INDIVIDUAL_ON          0		// 个性化车牌开启
#define PARAM_INDIVIDUAL_OFF         1		// 个性化车牌关闭
#define PARAM_TWOROWYELLOW_ON	     2		// 双层黄色车牌开启
#define PARAM_TWOROWYELLOW_OFF	     3		// 双层黄色车牌关闭
#define PARAM_ARMPOLICE_ON           4		// 单层武警车牌开启
#define PARAM_ARMPOLICE_OFF          5		// 单层武警车牌关闭
#define PARAM_TWOROWARMY_ON          6		// 双层军队车牌开启
#define PARAM_TWOROWARMY_OFF         7		// 双层军队车牌关闭
#define PARAM_TRACTOR_ON			 8		// 农用车车牌开启
#define PARAM_TRACTOR_OFF			 9		// 农用车车牌关闭
#define PARAM_ONLY_TWOROWYELLOW_ON   10		// 只识别双层黄牌开启
#define PARAM_ONLY_TWOROWYELLOW_OFF  11		// 只识别双层黄牌关闭
#define PARAM_EMBASSY_ON			 12		// 使馆车牌开启
#define PARAM_EMBASSY_OFF			 13		// 使馆车牌关闭
#define PARAM_ONLY_LOCATION_ON		 14		// 只定位车牌开启
#define PARAM_ONLY_LOCATION_OFF		 15		// 只定位车牌关闭
#define PARAM_ARMPOLICE2_ON			 16		// 双层武警车牌开启
#define PARAM_ARMPOLICE2_OFF		 17		// 双层武警车牌关闭


#define RECOG_STAGE_ALL			0		// 无错误
#define RECOG_STAGE_FINDPLATE	1		// 没有找到车牌
#define RECOG_STAGE_PLATESCORE_ZERO	2	// 车牌评价值(0分)
#define RECOG_STAGE_PLATESCORE_LOW	3	// 车牌评价值(不及格)
#define RECOG_STAGE_RECOGSCORE_ZERO	4	// 车牌识别分数(0分)
#define RECOG_STAGE_RECOGSCORE_LOW	5	// 车牌识别分数(不及格)

	
//车标类型
static char *CarLogo[20]={"未知", "奥迪", "宝马", "奔驰", "本田", "标志","别克","大众",\
					"丰田","福特","铃木","马自达","起亚","日产尼桑","现代","雪佛兰","雪铁龙","其他",""};
#define CarLogo_UNKNOWN        0    //未知
#define CarLogo_AUDI          1    //奥迪
#define CarLogo_BMW           2    //宝马
#define CarLogo_BENZ          3    //奔驰
#define CarLogo_HONDA         4    //本田
#define CarLogo_PEUGEOT       5    //标志
#define CarLogo_BUICK         6    //别克
#define CarLogo_DASAUTO       7    //大众
#define CarLogo_TOYOTA        8    //丰田
#define CarLogo_FORD          9    //福特
#define CarLogo_SUZUKI        10   //铃木
#define CarLogo_MAZDA         11   //马自达
#define CarLogo_KIA           12   //起亚
#define CarLogo_NISSAN        13   //日产尼桑
#define CarLogo_HYUNDAI       14   //现代
#define CarLogo_CHEVROLET     15   //雪佛兰
#define CarLogo_CITROEN       16   //雪铁龙
#define CarLogo_OTHER         17   //其他

// 车辆类型
//车标类型
static char *CarType[4]={"未知", "轿车", "面包车",""};
#define CARTYPE_UNKNOWN		0	// 未知
#define CARTYPE_SALOON		1	// 轿车
#define CARTYPE_VAN			2	// 面包车

typedef struct TH_RECT
{
	int left;
	int top;
	int right;
	int bottom;
}TH_RECT;

typedef struct TH_PlateIDCfg
{
	int nMinPlateWidth;					// 检测的最小车牌宽度，以像素为单位
	int nMaxPlateWidth;					// 检测的最大车牌宽度，以像素为单位
	
	int nMaxImageWidth;					// 最大图像宽度
	int nMaxImageHeight;				// 最大图像高度

	unsigned char bVertCompress;		// 是否垂直方向压缩1倍后识别
	unsigned char bIsFieldImage;		// 是否是场图像
	unsigned char bOutputSingleFrame;	// 是否视频图像中同一个车的多幅图像只输出一次结果		
	unsigned char bMovingImage;			// 识别运动or静止图像

	unsigned char bIsNight;
	unsigned char nImageFormat;

	unsigned char * pFastMemory;		// DSP等的片内内存，耗时多的运算优先使用这些内存
	int nFastMemorySize;				// 快速内存的大小

	unsigned char *pMemory;				// 普通内存的地址，内建的内存管理，避免内存泄漏等问题
	int nMemorySize;					// 普通内存的大小

	int (*DMA_DataCopy)(void *dst, void *src,int nSize);
	int (*Check_DMA_Finished)();

	int nLastError;			// 用于传递错误信息
							// 0: 无错误
							// 1: FindPlate(没有找到车牌)
							// 2: 车牌评价值(0分)
							// 3: 车牌评价值(不及格)
							// 4: 车牌识别分数(0分)
	                        // 5: 车牌识别分数(不及格)
	int nErrorModelSN;		// 出错的模块编号
	unsigned char nOrderOpt;			//输出顺序选项 0-置信度 1-自上而下 2-自下而上
	unsigned char bLeanCorrection;		// 是否启用车牌旋转功能
	unsigned char bMovingOutputOpt;   	// 0-内部推送+外部获取 1:外部获取	
	unsigned char nImproveSpeed;        // 0: 识别率优先 1:识别速度优先
	unsigned char bCarLogo;             // 0: 不检测车标 1: 检测车标
	unsigned char bLotDetect;			// 0: 不检测车位 1: 检测车位
	unsigned char bShadow;              // 0: 针对无阴影的车牌 1：针对有阴影的车牌

	char reserved[113];

	/*TH_PlateIDCfg(){
		memset(this, 0 , sizeof(TH_PlateIDCfg));
	}*/


}TH_PlateIDCfg;

// 缺省车牌识别SDK配置
const TH_PlateIDCfg c_defConfig = {80, 200, /* Min and Max Plate Width */ 
	720, 576, /* Max Image Width and Height*/ 
	0, /*vert compress*/ 0, /* Field Image? */0, /* output single*/  0/*Moving */,
	0 /* Night? */ , ImageFormatBGR,
	NULL, 0, 
	NULL,0, 
	NULL, NULL,
	0,0,0,0,0,0,0};

static char *TH_Dirction[6]={"未知","左","右","上","下",""};
typedef struct TH_PlateIDResult 
{
	char license[16];	// 车牌字符串
	char color[8];		// 车牌颜色

	int nColor;			// 车牌颜色
	int nType;			// 车牌类型
	int nConfidence;	// 整牌可信度
	int nBright;		// 亮度评价
	int nDirection;		// 车牌运动方向，0 unknown, 1 left, 2 right, 3 up, 4 down 
	
	TH_RECT rcLocation;				// 车牌坐标
	const unsigned char *pbyBits;	/* 该识别结果对应的图片的缓冲区, 只有当 bOutputSingleFrame = true 时，该指针才有效。
									下次识别后，该缓冲区内容被覆盖。调用程序无需释放该缓冲区。
									缓冲区大小等于传递进来的图片数据的长度*/
	int nTime;						// 识别耗时
	unsigned char nCarBright;		//车的亮度
	unsigned char nCarColor;		//车的颜色
	unsigned char nCarLogo;         //车标类型
	unsigned char nCarType;			//车辆类型
	char reserved[94];				//保留
	unsigned short nCarWidth;		//车辆像素宽度
	unsigned short nCarHeight;		//车辆像素高度
}TH_PlateIDResult;


/************************************************************************/
/* TH_InitPlateIDSDK: 初始化车牌识别SDK，在使用该SDK的功能前			*/
/*			必需且仅需调用一次该函数									*/
/*		Parameters:														*/
/*			pPlateConfig[in]: 车牌识别SDK的配置							*/
/*		Return Value: int(ERR_Code)										*/
/************************************************************************/
int __stdcall TH_InitPlateIDSDK(TH_PlateIDCfg *pPlateConfig);

/************************************************************************/
/* TH_UninitPlateIDSDK: 释放车牌识别SDK，在使用该SDK的功能后			*/
/*			必需且仅需调用一次该函数，以释放内存。						*/
/*		Return Value: int(ERR_Code)										*/
/************************************************************************/
int __stdcall TH_UninitPlateIDSDK(TH_PlateIDCfg *pPlateCfg);

/************************************************************************/
/* TH_RecogImage: 识别内存中车牌图像(输出识别到的多个结果)	*/
/*		Parameters:														*/
/*			pbyBits[in]: 指向内存图像数据的指针，3个字节表示1个像素		*/
/*			nWidth[in]: 图像的宽度										*/
/*			nHeight[in]: 图像的高度										*/
/*			pResult[out]: 车牌识别结果数组, 调用方开辟pResult[nResultNum]内存*/
/*			nResultNum[in,out]: in 最大候选车牌个数，out 识别出的车牌个数*/
/*			prcRange[in]: 指定识别范围									*/
/*		Return Value: int(ERR_Code)										*/
/************************************************************************/
int __stdcall TH_RecogImage(const unsigned char *pbyBits, int nWidth, int nHeight, TH_PlateIDResult *pResult,
	int *nResultNum, const TH_RECT *prcRange, TH_PlateIDCfg *pPlateConfig);

/************************************************************************/
/* TH_EvaluateCarColor: 识别车的颜色									*/
/*		Parameters:														*/
/*			pbyBits[in]: 指向内存图像数据的指针，3个字节表示1个像素		*/
/*			nWidth[in]: 图像的宽度										*/
/*			nHeight[in]: 图像的高度										*/
/*			pResult[out]: 车牌识别结果数组, 调用方开辟pResult[nResultNum]内存*/
/*			nResultNum[in,out]: in 最大候选车牌个数，out 识别出的车牌个数*/
/*			prcRange[in]: 指定识别范围									*/
/*		Return Value: int(ERR_Code)										*/
/*		TH_EvaluateCarColor紧接TH_RecogImage之后调用，保持参数不变		*/
/************************************************************************/
int __stdcall TH_EvaluateCarColor(const unsigned char *pbyBits, int nWidth, int nHeight, TH_PlateIDResult *pResult,
	int *nResultNum, const TH_RECT *prcRange, TH_PlateIDCfg *pPlateConfig);

/************************************************************************/
/* TH_SetImageFormat: 设置图像格式										*/
/*		Parameters:														*/
/*			cImageFormat[in]: 图像格式									*/
/*			bVertFlip[in]: 是否颠倒										*/
/*			bDwordAligned[in]: 是否4字节对齐							*/
/*		Return Value: int(ERR_Code)										*/
/************************************************************************/
int __stdcall TH_SetImageFormat( unsigned char cImageFormat, unsigned char bVertFlip, unsigned char bDwordAligned, TH_PlateIDCfg *pPlateConfig );

/************************************************************************/
/* TH_SetEnabledPlateFormat: 设置支持的车牌类型							*/
/*		Parameters:														*/
/*			dFormat[in]: 车牌类型开关，例如PARAM_INDIVIDUAL_ON			*/
/*		Return Value: int(ERR_Code)										*/
/************************************************************************/
int __stdcall TH_SetEnabledPlateFormat(unsigned int dFormat, TH_PlateIDCfg *pPlateConfig);

/************************************************************************/
/* TH_SetProvinceOrder: 设置省份字符串									*/
/*		Parameters:														*/
/*			szProvince[in]: 默认省份字符串，例如"京津冀"，最多支持8个省份*/
/*		Return Value: int(ERR_Code)										*/
/************************************************************************/
int __stdcall TH_SetProvinceOrder( char* szProvince, TH_PlateIDCfg *pPlateConfig);

/************************************************************************/
/* TH_SetRecogThreshold: 设置识别阈值									*/
/*		Parameters:														*/
/*			nPlateLocate_Th[in]: 0 - 9		--   7: 默认阈值			*/
/*					nOCR_Th[in]: 0 - 9		--   5: 默认阈值			*/
/*				0: 最宽松的阈值											*/
/*				9:最严格的阈值											*/
/*		Return Value: int(ERR_Code)										*/
/************************************************************************/
int __stdcall TH_SetRecogThreshold( unsigned char nPlateLocate_Th, unsigned char nOCR_Th, TH_PlateIDCfg *pPlateCfg);

//检查工作过程中最小的剩余内存，如果出现负数，则需要增加给定的初始内存
int __stdcall TH_CheckMinFreeMemory( int *pnMinFreeSRAM, int *pnMinFreeSDRAM, TH_PlateIDCfg *pPlateCfg);

/************************************************************************/
/* 功能: 获取版本号														*/
/*		Parameters：无													*/
/*		返回值: 字符串 格式：主版本号.副版本号.修订号.编译号			*/
/*				不用释放该指针。										*/
/************************************************************************/
const char * __stdcall TH_GetVersion();

/************************************************************************/
/* 功能: 获取加密锁ID, 加密锁ID是8字节唯一ID，							*/
/*		调用此函数前需要调用TH_InitPlateIDSDK							*/
/*		Parameters：存在唯一ID时，ID1是前4字节,ID2是后四字节			*/
/*		返回值: 0-有唯一ID												*/
/*				1-无唯一ID												*/
/************************************************************************/
int __stdcall TH_GetKeyID(unsigned int* ID1, unsigned int* ID2);

int __stdcall TH_SetContrast( unsigned char nContrast, TH_PlateIDCfg *pPlateCfg ); // 清晰度指数 nContrast[in]: 0 - 9
																				 // 最模糊时设为1;最清晰时9设为;
                                                                                 // 自动探测设为0;默认值9

/************************************************************************/
/* TH_SetEnableCarDetect: 设置是否启动车位占用判断						*/
/*		Parameters:														*/
/*			bDetectCar[in]: true:车位占用判断; false:不进行车位占用判断	*/
/*		Return Value: int(ERR_Code)										*/
/*		支持格式: ImageFormatBGR, ImageFormatRGB,
				ImageFormatUYVY,ImageFormatNV21							*/
/************************************************************************/
int __stdcall TH_SetEnableCarDetect( unsigned char bDetectCar, TH_PlateIDCfg *pPlateCfg);

/************************************************************************/
/* TH_SetEnableCarTypeClassify: 设置是否车辆类型判别					*/
/*		Parameters:														*/
/*			bCarTypeClass[in]: true:车型分类;							*/
/*								false:不进行车型分类					*/
/*		Return Value: int(ERR_Code)										*/
/*		支持格式: ImageFormatBGR, ImageFormatRGB,ImageFormatYUV422,
				ImageFormatUYVY,ImageFormatNV21*/
/************************************************************************/
int __stdcall TH_SetEnableCarTypeClassify( unsigned char bCarTypeClass, TH_PlateIDCfg *pPlateCfg);


/************************************************************************/
/* TH_SetEnableCarLogo: 设置是否车标识别								*/
/*		Parameters:														*/
/*			bCarLogo[in]: true:车标识别;								*/
/*								false:不进行车标识别					*/
/*		Return Value: int(ERR_Code)										*/
/************************************************************************/
int __stdcall TH_SetEnableCarLogo( unsigned char bCarLogo, TH_PlateIDCfg *pPlateCfg);

/************************************************************************/
/* TH_SetEnableCarWidth: 设置是否测量车辆宽度							*/
/*		Parameters:														*/
/*			bCarLogo[in]: true:测量车辆宽度;							*/
/*							false:不测量车辆宽度						*/
/*		Return Value: int(ERR_Code)										*/
/************************************************************************/
int __stdcall TH_SetEnableCarWidth( unsigned char bCarWidth, TH_PlateIDCfg *pPlateCfg);

// TH_SetDayNightMode: V4.3.13.0以后函数无实际意义，为保证兼容性保留此函数
int __stdcall TH_SetDayNightMode( unsigned char bIsNight, TH_PlateIDCfg *pPlateConfig);
#ifdef __cplusplus
}
#endif

#endif	//	__TH_PLATEID_H_INCLUDE__
