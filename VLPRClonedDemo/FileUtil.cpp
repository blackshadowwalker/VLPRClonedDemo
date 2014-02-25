
// FileUtil.cpp : 实现文件

/**
* Auth: Karl
* Date: 2014/2/20
* LastUpdate: 2014/2/24
*/

#include <windows.h>  
#include <ShlObj.h> 
#include <stdio.h>
#include <time.h>   
#include <string>

#include "FileUtil.h"

using namespace std;

FileUtil::FileUtil(void)
{
}

FileUtil::~FileUtil(void)
{
}
//删除目录和文件
bool FileUtil::RemoveDir(const char* szFileDir)
{
	
	if(szFileDir==NULL  || !FindFirstFileExists(szFileDir, FILE_ATTRIBUTE_DIRECTORY) )
		return false;
	char *strDir =new char[256];
	char fileName[256] ={0}, temp[256]={0};
	memset(strDir,0, 256);
	memcpy(strDir, szFileDir, strlen(szFileDir));
	sprintf(temp, "%s\\*.*",strDir);
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile((temp),&wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	do
	{
		sprintf(fileName, "%s\\%s", strDir, wfd.cFileName);
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (stricmp(wfd.cFileName,".") != 0 &&
				stricmp(wfd.cFileName,"..") != 0)
				RemoveDir(fileName);
		}
		else
		{
			DeleteFile(fileName);
		}
	}
	while (FindNextFile(hFind,&wfd));
		FindClose(hFind);
	RemoveDirectory(szFileDir);
	delete strDir;
	
	return true;
	
} 

//创建多级目录
bool FileUtil::CreateFolders(const char* folderPath)
{
	char  folder[MAX_PATH]={0};
	char  temp[MAX_PATH]={0};
	char *p=0;
	memcpy(folder, folderPath, strlen(folderPath));

	int len = strlen(folder);
	while( (p = strchr(folder, '\\'))!=NULL){
		*p='/';
	}
	if( folder[len-1]!='/')
		folder[len] = '/';
	len = strlen(folder);
	folder[len] = '\0';

	int index=2;
	BOOL isOk = true;
	
	p = folder;
	while((p = strchr(p, '/'))!=NULL)
	{
		memcpy(temp, folder, p-folder);
		p ++;
		isOk = (BOOL)CreateDirectory( temp, 0 );
		//if(!isOk） 
		//	return false;
	}
	isOk = (BOOL)CreateDirectory( temp, 0 );
	
	return true;
}

/**
* 列出目录下的所有文件
* @czPath : 文件目录
* @listResult: 返回的文件列表
* @fileExt: 文件扩展名称，如 *.jpg
* @fullPathName: 返回的文件名称是否填写全路径，否则只填写名称
* @bListDirectory: 文件夹名称是否加入文件列表
* @bListSub: 是否列出子文件夹中的文件
*
* @Return : &listResult
*/
list<char*>* FileUtil::ListFiles(char *czPath, list<char*> &listResult, char *fileExt, bool fullPathName, bool bListDirectory, bool bListSub)
{
	char file[MAX_PATH];
	lstrcpy(file,czPath);
	lstrcat(file,"\\");
	lstrcat(file,fileExt);

	WIN32_FIND_DATA wfd; 
	HANDLE Find = FindFirstFile(file,&wfd); 
	if (Find == INVALID_HANDLE_VALUE)  
		return NULL;
	
	do
	{
		if (wfd.cFileName[0] == '.') 
		{
			continue;
		} 
		char *szFindPath = new char[MAX_PATH];
		lstrcpy(szFindPath,czPath); 
		lstrcat(szFindPath,"\\"); 
		lstrcat(szFindPath,wfd.cFileName); 

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			if(bListSub)
				ListFiles(szFindPath, listResult,fileExt, bListDirectory, bListSub);  

			if(fullPathName != true)
				sprintf(szFindPath, wfd.cFileName, MAX_PATH);
			if(bListDirectory)
				listResult.push_back( szFindPath );
		}
		else{
			if(fullPathName != true)
				sprintf(szFindPath, wfd.cFileName, MAX_PATH);
			listResult.push_back( szFindPath );

		}
	}while (FindNextFile(Find, &wfd));
	FindClose(Find);

	return &listResult;

}




