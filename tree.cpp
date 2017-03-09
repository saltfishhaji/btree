#include "tree.h"
Node::Node() :number(0), parent(NULL)
{
	for (int i = 0; i < m; ++i)
		key[i] = "0";
}

Node::~Node()
{
}

index_node::index_node()
{
	for (int i = 0; i < m; ++i)
		son[i] = NULL;
}

index_node::~index_node()
{
}

leaf_node::leaf_node()
{
	next = NULL;
}

leaf_node::~leaf_node()
{
}

btree::btree(const string&pathname)
{
	root=new index_node();
	leaf_root = new leaf_node();
	root->son[0] = leaf_root;
	leaf_root->parent = root;
	path = pathname;
	available = 0;
	position = 0;
	read();
}

btree::~btree()
{
	if (root->number == 0)
	{
		for (map<int, list*>::iterator it = Map.begin(); it != Map.end(); ++it)
		{
			it->second->fin_delete();
		}
		return;
	}
	write();
	fin_delete(root);
	for (map<int, list*>::iterator it = Map.begin(); it != Map.end(); ++it)
	{
		it->second->fin_delete();
	}
}

void list::fin_delete()
{
	//listdelete(this);
	while (this->next != NULL)
	{
		list* p = this->next;
		this->next = p->next;
		delete p;
	}
	delete this;
}

//void list::listdelete(list* p)
//{
//	if(p!=NULL)
//	{
//		listdelete(p->next);
//		delete p;
//	}
//}

void btree::fin_delete(Node* t)
{
	if(t!=NULL)
	{
		if(t->return_type()==index)
		{
			for(int i=0;i<t->number;i++)
				fin_delete(((index_node*)t)->son[i]);
			delete t;
		}
		else delete t;
	}
}

leaf_node* btree::find_helper(Node* t, const string& k)
{
	if (t!=NULL)
	{
		if (t->return_type() == index)
		{
			int i = -1;
			for (; i < t->number-1; ++i)
			{
				if (k < t->key[i + 1]) break;
			}
			if (i == -1) 
				{
					//cout<<"x";
					return NULL;
				}
			return find_helper(((index_node*)t)->son[i], k);
		}
		else
		{
			for (int i = 0; i < t->number; ++i)
			{
				//leaf_node* x = (leaf_node*)t;
				if (k == t->key[i])	return (leaf_node*)t;
				else if (k < t->key[i])	
					{
						//cout<<"y";
						return  NULL;
					}
			}
			//cout<<"z";
			return  NULL;
		}
	}
	return NULL;
}

void btree::print()
{
	print_helper(root);
	cout<<"*******************************************************\n";
}

void btree::print_helper(Node* t)
{
	if (t == NULL) return;
	for (int i = 0; i < t->number; ++i)
		cout << t->key[i] << "\t";
	cout << endl;
	if (t->return_type() == index)
	{
		for (int i = 0; i < t->number; ++i)
		{
			print_helper(((index_node*)t)->son[i]);
		}
	}
	else
	{
		for (int i = 0; i < t->number; ++i)
		{
			ifstream ifs;
			ifs.open(((leaf_node*)t)->name[i].c_str());
			ifs.seekg(((leaf_node*)t)->excur[i]);
			string temp;
			char ch;
			for (int j = 0; j < ((leaf_node*)t)->value[i]; ++j)
			{
				ifs.get(ch);
				temp += ch;
			}
			cout << temp << "\t";
		}
		cout << endl;
	}
}

bool btree::insert(string k,string v)
{
	if (if_exist(k)==true) return false; //已存在，不能插入
	//globali++;
	//check();
	add(root, k, v);
	//check();
	return true;
}

bool btree::change(string k, string v)
{
	leaf_node* x = find_helper(root, k);
	if (x == NULL) return false;
	else
	{
		for (int i = 0; i < x->number; ++i)
		{
			if (k == x->key[i])
			{
				datadelete(x->value[i], x->name[i], x->excur[i]);
				dataadd(v, x, i);
				return true;
			}
		}
		return false;
	}
}

void btree::store(string k, string v)
{
	add(root, k, v);
}

void btree::install_key(Node* t)
{
	if (t->parent == NULL) return;
	else if (((index_node*)t->parent)->son[0] == t)
	{
		if (t->parent->key[0] > t->key[0])
		{
			t->parent->key[0] = t->key[0];
			install_key(t->parent);
		}
	}
	else
	{
		for (int i = 1; i < t->parent->number; ++i)
		{
			if (((index_node*)t->parent)->son[i] == t)
			{
				if (t->parent->key[i]>t->key[0])
					t->parent->key[i] = t->key[0];
				return;
			}
		}
	}
}

