
// LPRDB.h
/*
* Auth: Karl
* Date: 2013/12/19
* LastUpdate: 2013/12/19
*/

#ifndef  __LPR_DB_H
#define  __LPR_DB_H

//=====  STL ==============
#include <iostream>
#include <list>
#include <map>
using namespace std;
//=========================

extern "C"
{
	#include "./sqlite3.h"
};
#include "LPR_info.h"


int insertLpr(LPR_Result *result);
int getClonedLpr(LPR_Result *result, list<LPR_ResultPair*> &lprConedList, int thread_timeInSecond = 3600);
int delteLpr();
int checkFolder(char *folder);
int getLPRList(char *folder, list< LPR_Result*> &list);



#endif