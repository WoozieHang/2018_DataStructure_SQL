#pragma once

#include <iostream>
#include <fstream>
#include <assert.h>
#include <cstring>
#include <time.h>
#pragma warning(disable:4996)

using namespace std;

#define MAX_COL_SIZE 10
#define MAX_ATTR_LEN 30
#define MAX_COMMAND_SEG 20
#define MAX_COMMAND_SEG_LEN 20

class Record {

public:
	int attrc;
	char *attrv[MAX_COL_SIZE];
	Record();
	~Record();
	friend ifstream & operator >> (ifstream &input, Record &record);
	friend ofstream & operator <<(ofstream &output, Record &record);
};

class Command {
public:
	int argc;
	char *argv[MAX_COMMAND_SEG];
	Command();
	~Command();
	friend ifstream & operator >> (ifstream &input, Command &command);
};
class Heap
{
	
	int wa;//which attr
	
public:
	int num;
	Record* h[1000000];
	Heap();
	void set(int x);
	void maxinsert(Record* x);
	void maxexchange(Record* x);
	void mininsert(Record* x);
	void minexchange(Record*x);
	Record* maxfetch();
	Record* minfetch();
};
ifstream & operator >> (ifstream &input, Record &record);
ofstream & operator << (ofstream &output, Record &record);
ifstream & operator >> (ifstream &input, Command &command);
void INFO(const char* msg);
void loadData(int table, char *datafile);
int str_to_num(char* c);
void Union(int firsttable,ofstream &fout);
void Inter(int firsttable,ofstream &fout);
extern bool starttag;
