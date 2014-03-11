
// LPRDB.cpp
/*
* Auth: Karl
* Date: 2013/12/19
* LastUpdate: 2013/12/19
*/

//======================================================
#include "stdio.h"
#include "string.h"
#include "LPRDB.h"
#include "FileUtil.h"
#include "TCode.h"

//===========  define ================================
#define SQLIT_NAME		"vlprClone.db"  //数据库文件名称
#define TABLE_LPR_NAME	"t_s_lpr_result" //车牌识别结果存储表名称

#ifndef MAX_PATH
	#define		MAX_PATH	 512
#endif

#ifndef NULL
	#define		NULL	 0
#endif

#define MAX_SQL  4096

//===========  //define ================================


static char appPath[MAX_PATH]={0};
static char sqlitePath[MAX_PATH]={0};
static sqlite3  *pSqlite3=0;
static sqlite3_stmt  *stmt = NULL;

//======================================================

char *GetAppPath()
{
	memset(appPath, 0,MAX_PATH);
	char temp[MAX_PATH]={0};
	GetModuleFileName(NULL,  temp, MAX_PATH);
	char *p = strrchr(temp,'\\');
	if(p != NULL)
	{
		memcpy(appPath, temp, sizeof(char)*(p-temp));
		return appPath;
	}
	return NULL;
}

sqlite3 *OpenSqlite()
{
	if(pSqlite3==NULL)
	{
		GetAppPath();
		memset(sqlitePath, 0, 512);
		sprintf(sqlitePath, "%s/"SQLIT_NAME, appPath );
		int error = sqlite3_open(sqlitePath, &pSqlite3);
		if(error){
			debug("can't open database[%d]: %s\n",error, sqlite3_errmsg(pSqlite3), sqlite3_errstr(error));
		//	MessageBox(0, L"opendb success.", L"", 0);
			return NULL;
		}
	}

	return pSqlite3;
}
int close_db(sqlite3  *p=0)
{
	return 0;


	int ret = 0;
	if(pSqlite3==p)
		pSqlite3 = 0;
	if(p!=0 ){
		ret =sqlite3_close(p);
		p = 0;
	}
	return ret;
}

int insertLpr(LPR_Result *result)
{
	int line=0;

	pSqlite3 = OpenSqlite();
	if(pSqlite3==NULL)
		return -1;

	char strsql[MAX_SQL]={0};
	sprintf(strsql, "insert into "TABLE_LPR_NAME"(plate, confidence, carlogo, tasksTime, lastUpdateTime,time, resultPicture, folder, status) \
						values(?,?,?,?,?,?,?,?,1) " ) ;

	debug("strsql = %s \n", strsql);
	
	int rc = sqlite3_prepare_v2(pSqlite3 , strsql , strlen(strsql) , &stmt , NULL);
	if(rc != SQLITE_OK)
	{
		if(stmt)
		{
			sqlite3_finalize(stmt);
		}
		close_db(pSqlite3);
		return -1;
	}
	int i=0;
	char *temp = 0 ;
	temp = G2U(result->plate);
	sqlite3_bind_text( stmt , 1 , temp , strlen(temp), NULL);
	sqlite3_bind_double(  stmt , 2 , result->confidence);
	temp = G2U(result->carLogo);
	sqlite3_bind_text( stmt , 3 , temp, strlen(temp), NULL);
	sqlite3_bind_int(  stmt , 4 , result->takesTime);
	sqlite3_bind_text( stmt , 5 , result->lastUpdateTime, strlen(result->lastUpdateTime), NULL);
	sqlite3_bind_int(  stmt , 6 , result->time);
	sqlite3_bind_text( stmt , 7 , result->resultPicture, strlen(result->resultPicture), NULL);
	temp = G2U(result->folder);
	sqlite3_bind_text( stmt , 8 , temp, strlen(temp), NULL);


	int ret = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	close_db(pSqlite3);
	if(ret != SQLITE_DONE)
	{  
		debug("sqlite3_step faile \n" );
		return -1;
	}      

	return 1;
}