void btree::check()
{
	check_helper(leaf_root);
}

void btree::check_helper(leaf_node* t)
{
	if (t->parent->number>4||t->parent->number<0)
	{
		cout<<"fuck the dog:"<<"\t";
		for(int i=0;i<t->number;++i)
			cout<<t->key[i]<<"\t";
		/*getchar();
		getchar();
		getchar();
		getchar();
		getchar();
		getchar();
		getchar();*/
		cout<<endl;
	}
	t = t->next;
	if (t != NULL)
		check_helper(t);
}


void btree::insert_helper(leaf_node* t, const string& k, const string& v)
{
	int i = 0;
	if (t->number == 0);
	else if (k > t->key[t->number - 1]) i = t->number;
	else{
		for (i = 0; i < t->number; ++i)
		{
			if (k == t->key[i])
			{
				datadelete(t->value[i], t->name[i], t->excur[i]);
				dataadd(v, t, i);
				return;
			}
			else if (k < t->key[i]) break;
		}
	}
	t->number++;
	//if(globali==39) cout<<"for 39: t->number=="<<t->number;
	//if(globali==40) cout<<"for 40: t->number=="<<t->number;
	if (t->number <= m)
	{
		//cout<<"into "<<globali<<"   t->number="<<t->number<<"\n";
		for (int j = t->number-1; j >i; j--)
		{
			t->key[j] = t->key[j - 1];
			t->value[j] = t->value[j - 1];
			t->name[j] = t->name[j - 1];
			t->excur[j] = t->excur[j - 1];
		}
		t->key[i] = k;
		t->value[i] = v.length();
		dataadd(v, t, i);
		if (i == 0) install_key(t);
	}
	else
	{
		//cout<<"into "<<globali<<"   t->number="<<t->number<<"\n";

		string newkey[m + 1] = { "" };
		int newvalue[m + 1] = { 0 };
		string newname[m + 1] = { "" };
		int newexcur[m + 1] = { 0 };
		for (int j = 0; j < i; j++)
		{
			newkey[j] = t->key[j];
			newvalue[j] = t->value[j];
			newname[j] = t->name[j];
			newexcur[j] = t->excur[j];
		}
		newkey[i] = k;
		newvalue[i] = v.length();
		for (int j = i + 1; j < t->number; j++)
		{
			newkey[j] = t->key[j - 1];
			newvalue[j] = t->value[j - 1];
			newname[j] = t->name[j - 1];
			newexcur[j] = t->excur[j - 1];
		}
		leaf_node* newnode = new leaf_node();
		newnode->number = t->number / 2;
		t->number = t->number - newnode->number;
		for (int j = i; j < t->number; j++)
		{
			t->key[j] = newkey[j];
			t->value[j] = newvalue[j];
			t->name[j] = newname[j];
			t->excur[j] = newexcur[j];
		}
		for (int j = 0; j < newnode->number; ++j)
		{
			newnode->key[j] = newkey[t->number + j];
			newnode->value[j] = newvalue[t->number + j];
			newnode->name[j] = newname[t->number + j];
			newnode->excur[j] = newexcur[t->number + j];
		}
		if (i < t->number) dataadd(v, t, i);
		else dataadd(v, newnode, i - t->number);
		newnode->next = t->next;
		t->next = newnode;
		if (i == 0) install_key(t);
		moreadd(t, newnode);
	}
}

void btree::add(Node* t, const string& k, const string& v)
{
	if (t != NULL)
		//if (k < t->key[0]) return pair<Node*,string>(NULL,"");
	{
		if (t->return_type() == index)
		{
			//index_node* t = (index_node*)t;
			int i =0;
			if (t->number == 0) t->number++;
			else
			{
				for (; i < t->number - 1; ++i)
				{
					if (k < t->key[i + 1]) break;
				}
			}
				//if (k >= ((index_node*)t)->key[t->number - 1]) i = t->number-1;
				//else while (k > ((index_node*)t)->key[i + 1]) i++;
			add(((index_node*)t)->son[i], k, v);
		}
		else insert_helper((leaf_node*)t, k, v);
	}
}

