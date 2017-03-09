#include "tree.h"
#include "apue_db.h"
#include "stdlib.h"
#include <fstream>
#include "time.h"
void test(ofstream * ofs ,int amount)
{
	cout<<"start testing:"<<amount<<endl;
	int i=0;
	string key,value;
	DBHANDLE db=new btree("shit");
	ifstream ifsd("f1"); //dynamic
	while(i<amount)
	{
		ifsd>>key>>value;
		db_store(db,key,value,DB_INSERT);
		//db_rewind(db);
		if(db_fetch(db,key)!=value)
		{
			cout<<i<<"   ["<<db_fetch(db,key)<<"]  ERROR1\n";

			return;
		}
		i++;
	}
	db_rewind(db);
	//(*ofs)<<amount<<" ";
	
	ifstream ifs("f");
	i=0;
	while(i<amount)
	{
		ifs>>key>>value;
		if (i == 0) getchar();
		cout << "{" << i << "}:  \n";
		db_rewind(db);
		db_store(db,key,value,DB_STORE);
		if(db_fetch(db,key)!=value)
		{
			cout<<i<<endl;
			cout<<"ERROR4\n";
			cout<<db_fetch(db,key).size()<<endl;
		}
		i++;
	}
	db_rewind(db);
	ifstream ifs3("f");
	i=0;
	while(i<amount)
	{
		ifs3>>key>>value;
		db_store(db, key, "z", DB_REPLACE);
		if(db_fetch(db,key)!="z")
		{	
			cout<<"ERROR2\n";
			exit(0);
		}
		i++;
	}
	ifs.close();
	db_rewind(db);
	ifstream ifs2("f");
	i=0;
	while(i<amount)
	{
		ifs2>>key>>value;
		db_delete(db, key);
		i++;
	}
	db_rewind(db);
	delete db;
	/*getchar();
	getchar();
	getchar();*/
	cout<<"deleted\n\n";
}

int main(int argc, char** argv)
{
	ofstream ofs("differ amount test");
	cout<<"!: ";
		test(&ofs,3);
		cout<<"!: ";
		test(&ofs,6);
		cout<<"!: ";
		test(&ofs,10);
}