//LogM.cpp

#include "string.h"
#include "windows.h"
#include "tchar.h"
#include "time.h"

#include <direct.h>  
#include <process.h>

#include "LogM.h"


#ifndef NULL
#define NULL 0
#endif

void ThreadLogFile(void* pParam);

static LogM *g_logM = LogM::Instance();


//====================================================
LogM* LogM::Instance()
{
	LogM *logM = new LogM();
	logM->StartLog();
	return logM;
}

LogM* LogM::newInstance()
{
	LogM *logM = new LogM();
	return logM;
}


LogM::LogM()
{
	logFile = 0;
	memset(appPath, 0 , MAX_PATH);
	memset(EventLogFile, 0 , MAX_PATH);

	GetModuleFileName( 0, EventLogFile, MAX_PATH );
	PTSTR pszDot = _tcsrchr( EventLogFile, _T('.') );
	if ( pszDot )
	{
		pszDot ++;   // Advance past the '.'
		if ( _tcslen(pszDot) >= 3 )
			_tcscpy( pszDot, _T("log") );   // "RPT" -> "Report"
	}

	pszDot = _tcsrchr( EventLogFile, _T('\\') );
	if(pszDot==0)
		pszDot = _tcsrchr( EventLogFile, _T('/') );
	if(pszDot){
		memcpy(appPath, EventLogFile, pszDot-EventLogFile);
	}

};

void __cdecl log(const char *format, ...)
{
    char buf[1024]={0}, *p = buf;
    va_list args;
    va_start(args, format);
    p += _vsnprintf(p, sizeof buf - 1, format, args);
    va_end(args);
  //  while ( p > buf && isspace(p[-1]) ) *--p = '\0';
    *p++ = '\n';
  //  *p = '\0';

	time_t timer;//time_t就是long int 类型
	timer = time(NULL);//这一句也可以改成time(&timer); 
	char str1[64]={0};
	struct tm *tmt =0;
	tmt = localtime(&timer);
	
	sprintf(str1,"%d-%d-%d  %d:%d:%d", tmt->tm_year+1900, tmt->tm_mon+1, tmt->tm_mday, tmt->tm_hour, tmt->tm_min, tmt->tm_sec );
	printf("[%s] \t%s ", str1, buf);
   
	OutputDebugString(buf);
}

FILE* LogM::OpenLog()
{
	logFile = freopen(EventLogFile, "a+",stdout);
	log("=================openLog==============");
	return logFile;
}

void LogM::CloseLog(FILE* fp)
{
	log("=================CloseLog==============");
	if(fp==0)
		fclose(logFile);
	else
		fclose(fp);
}

//  开启log \
* sleepTime: 时间(秒)，每隔 sleepTime 会生成一个新的log文件， 范围: 10 - MAX, 默认 600秒(10分钟)

int LogM::StartLog(int sleepTimeIn)
{
	sleepTime = sleepTimeIn;
	if(sleepTime<10)
		sleepTime = 600;
	_beginthread(ThreadLogFile, 0, this);
	return 0;
}



void  ThreadLogFile(void* pParam)
{
	LogM *logm = (LogM*)pParam;

	char appLogPath[MAX_PATH]={};
	char newFile[MAX_PATH] = {0};
	long sleepTime = 60*60;
	char lpFileName[512]={0};
	sprintf(appLogPath, "%s\\log", logm->appPath);
	int ret = _mkdir(appLogPath);
	if(ret)
		ret = GetLastError();
	sprintf(lpFileName,"%s", logm->EventLogFile);
	sleepTime = logm->sleepTime;

	while(1){
		
		if(logm->GetLogFile()){
			logm->CloseLog();

			time_t timer;//time_t就是long int 类型
			timer = time(NULL);//这一句也可以改成time(&timer); 

			struct tm *tmt =0;
			tmt = localtime(&timer);
			if(sleepTime<60)
				sprintf(newFile,"%s\\%d-%d-%d_%d.%d.%d_%d.log", appLogPath, tmt->tm_year+1900, tmt->tm_mon+1, tmt->tm_mday, tmt->tm_hour, tmt->tm_min, tmt->tm_sec, timer );
			else if(sleepTime<60*60)
				sprintf(newFile,"%s\\%d-%d-%d_%d.%d_%d.log", appLogPath, tmt->tm_year+1900, tmt->tm_mon+1, tmt->tm_mday, tmt->tm_hour, tmt->tm_min, timer );
			else if(sleepTime<24*60*60)
				sprintf(newFile,"%s\\%d-%d-%d_%d_%d.log", appLogPath, tmt->tm_year+1900, tmt->tm_mon+1, tmt->tm_mday, tmt->tm_hour, timer );
			else
				sprintf(newFile,"%s\\%d-%d-%d_%d.log", appLogPath, tmt->tm_year+1900, tmt->tm_mon+1, tmt->tm_mday, tmt->tm_hour, timer );
			rename(lpFileName, newFile);
		}

		logm->OpenLog();
		Sleep(sleepTime*1000);
	}

}