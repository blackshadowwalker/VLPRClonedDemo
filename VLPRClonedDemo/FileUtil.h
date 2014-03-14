// FileUtil.h

/**
* Auth: Karl
* Date: 2014/2/20
* LastUpdate: 2014/2/24
*/

#pragma once

#ifndef HEADER_FILEUTIL
#define HEADER_FILEUTIL

//#include <list>
//#include <string>
//using namespace std;

#include <list>
using namespace std;

#ifndef MAX_PATH
	#define MAX_PATH 256
#endif

class FileUtil
{
public:
	FileUtil(void);
	~FileUtil(void);

	//列出文件夹下面的所有文件
	static list<char*>* ListFiles(char *czPath, list<char*> &listResult, char *fileExt="*.*",bool fullPathName=false, bool bListDirectory=false, bool bListSub=false);
	
	//弹出选择文件夹对话框
	static char* SelectFolder(HWND hwnd, char* title="Select Folder", char* dir=0);

	//判断文件/文件夹是否存在
	//判断文件：	FindFirstFileExists(lpPath, FALSE);  
	//判断文件夹：	FindFirstFileExists(lpPath, FILE_ATTRIBUTE_DIRECTORY);  
	static BOOL FindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter);

	//创建多级目录
	static bool CreateFolders(const char* folderPath);

	//删除多级目录（包含文件）
	static bool RemoveDir(const char* szFileDir);

	static char *FormatFileName(const char *path, int indexIn=0, bool fileTime=true, const char *destDir=0);

private:
	


};

char *GetDateTime(char *timeString=0);
void __cdecl debug(const char *format, ...);
void __cdecl release(const char *format, ...);

long __cdecl getCurrentTime();

#endif