void btree::moreadd(Node* t, Node* newnode)
{
	//cout<<"intomoreadd: "<<globali<<"\n";
	if (newnode == NULL)
	{
		//cout<<"hehe "<<globali<<"\n";
		return;
	}
	if (t->parent == NULL)
	{
		//cout<<"shit "<<globali<<"\n";
		//check();
		index_node* x = new index_node();
		t->parent = x;
		newnode->parent = x;
		x->son[0] = t;
		x->son[1] = newnode;
		x->key[0] = t->key[0];
		x->key[1] = newnode->key[0];
		x->number = 2;
		root = x;
		return;
	}
	int i = 0;
	for (i = 0; i < t->parent->number; i++)
	{
		if (t->key[0] < t->parent->key[i])
		{
			//i--;
			break;
		}
	}
	//i++; //i是newnode的排序
	if (t->parent->number < m)
	{
		//cout<<"into111 "<<globali<<":   t->parent->number="<<t->parent->number<<"    i="<<i<<"\n";
		t->parent->number++;
		for (int j = t->parent->number-1; j >i; j--)
		{
			t->parent->key[j] = t->parent->key[j - 1];
			((index_node*)t->parent)->son[j] = ((index_node*)t->parent)->son[j - 1];
		}
		t->parent->key[i] = newnode->key[0];
		((index_node*)t->parent)->son[i] = newnode;
		newnode->parent = t->parent;
		return;
	}
	else
	{
		//cout<<"into222 "<<globali<<":   t->parent->number="<<t->parent->number<<"    i="<<i<<"\n";
		string newkey[m + 1] = { "" };
		Node* newson[m + 1] = { NULL };
		for (int j = 0; j < i; j++)
		{
			newkey[j] = t->parent->key[j];
			newson[j] = ((index_node*)t->parent)->son[j];
		}
		newkey[i] = newnode->key[0];
		newson[i] = newnode;
		for (int j = i; j < t->parent->number; ++j)
		{
			newkey[j+1] = t->parent->key[j];
			newson[j+1] = ((index_node*)t->parent)->son[j];
		}
		index_node* x = new index_node();
		t->parent->number++;
		x->number = t->parent->number / 2;
		t->parent->number = t->parent->number - x->number;
		for (int j = i; j < t->parent->number; j++)
		{
			t->parent->key[j] = newkey[j];
			//if (t->return_type==index)
			newson[j]->parent=t->parent;
			((index_node*)t->parent)->son[j] = newson[j];
		}
		Node* temp = t->parent;
		for (int j = 0; j < x->number; ++j)
		{
			x->key[j] = newkey[temp->number + j];
			x->son[j] = newson[temp->number + j];
			x->son[j]->parent = x;
		}
		return moreadd(temp,x);
	}
}

bool btree::tdelete(string str)
{
	//globali++;
	//cout << globali << "\t";
	//if (globali == 27488) getchar();
	leaf_node* temp = find_helper(root, str);
	if (temp == NULL) return false;
	else delete_helper((index_node*)temp, str);
	return true;
}

bool btree::if_exist(const string& key)
{
	leaf_node* temp = find_helper(root, key);
	if (temp == NULL)return false;
	return true;
}

string btree::find(string k)
{
	leaf_node* temp = find_helper(root, k);
	if (temp == NULL) return "";
	for (int i=0; i < temp->number; ++i)
	{
		if (temp->key[i] == k)
		{
			string str;
			char ch;
			ifstream ifs;
			ifs.open(temp->name[i].c_str());
			ifs.seekg(temp->excur[i]);
			for (int j = 0; j < temp->value[i]; ++j)
			{
				ifs.get(ch);
				str += ch;
			}
			return str;
		}
	}
	return "";
}

//string btree::findnext(string key)
//{
//	pair<Node*, string> x = find_helper(root, key);
//	if (x.first == NULL) return "";
//	for (int i = 0; i < x.first->number; ++i)
//	{
//		if (key == ((leaf_node*)x.first)->value[i])
//		{
//			if (i != x.first->number-1)
//				return ((leaf_node*)x.first)->value[++i];
//			else if (((leaf_node*)x.first)->next!=NULL)
//				return ((leaf_node*)x.first)->next->value[0];
//			else return "";
//		}
//	}
//}

