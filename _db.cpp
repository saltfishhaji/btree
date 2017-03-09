#include "apue_db.h"
/*DBHANDLE db_open(const string pathname)
{
	DBHANDLE db = new btree(pathname);
	db->read();
	return db;
}

void db_close(DBHANDLE db)
{
	db->write();
}*/

bool db_store(DBHANDLE db, const string key, const string data, int flag)
{
	if (flag == DB_INSERT)
		return db->insert(key, data);
	else if (flag == DB_REPLACE)
		return db->change(key, data);
	else if (flag == DB_STORE)
	{
		db->store(key, data);
		return true;
	}
	else return false;
}

string  db_fetch(DBHANDLE db, const string key)
{
	return db->find(key);
}

bool db_delete(DBHANDLE db, const string key)
{
	return db->tdelete(key);
}

void db_rewind(DBHANDLE db)
{
	db->print();
}
//string db_nextrec(DBHANDLE db, string key)
//{
//	return db->findnext(key);
//}