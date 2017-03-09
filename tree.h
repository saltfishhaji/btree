#ifndef _TREE_H
#define _TREE_H

#define index true
#define leaf false
#define INDEX '0'
#define LEAF '1'

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include<sstream>
using namespace std;

const int m = 4;
//static int globali=-1;

class Node{
public:
	Node();
	int number;      //��root����number��m/2��m֮��
	string key[m];
	virtual bool return_type() = 0;
	~Node();
	Node* parent;
};

class index_node :public Node{
public:
	bool return_type(){ return index; };
	index_node();
	Node* son[m];
	~index_node();
};

class leaf_node :public Node{
public:
	bool return_type(){ return leaf; };
	leaf_node();
	int value[m];
	leaf_node* next;
	~leaf_node();
	string name[m];
	int excur[m];
};

class list
{
public:
	list(string str,int p,list* n);
	string filename;
	int pos;
	list* next;
	void add(string str, int p);
	void fin_delete();
//private:
	//void listdelete(list* p);
};


class btree{
public:
	btree(const string&pathname);
	bool insert(string k,string value);
	bool tdelete(string str);
	bool if_exist(const string&key);
	string find(string key);
	//string findnext(string key);
	bool change(string k, string value);
	void store(string k, string value);
	void print();
	~btree();
	void check();
	//void rewind();
private:
	void check_helper(leaf_node* t);
	index_node* root;
	leaf_node* leaf_root;
	int available;  //�ļ���
	int position;  //ƫ����
	string path;
	void write();
	void read();
	void print_helper(Node* t);
	multimap<int, list*> Map;
	void fin_delete(Node* t);
	void datadelete(int str,string& name,int excur);
	void dataadd(const string& str,leaf_node* t,int j);
	void read_helper(ifstream& ifs, Node* t,leaf_node* p);
	void writer_helper(fstream& ofs,Node* t);
	void add(Node* t, const string& k, const string& v); //store
	void insert_helper(leaf_node* t,const string& k,const string& v); //��Ҷ�ӽڵ����k
	void moreadd(Node* t,Node* newnode); //���ϵݹ����
	leaf_node* find_helper(Node* t, const string&key); //����У��ҵ�leaf_node*
	void delete_helper(Node* t, const string& k); //ɾ��һ�����ж�Ҫ��Ҫ���ϵݹ�ɾ��
	void deletemove(Node* t1, Node* t2); //���ұ��ھӵĵ�һ���赽���
	void deleteunion(Node* t1,Node* t2);//��t1��t2������������һ���ھӣ��ٵ���delete_helper
	void install_key(Node* t); //��С��key����
};

#endif