void btree::delete_helper(Node* t, const string& k)
{
	//;cout<<"fuck";
	int i = 0;
	for (i = 0; i < t->number; i++)
	{
		if (k == t->key[i]) break;
	}
	t->number--;
	if (t->return_type() == leaf)
	{
		datadelete(((leaf_node*)t)->value[i], ((leaf_node*)t)->name[i], ((leaf_node*)t)->excur[i]);
		for (int j=i; j < t->number; j++)
		{
			t->key[j] = t->key[j + 1];
			((leaf_node*)t)->value[j] = ((leaf_node*)t)->value[j + 1];
			((leaf_node*)t)->name[j] = ((leaf_node*)t)->name[j + 1];
			((leaf_node*)t)->excur[j] = ((leaf_node*)t)->excur[j + 1];
		}
	}
	else
	{
		for (int j=i; j < t->number; j++)
		{
			t->key[j] = t->key[j + 1];
			((index_node*)t)->son[j] = ((index_node*)t)->son[j + 1];
		}
	}
	//if (i == 0) install_key(t);
	if (t->number >= m / 2) return;
	else if (t == root)
	{
		if (t->number == 1 && root->son[0]->return_type()==index)
		{
			root = (index_node*)root->son[0];
			delete t;
		}
		return;
	}
	else if (t->parent->number>1)
	{
		i = 0;
		for (; i < t->parent->number; ++i)
		{
			if (((index_node*)t->parent)->son[i] == t)break;
		}
		if (i>0 && ((index_node*)t->parent)->son[i - 1]->number > m / 2)
		{
			for (int j = t->number; j > 0; --j)
			{
				t->key[j] = t->key[j - 1];
				if (t->return_type() == leaf)
				{
					((leaf_node*)t)->value[j] = ((leaf_node*)t)->value[j - 1];
					((leaf_node*)t)->name[j] = ((leaf_node*)t)->name[j - 1];
					((leaf_node*)t)->excur[j] = ((leaf_node*)t)->excur[j - 1];
				}
				else
					((index_node*)t)->son[j] = ((index_node*)t)->son[j-1];
			}
			t->number++;
			Node* p = ((index_node*)t->parent)->son[i - 1];
			p->number--;
			t->key[0] = p->key[p->number];
			if (t->return_type() == leaf)
			{
				((leaf_node*)t)->value[0] = ((leaf_node*)p)->value[p->number];
				((leaf_node*)t)->name[0] = ((leaf_node*)p)->name[p->number];
				((leaf_node*)t)->excur[0] = ((leaf_node*)p)->excur[p->number];
			}
			else
			{
				((index_node*)t)->son[0] = ((index_node*)p)->son[p->number];
				((index_node*)t)->son[0]->parent = t;
			}
			install_key(t);
		}
		else if (i + 1 < t->parent->number && ((index_node*)t->parent)->son[i + 1]->number > m / 2)
		{
			deletemove(t, ((index_node*)t->parent)->son[i + 1]);
			t->parent->key[i + 1] = ((index_node*)t->parent)->son[i + 1]->key[0];
		}
		else
		{
			if (i < t->parent->number - 1)
				deleteunion(t, ((index_node*)t->parent)->son[i + 1]);
			else	deleteunion(((index_node*)t->parent)->son[i - 1], t);
			
		}
	}
	else if (t == leaf_root)
	{
		if (t->number == 0)
			root->number = 0;
		return;
	}
	else
	{
		cout << "Ooooooops!!!!unknown error!/n";
		//system("pause");
		//exit(1);
	}
	//cout<<"dog";
}

void btree::deletemove(Node* t1, Node* t2)
{
	t1->key[t1->number] = t2->key[0];
	if (t1->return_type() == leaf)
	{
		/*leaf_node* t1 = (leaf_node*)t1;
		leaf_node* t2 = (leaf_node*)t2;*/
		((leaf_node*)t1)->value[t1->number] = ((leaf_node*)t2)->value[0];
		((leaf_node*)t1)->name[t1->number] = ((leaf_node*)t2)->name[0];
		((leaf_node*)t1)->excur[t1->number] = ((leaf_node*)t2)->excur[0];
		for (int i = 1; i < t2->number; i++)
		{
			t2->key[i - 1] = t2->key[i];
			((leaf_node*)t2)->value[i - 1] = ((leaf_node*)t2)->value[i];
			((leaf_node*)t2)->name[i - 1] = ((leaf_node*)t2)->name[i];
			((leaf_node*)t2)->excur[i - 1] = ((leaf_node*)t2)->excur[i];
		}
	}
	else
	{
		/*index_node* t1 = (index_node*)t1;
		index_node* t2 = (index_node*)t2;*/
		((index_node*)t1)->son[t1->number] = ((index_node*)t2)->son[0];
		((index_node*)t1)->son[t1->number]->parent = t1;
		for (int i = 1; i < t2->number; i++)
		{
			t2->key[i - 1] = t2->key[i];
			((index_node*)t2)->son[i - 1] = ((index_node*)t2)->son[i];
		}
	}
	t1->number++;
	t2->number--;
	install_key(t2);
}

