#pragma once

#include "common.h"
struct node {
	Record record;
	node* link;
};
class MyDatabase {
	node* head;//根据id顺序构建的链表,表头不放东西
	node* tail;
	//存储所有列的按大到小的排序，链表有多长表示找到前k大的
	//存储所有列的按小到大的排序，链表有多长表示找到前k小的
	char colname[MAX_COL_SIZE][MAX_ATTR_LEN];//每个项的名字
	int num;//多少条记录
	int colnum;//多少个列，不包括名字和id
public:
	char tablename[20];
	MyDatabase();
	void setattr(Record r);
	void build(ifstream &fin_data);//通过fin_data文件构造整个类
	void insert(Record record);//插到链表中，在建表时用不到，但是操作command时候要用
	bool cmp(int which_attr, Record* a, Record* b,bool is_min);//对于第which_attr个属性，比较a和b指向对象的该属性(id为次属性)哪个更大，a大返回1，否则返回0
	void DelAll();//析构
	void del(char* id);
	void del(char* id1, char* id2);
	int cmpch(char* a, char* b);//1,0,-1
	void set(char* id, char* keych, char* value);//把id的记录的第key个属性设置成value
	void add(char* id, char* keych, char* value);
	void set(char* id1,char* id2, char* keych, char* value);
	void add(char* id1,char* id2, char* keych, char* value);
	void chadd(char* dest, char* src);
	void query_id(char* id,ofstream &fout);
	void query_name(char* name, ofstream &fout);
	void query_key(char* keych, int symb, char* value, ofstream &fout);
	void query_key(char* id1,char*id2,char* keych, int symb, char* value, ofstream &fout);
	void query_the_max(char* keych, int k, ofstream &fout);
	void query_the_min(char* keych, int k, ofstream &fout);
	void query_the_max(char* id1,char*id2,char* keych, int k, ofstream &fout);
	void query_the_min(char*id1,char*id2,char* keych, int k, ofstream &fout);
	void query_max(char* keych, int k, ofstream &fout);
	void query_min(char* keych, int k, ofstream &fout);
	void query_max(char* id1, char*id2, char* keych, int k, ofstream &fout);
	void query_min(char*id1, char*id2, char* keych, int k, ofstream &fout);
	void sum(char* id1, char* id2, char * keych, ofstream &fout);
	friend void change(char name[][30], MyDatabase& mdb,int which);
};

void change(char name[][30], MyDatabase& mdb, int which);