int getLPRList(char *folder, list< LPR_Result*> &list)
{
	pSqlite3 = OpenSqlite();
	if(pSqlite3==NULL)
		return -1;

	char strsql[MAX_SQL]={0};
	sprintf(strsql, "select id, plate, carLogo, resultPicture, time, lastUpdateTime  from "TABLE_LPR_NAME" where folder=? " ) ;

	debug("strsql = %s \n", strsql);
		
	int error = sqlite3_prepare_v2(pSqlite3 , strsql , strlen(strsql) , &stmt , NULL);
	if(error != SQLITE_OK)
	{
		debug("can't getLPRList[%d]: %s  \n",error,   sqlite3_errstr(error));
		if(stmt)
		{
			sqlite3_finalize(stmt);
		}
		close_db(pSqlite3);
		return -1;
	}
	char *temp = 0 ;
	temp = G2U(folder);//folder
	sqlite3_bind_text( stmt , 1 , temp , strlen(temp), NULL);

	int nColumn = sqlite3_column_count(stmt);
	int vtype , i, row=0;
	int ret=0;
	do{	
		error = sqlite3_step(stmt);
		if(error == SQLITE_ROW)
		{
			row++;
			LPR_Result *lpr = new LPR_Result();
			lpr->id = sqlite3_column_int(stmt , 0);
			temp = (char*)sqlite3_column_text(stmt , 1);
			sprintf( lpr->plate, "%s", U2G(temp));//车牌
			temp = (char*)sqlite3_column_text(stmt , 2);
			sprintf( lpr->carLogo, "%s", U2G(temp));//车标
			sprintf( lpr->resultPicture, "%s", sqlite3_column_text(stmt , 3));//图片路径
			lpr->time = sqlite3_column_int(stmt , 4);//出现时间
			sprintf( lpr->lastUpdateTime, "%s", sqlite3_column_text(stmt , 5));//信息最后更新时间
			
			list.push_back(lpr);//加入队列
		}
		else if(error == SQLITE_DONE)
		{
			printf("Select finish\n");
			ret = 0;
			break;
		}
		else
		{
			printf("Select failed\n");
			sqlite3_finalize(stmt);
			ret = 0;
			break;
		}
	}while(1);
	sqlite3_finalize(stmt);
	close_db(pSqlite3);

	return list.size();

}
int getClonedLpr(LPR_Result *result, list<LPR_ResultPair*> &lprConedList,  int thread_timeInSecond)
{
	if(result == NULL)
		return -1;

	int ret = -1;

	pSqlite3 = OpenSqlite();
	if(pSqlite3==NULL)
		return -1;

	char strsql[MAX_SQL]={0};
	sprintf(strsql, "select id, plate, carLogo, resultPicture, time, lastUpdateTime  from "TABLE_LPR_NAME" where plate=? and carLogo!=? " ) ;

	debug("strsql = %s \n", strsql);
		
	int error = sqlite3_prepare_v2(pSqlite3 , strsql , strlen(strsql) , &stmt , NULL);
	if(error != SQLITE_OK)
	{
		debug("can't getLpr[%d]: %s  \n",error,   sqlite3_errstr(error));
		if(stmt)
		{
			sqlite3_finalize(stmt);
		}
		close_db(pSqlite3);
		return -1;
	}
	char *temp = 0 ;
	temp = G2U(result->plate);//车牌
	sqlite3_bind_text( stmt , 1 , temp , strlen(temp), NULL);
	temp = G2U(result->carLogo);//车标
	sqlite3_bind_text( stmt , 2 , temp , strlen(temp), NULL);

	int nColumn = sqlite3_column_count(stmt);
	int vtype , i, row=0;
	int diff=0;
	do{	
		error = sqlite3_step(stmt);
		if(error == SQLITE_ROW)
		{
			row++;
			diff = abs( result->time -  sqlite3_column_int(stmt , 3));
			if( diff < thread_timeInSecond ){
				LPR_Result *lpr = new LPR_Result();
				lpr->id = sqlite3_column_int(stmt , 0);
				temp = (char*)sqlite3_column_text(stmt , 1);//车牌
				sprintf( lpr->plate, "%s", U2G(temp));//车牌
				temp = (char*)sqlite3_column_text(stmt , 2);
				sprintf( lpr->carLogo, "%s", U2G(temp));//车标
				sprintf( lpr->resultPicture, "%s", sqlite3_column_text(stmt , 3));//车标
				lpr->time = sqlite3_column_int(stmt , 4);//绝对时间
				sprintf( lpr->lastUpdateTime, "%s", sqlite3_column_text(stmt , 5));//数据更新时间
				
				LPR_ResultPair *lprPair = new LPR_ResultPair();
				lprPair->set(lpr, result);//设置套牌车对
				lprConedList.push_back(lprPair);//加入队列
				delete lpr;
				ret = 1;
				break;
			}
		}
		else if(error == SQLITE_DONE)
		{
			printf("Select finish\n");
			ret = 0;
			break;
		}
		else
		{
			printf("Select failed\n");
			sqlite3_finalize(stmt);
			ret = 0;
			break;
		}
	}while(1);
	sqlite3_finalize(stmt);
	close_db(pSqlite3);

	return ret;

}

//检查此文件夹是否已经分析过
int checkFolder(char *folder)
{
	
	pSqlite3 = OpenSqlite();
	if(pSqlite3==NULL)
		return -1;

	char strsql[MAX_SQL]={0};
	sprintf(strsql, "select * from "TABLE_LPR_NAME" where folder=?" ) ;

	debug("strsql = %s \n", strsql);
		
	int error = sqlite3_prepare_v2(pSqlite3 , strsql , strlen(strsql) , &stmt , NULL);
	if(error != SQLITE_OK)
	{
		debug("can't getLpr[%d]: %s  \n",error,   sqlite3_errstr(error));
		if(stmt)
		{
			sqlite3_finalize(stmt);
		}
		close_db(pSqlite3);
		return -1;
	}
	char *temp = 0 ;
	temp = G2U(folder);//folder
	sqlite3_bind_text( stmt , 1 , temp , strlen(temp), NULL);

	int ret = sqlite3_step(stmt);

	sqlite3_finalize(stmt);
	close_db(pSqlite3);
	if(ret != SQLITE_DONE)
	{  
		debug("sqlite3_step faile \n" );
		return -1;
	}      
	
	return (ret == SQLITE_ROW);
}