void btree::deleteunion(Node* t1, Node* t2)
{
	if (t1->return_type() == leaf)
	{
		((leaf_node*)t1)->next = ((leaf_node*)t2)->next;
		for (int i = 0; i < t2->number; ++i)
		{
			t1->key[t1->number + i] = t2->key[i];
			((leaf_node*)t1)->value[t1->number + i] = ((leaf_node*)t2)->value[i];
			((leaf_node*)t1)->name[t1->number + i] = ((leaf_node*)t2)->name[i];
			((leaf_node*)t1)->excur[t1->number + i] = ((leaf_node*)t2)->excur[i];
		}
	}
	else
	{
		for (int i = 0; i < t2->number; ++i)
		{
			t1->key[t1->number + i] = t2->key[i];
			((index_node*)t1)->son[t1->number + i] = ((index_node*)t2)->son[i];
			((index_node*)t1)->son[t1->number + i]->parent = t1;
		}
	}
	t1->number = t1->number + t2->number;
	//t1->parent->number--;
	delete t2;
	int i = 0;
	//t1 = t1->parent;
	for ( i = 0; i < t1->parent->number; i++)
	{
		if (((index_node*)t1->parent)->son[i] == t1) break;
	}
	delete_helper(t1->parent, t1->parent->key[++i]);
}

//void btree::rewind()
//{
//	leaf_node* p = leaf_root;
//	while (p != NULL)
//	{
//		for (int i = 0; i < p->number; ++i)
//			cout << p->value[i] << "\t";
//		p = p->next;
//	}
//}

void btree::read()
{
	ifstream ifs;
	ifs.open(("index/" + path + ".idx").c_str());
	if (!ifs) return;
	ifs >> position >> available;
	int count = 0;
	int lengt = 0;
	int pos = 0;
	int n = 0;
	string name;
	ifs >> count;
	for (int i = 0; i < count; ++i)
	{
		ifs >> lengt >> n >> name >> pos;
		map<int, list*>::iterator it;
		it =Map.insert(pair<int, list*>(lengt, new list(name, pos, 0)));
		for (int i = 0; i < n - 1; ++i)
		{
			ifs >> name >> pos;
			it->second->add(name, pos);
		}
	}
	read_helper(ifs, root,leaf_root);
	leaf_node* x = leaf_root->next;
	if (x != NULL)
	{
		delete leaf_root;
		leaf_root = x;
	}
	ifs.close();
}

void btree::read_helper(ifstream& ifs, Node* t,leaf_node* p)
{
	//int lengt = 0;
	//ifs.seekp(pos);
	char ch1,ch2;
	ifs.get(ch1);
	if (ch1 == INDEX)
		ifs.get(ch2);
	else if (ch1 == '\t')
	{
		ifs.get(ch1);
		if (ch1 == INDEX)
			ifs.get(ch2);
	}

	ifs >> t->number;
	//ifs >> lengt;
	//string temp;
	//for (int i = 0; i < lengt; ++i)
	//{
	//	ifs.get(ch);
	//	temp += ch;
	//}
	//stringstream ss(temp);
	if (ch1 == INDEX)
	{
		for (int i = 0; i < t->number; ++i)
		{
			ifs >> t->key[i];
			//ifs >> pos;
			if (ch2 == INDEX)
				((index_node*)t)->son[i] = new index_node(); //儿子是index
			else
			{
				((index_node*)t)->son[i] = new leaf_node();  //儿子是leaf
				p->next = (leaf_node*)((index_node*)t)->son[i];
				p = p->next;
			}
			((index_node*)t)->son[i]->parent = t;
			read_helper(ifs,((index_node*)t)->son[i], p);
		}
	}
	else if (ch1 == LEAF)
	{
		for (int i = 0; i < t->number; ++i)
		{
			ifs >> t->key[i];
			ifs >> ((leaf_node*)t)->name[i];
			ifs >> ((leaf_node*)t)->excur[i];
			ifs >> ((leaf_node*)t)->value[i];
		}
	}
	else
	{
		cerr << "ooooops!unknown char for read...\n";
		//system("pause");
		//exit(1);
	}
}

