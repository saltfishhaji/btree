#ifndef _APUE_DB_H
#define _APUE_DB_H

#include "tree.h"
typedef btree* DBHANDLE;
//#define O_RDWR 0
//#define O_CREAT 1
//#define O_TRUNC 2
#define DB_INSERT 1 //int flag
#define DB_REPLACE 2
#define DB_STORE 3

//void err_sys(string str);
//void err_quit(string str);

//DBHANDLE db_open(const string pathname); //db_open成功返回，则将建立两个文件
//void db_close(DBHANDLE db); //db_close将关闭索引文件和数据文件,释放内部缓冲的存储空间
//flag:DB_INSERT,DB_REPLACE,DB_STORE
bool db_store(DBHANDLE db, const string key, const string data, int flag);
string db_fetch(DBHANDLE db, const string key);  //寻找
bool db_delete(DBHANDLE db, const string key);
void db_rewind(DBHANDLE db);
//string db_nextrec(DBHANDLE db, string key);

#endif