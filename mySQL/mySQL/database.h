#pragma once

#include "common.h"
struct node {
	Record record;
	node* link;
};
class MyDatabase {
	node* head;//����id˳�򹹽�������,��ͷ���Ŷ���
	node* tail;
	//�洢�����еİ���С�����������ж೤��ʾ�ҵ�ǰk���
	//�洢�����еİ�С��������������ж೤��ʾ�ҵ�ǰkС��
	char colname[MAX_COL_SIZE][MAX_ATTR_LEN];//ÿ���������
	int num;//��������¼
	int colnum;//���ٸ��У����������ֺ�id
public:
	char tablename[20];
	MyDatabase();
	void setattr(Record r);
	void build(ifstream &fin_data);//ͨ��fin_data�ļ�����������
	void insert(Record record);//�嵽�����У��ڽ���ʱ�ò��������ǲ���commandʱ��Ҫ��
	bool cmp(int which_attr, Record* a, Record* b,bool is_min);//���ڵ�which_attr�����ԣ��Ƚ�a��bָ�����ĸ�����(idΪ������)�ĸ�����a�󷵻�1�����򷵻�0
	void DelAll();//����
	void del(char* id);
	void del(char* id1, char* id2);
	int cmpch(char* a, char* b);//1,0,-1
	void set(char* id, char* keych, char* value);//��id�ļ�¼�ĵ�key���������ó�value
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