void btree::datadelete(int str, string& name, int excur)
{
	if (excur == 29) getchar();
	map<int, list*>::iterator it;
	it = Map.find(str);
	if (it == Map.end())
		Map.insert(pair<int, list*>(str, new list(name,excur,0)));
	else
		it->second->add(name, excur);
}

void btree::dataadd(const string&str, leaf_node* t,int j)
{
	t->value[j] = str.length();
	for (map<int, list*>::iterator it = Map.begin(); it != Map.end(); ++it)
	{
		if (it->first >= t->value[j])
		{
			t->excur[j] = it->second->pos;
			t->name[j] = it->second->filename;
			if (it->first > t->value[j])
				datadelete(it->first - t->value[j], t->name[j], t->excur[j] + t->value[j]);
			fstream ofs;
			ofs.open(t->name[j].c_str());  //t->name[j]理论上肯定存在
			/*if (!ofs.is_open())
			{
			fstream os;
			os.open(t->name[j].c_str(), fstream::app);
			os.close();
			ofs.open(t->name[j].c_str());
			}*/
			ofs.seekp(t->excur[j]);
			ofs << str;
			ofs.close();
			if (it->second->next == NULL)   //就一个list* p
			{
				delete it->second;
				Map.erase(it);
				return;
			}
			list* p = it->second;   //一串list* p
			it->second = it->second->next;
			delete p;
			/*if (str.length() < i->first)
			datadelete(i->first - str.length(), t->name[j], t->excur[j]);
			Map.erase(i);*/
			return;
		}
	}
	fstream ofs;
	stringstream ss;
	ss << available;
	string str1 = ss.str();
	t->name[j] = "data\\" + path + str1 + ".dat";
	t->excur[j] = position;
	ofs.open(t->name[j].c_str());
	if (!ofs.is_open())
	{
		fstream os;
		os.open(t->name[j].c_str(),fstream::app);
		os.close();
		ofs.open(t->name[j].c_str());
		if (!ofs.is_open()) cout << "还是没打开\n";
	}
	ofs.seekp(position);
	position += str.length();
	ofs << str;
	ofs.close();
	if (position > 131072)
	{
		available += 1;
		position = 0;
	}
}

void btree::write()
{
	fstream ofs;
	ofs.open(("index\\" + path + ".idx").c_str(),fstream::out|fstream::trunc|fstream::binary);
	if (!ofs.is_open())
	{
		fstream os;
		os.open(("index\\" + path + ".idx").c_str(), fstream::app);
		os.close();
		ofs.open(("index\\" + path + ".idx").c_str(), fstream::out | fstream::trunc | fstream::binary);
	}
	ofs << position << '\t' << available << '\t' << Map.size()<<'\t';
	map<int, list*>::iterator it;
	for (it = Map.begin(); it != Map.end(); ++it)
	{
		ofs << it->first << '\t';
		int count = 0;
		list* p = it->second;
		while (p != NULL)
		{
			p = p->next;
			count++;
		}
		ofs << count << '\t';
		p = it->second;
		for (int i = 0; i < count; i++)
		{
			ofs << p->filename << '\t' << p->pos << '\t';
			p = p->next;
		}
	}
	writer_helper(ofs, root);
	ofs.close();
}

void btree::writer_helper(fstream&ofs, Node* t)
{
	if (t->return_type() == leaf) 
	{
		//#define leaf '1'
		ofs << LEAF << t->number << '\t';
		for (int i = 0; i < t->number; ++i)
		{
			ofs << t->key[i] << '\t' << ((leaf_node*)t)->name[i] << '\t'
				<< ((leaf_node*)t)->excur[i] << '\t' << ((leaf_node*)t)->value[i] << '\t';
		}
	}
	else
	{
		ofs <<INDEX;
		if (t->number == 0)
		{
			ofs << INDEX<< '0' << '\n';
			return;
		}
		if (((index_node*)t)->son[0]->return_type() == leaf)
			ofs << LEAF;
		else ofs << INDEX;
		ofs << t->number << '\t';
		for (int i = 0; i < t->number; ++i)
		{
			ofs << t->key[i]<<'\t';
			writer_helper(ofs, ((index_node*)t)->son[i]);
		}
	}
}

list::list(string str, int p, list* n):filename(str),pos(p),next(n){}

void list::add(string str, int p)
{
	list* t = new list(str, p, 0);
	t->next = this->next;
	this->next = t;
}