//选择文件夹 对话框
#ifndef BIF_NEWDIALOGSTYLE
#define BIF_NEWDIALOGSTYLE 0x0040
#endif
char* FileUtil::SelectFolder(HWND hwnd, char* title)
{
	char *szFolder = new char[MAX_PATH]; //得到文件路径	
	memset(szFolder, 0, MAX_PATH);

	//HWND hwnd = hWnd->GetSafeHwnd();   //得到窗口句柄
#ifdef _SHGetMalloc_  
	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR)		//取得IMalloc分配器接口
	{   
		BROWSEINFO		bi;
		TCHAR			pszBuffer[MAX_PATH];
		LPITEMIDLIST	pidl;   

		bi.hwndOwner		= hwnd;
		bi.pidlRoot			= NULL;
		bi.pszDisplayName   = pszBuffer;
		bi.lpszTitle		= _T(title); //选择目录对话框的上部分的标题
		//添加新建文件夹按钮 BIF_NEWDIALOGSTYLE
		bi.ulFlags			=  BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
		bi.lpfn				= NULL;
		bi.lParam			= 0;
		bi.iImage			= 0;
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)  //取得IMalloc分配器接口
		{   
			if (::SHGetPathFromIDList(pidl, pszBuffer)) //获得一个文件系统路径
			{
				sprintf(szFolder, "%s",  pszBuffer);
			}
			else szFolder = '\0';
			pMalloc->Free(pidl);	//释放内存

			//	MessageBox(m_strPath);
		}
		pMalloc->Release();			//释放接口
	}
#else  
	BROWSEINFO bi;  
	TCHAR			pszBuffer[MAX_PATH];
	ZeroMemory(&bi,sizeof(BROWSEINFO));  
	bi.hwndOwner		= hwnd;
	bi.pidlRoot			= NULL;
	bi.lpszTitle		= (title); //选择目录对话框的上部分的标题
	//添加新建文件夹按钮 BIF_NEWDIALOGSTYLE
	bi.ulFlags			=  BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS | BIF_EDITBOX;
	bi.lpfn				= NULL;
	bi.lParam			= 0;
	bi.iImage			= 0;

	LPITEMIDLIST pidl=(LPITEMIDLIST)CoTaskMemAlloc(sizeof(LPITEMIDLIST));  
	pidl = SHBrowseForFolder(&bi);  
	TCHAR * path = new TCHAR[MAX_PATH];  
	memset(path, 0, MAX_PATH);
	if(pidl != NULL)  
	{  
		if(SHGetPathFromIDList(pidl,path))
			sprintf(szFolder, "%s",  path);
		//MessageBox(NULL,path,TEXT(title),MB_OK);  
	}  
	else 
	{  
		delete szFolder;
		szFolder = 0;
	  //	MessageBox(NULL,TEXT("EMPTY"),TEXT("Choose"),MB_OK);  
	}  
	CoTaskMemFree(pidl);  
	delete path;   
#endif  

	return szFolder;
}


//判断文件/文件夹是否存在
//判断文件：	FindFirstFileExists(lpPath, FALSE);  
//判断文件夹：	FindFirstFileExists(lpPath, FILE_ATTRIBUTE_DIRECTORY);  
BOOL FileUtil::FindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)  
{  
	WIN32_FIND_DATA fd;  
	HANDLE hFind = FindFirstFile(lpPath, &fd);  
	BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;  
	BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;  
	FindClose(hFind);  
	return RetValue;  
}  

char *GetDateTime(char *timeString)
{
	struct tm *tmt = 0;
	time_t t = time(0);
	tmt = localtime(&t);
	if(timeString==0)
		timeString = new char[32];
	memset(timeString, 0, 32);
	sprintf(timeString,"%4d-%02d-%02d  %02d:%02d:%02d", tmt->tm_year+1900, tmt->tm_mon+1, tmt->tm_mday, tmt->tm_hour, tmt->tm_min, tmt->tm_sec );
	return timeString;
}

void __cdecl debug(const char *format, ...)
{
//	return ;

	char buf[4096]={0}, *p=buf;
	
	char *t = GetDateTime();
	sprintf(p,"%s ",t);
	p += strlen(p);

    va_list args;
    va_start(args, format);
    p += _vsnprintf(p, sizeof buf - 1, format, args);
    va_end(args);
  // while ( p > buf && isspace(p[-1]) )    *--p = '\0';
    *p++ = '\r';
    *p++ = '\n';
    *p = '\0';

    OutputDebugString(buf);
}


void __cdecl release(const char *format, ...)
{
	char buf[4096]={0}, *p=buf;
	
	char *t = GetDateTime();
	sprintf(p,"%s ",t);
	p += strlen(p);

    va_list args;
    va_start(args, format);
    p += _vsnprintf(p, sizeof buf - 1, format, args);
    va_end(args);
  // while ( p > buf && isspace(p[-1]) )    *--p = '\0';
    *p++ = '\r';
    *p++ = '\n';
    *p = '\0';

    OutputDebugString(buf);
}