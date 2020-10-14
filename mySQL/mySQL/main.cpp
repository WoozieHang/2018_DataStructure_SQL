#include "common.h"
#include "test.h"
#include "database.h"
MyDatabase mydatabase1;
MyDatabase mydatabase2;
char name1[1000000][30];
char name2[1000000][30];
int namenum1 = 0;
int namenum2 = 0;
bool starttag;
Heap::Heap()
{
	wa = -1;
	num = 0;
}
void Heap::set(int x) {
	wa = x;
}

void Heap::maxinsert(Record* x) {
	h[num] = x;
	int index = num;
	num++;
	int up = (index - 1) / 2;
	while (up >= 0 && mydatabase1.cmp(wa, h[up], x, 1) == 1) {
		h[index] = h[up];
		index = up;
		up = up == 0 ? -1 : (up - 1) / 2;
	}
	h[index] = x;
}
void Heap::mininsert(Record* x) {
	h[num] = x;
	int index = num;
	num++;
	int up = (index - 1) / 2;
	while (up >=0 && mydatabase1.cmp(wa, h[up], x, 0) == 1) {
		h[index] = h[up];
		index = up;
		up = up==0?-1:(up - 1) / 2;
	}
	h[index] = x;
}
void Heap::maxexchange(Record* x) {
	h[0] = x;
	int index = 0;
	int dl = 1;
	int dr = 2;
	int d;
	if(2<num) d = mydatabase1.cmp(wa, h[dl], h[dr], 1) == 0 ? dl : dr;
	else d = 1;
	while (d < num && mydatabase1.cmp(wa, x, h[d], 1) == 1){
		h[index] = h[d];
		index = d;
		dl = 2 * index + 1;
		dr = 2 * index + 2;
		if(dr<num)
			d= mydatabase1.cmp(wa, h[dl], h[dr], 1) == 0 ? dl : dr;
		else d = dl;
	}
	h[index] = x;
}
void Heap::minexchange(Record* x) {
	h[0] = x;
	int index = 0;
	int dl = 1;
	int dr = 2;
	int d;
	if (2<num) d = mydatabase1.cmp(wa, h[dl], h[dr], 0) == 0 ? dl : dr;
	else d = 1;
	while (d < num && mydatabase1.cmp(wa, x, h[d], 0) == 1) {
		h[index] = h[d];
		index = d;
		dl = 2 * index + 1;
		dr = 2 * index + 2;
		if (dr<num)
			d = mydatabase1.cmp(wa, h[dl], h[dr], 0) == 0 ? dl : dr;
		else d = dl;
	}
	h[index] = x;
}
Record* Heap::maxfetch() {
	Record* m= h[0];
	h[0] = h[num - 1];
	Record* x = h[num - 1];
	num--;
	int index = 0;
	int dl = 1;
	int dr = 2;
	int d;
	if (2<num) d = mydatabase1.cmp(wa, h[dl], h[dr], 1) == 0 ? dl : dr;
	else d = 1;
	while (d < num && mydatabase1.cmp(wa, x, h[d], 1) == 1) {
		h[index] = h[d];
		index = d;
		dl = 2 * index + 1;
		dr = 2 * index + 2;
		if (dr<num)
			d = mydatabase1.cmp(wa, h[dl], h[dr], 1) == 0 ? dl : dr;
		else d = dl;
	}
	h[index] = x;
	return m;
}
Record* Heap::minfetch() {
	Record* m = h[0];
	h[0] = h[num - 1];
	Record* x = h[num - 1];
	num--;
	int index = 0;
	int dl = 1;
	int dr = 2;
	int d;
	if (2<num) d = mydatabase1.cmp(wa, h[dl], h[dr], 0) == 0 ? dl : dr;
	else d = 1;
	while (d < num && mydatabase1.cmp(wa, x, h[d], 0) == 1) {
		h[index] = h[d];
		index = d;
		dl = 2 * index + 1;
		dr = 2 * index + 2;

		if (dr<num)
			d = mydatabase1.cmp(wa, h[dl], h[dr], 0) == 0? dl : dr;
		else d = dl;
	}
	h[index] = x;
	return m;
}
int str_to_num(char* c) {
	int index = 1;
	int ans = c[0] - '0';
	while (c[index] != '\0') {
		ans = ans * 10 + c[index] - '0';
		index++;
	}
	return ans;
}
void Tester::helper(Command &command) {
	//TODO
	//recognize and execute command
	if (strcmp(command.argv[0], "INSERT") == 0) {
		Record a_record;
		a_record.attrc = command.argc - 1;
		//id 列去左括号
		a_record.attrv[0] = NULL;
		a_record.attrv[0] = new char[MAX_ATTR_LEN];
		int k = 1;
		for (; command.argv[1][k] != '\0'; k++) {
			a_record.attrv[0][k - 1] = command.argv[1][k];
			a_record.attrv[0][k] = '\0';
		}
		
		//最后一列去右括号
		a_record.attrv[a_record.attrc - 1] = NULL;
		a_record.attrv[a_record.attrc - 1] = new char[MAX_ATTR_LEN];
		k = 0;
		for (; command.argv[a_record.attrc][k] != ')'; k++) {
			a_record.attrv[a_record.attrc - 1][k] = command.argv[a_record.attrc][k];
		}
		a_record.attrv[a_record.attrc - 1][k] = '\0';
		//中间照搬即可
		for (int i = 1; i < a_record.attrc - 1; i++) {
			a_record.attrv[i] = new char[MAX_ATTR_LEN];
			strcpy(a_record.attrv[i], command.argv[i + 1]);
		}

		mydatabase1.insert(a_record);
	}
	else if (strcmp(command.argv[0], "DELETE") == 0) {
		if (command.argc == 2) {
			char* id = command.argv[1];
			mydatabase1.del(id);
		}
		else {
			char* id1 = command.argv[1];
			char* id2 = command.argv[2];
			mydatabase1.del(id1, id2);
		}
	}
	else if (strcmp(command.argv[0], "SET") == 0) {
		if (command.argc == 4) {
			char* id = command.argv[1];
			char* keych = command.argv[2];
			char* value = command.argv[3];
			mydatabase1.set(id, keych, value);
		}
		else {
			char* id1 = command.argv[1];
			char* id2 = command.argv[2];
			char* keych = command.argv[3];
			char* value = command.argv[4];
			mydatabase1.set(id1, id2, keych, value);
		}
	}
	else if (strcmp(command.argv[0], "ADD") == 0) {
		if (command.argc == 4) {
			char* id = command.argv[1];
			char* keych = command.argv[2];
			char* value = command.argv[3];
			mydatabase1.add(id, keych, value);
		}
		else {
			char* id1 = command.argv[1];
			char* id2 = command.argv[2];
			char* keych = command.argv[3];
			char* value = command.argv[4];
			mydatabase1.add(id1, id2, keych, value);
		}
	}
	//fout 作为引用传入函数
	else if (strcmp(command.argv[0], "QUERY") == 0) {
		if (command.argc == 2) {
			mydatabase1.query_id(command.argv[1], fout);
		}
		else if (strcmp(command.argv[1], "name") == 0) {
			mydatabase1.query_name(command.argv[3], fout);
		}
		else if (strcmp(command.argv[2], ">") == 0) {
			mydatabase1.query_key(command.argv[1],0, command.argv[3],fout);
		}
		else if (strcmp(command.argv[2], ">=") == 0) {
			mydatabase1.query_key(command.argv[1], 1, command.argv[3], fout);
		}
		else if (strcmp(command.argv[2], "<") == 0) {
			mydatabase1.query_key(command.argv[1], 2, command.argv[3], fout);
		}
		else if (strcmp(command.argv[2], "<=") == 0) {
			mydatabase1.query_key(command.argv[1], 3, command.argv[3], fout);
		}
		else if (strcmp(command.argv[2], "=") == 0) {
			mydatabase1.query_key(command.argv[1], 4, command.argv[3], fout);
		}
		else if (strcmp(command.argv[2], "!=") == 0) {
			mydatabase1.query_key(command.argv[1], 5, command.argv[3], fout);
		}
		else if (strcmp(command.argv[4], ">") == 0) {
			mydatabase1.query_key(command.argv[1], command.argv[2], command.argv[3], 0, command.argv[5], fout);
		}
		else if (strcmp(command.argv[4], ">=") == 0) {
			mydatabase1.query_key(command.argv[1], command.argv[2], command.argv[3], 1, command.argv[5], fout);
		}
		else if (strcmp(command.argv[4], "<") == 0) {
			mydatabase1.query_key(command.argv[1], command.argv[2], command.argv[3], 2, command.argv[5], fout);
		}
		else if (strcmp(command.argv[4], "<=") == 0) {
			mydatabase1.query_key(command.argv[1], command.argv[2], command.argv[3], 3, command.argv[5], fout);
		}
		else if (strcmp(command.argv[4], "=") == 0) {
			mydatabase1.query_key(command.argv[1], command.argv[2], command.argv[3], 4, command.argv[5], fout);
		}
		else if (strcmp(command.argv[4], "!=") == 0) {
			mydatabase1.query_key(command.argv[1], command.argv[2], command.argv[3], 5, command.argv[5], fout);
		}
		else if (command.argc == 4) {
			int k = str_to_num(command.argv[2]);
			if (strcmp(command.argv[3], "ASC") == 0)
				mydatabase1.query_the_max(command.argv[1], k, fout);
			else
				mydatabase1.query_the_min(command.argv[1], k, fout);
		}
		else if (command.argc == 5) {
			int k = str_to_num(command.argv[2]);
			if (strcmp(command.argv[4], "ASC") == 0)
				mydatabase1.query_max(command.argv[1], k, fout);
			else
				mydatabase1.query_min(command.argv[1], k, fout);
		}
		else if (command.argc == 6) {
			int k = str_to_num(command.argv[4]);
			
			if (strcmp(command.argv[5], "ASC") == 0)
				mydatabase1.query_the_max(command.argv[1],command.argv[2],command.argv[3], k, fout);
			else
				mydatabase1.query_the_min(command.argv[1], command.argv[2], command.argv[3], k, fout);
		}
		else if (command.argc == 7) {
			int k = str_to_num(command.argv[4]);
			
			if (strcmp(command.argv[6], "ASC") == 0) {
				mydatabase1.query_max(command.argv[1], command.argv[2], command.argv[3], k, fout);
			}
			else
				mydatabase1.query_min(command.argv[1], command.argv[2], command.argv[3], k, fout);
		}
	}
	else if (strcmp(command.argv[0], "SUM") == 0) {
		mydatabase1.sum(command.argv[1], command.argv[2], command.argv[3],fout);
	}
	else if (strcmp(command.argv[0], "UNION") == 0) {
		if (strcmp(command.argv[1], mydatabase1.tablename) == 0)
			Union(1, fout);
		else
			Union(2, fout);
	}
	else if (strcmp(command.argv[0], "INTER") == 0) {
		if (strcmp(command.argv[1], mydatabase1.tablename) == 0)
			Inter(1, fout);
		else
			Inter(2, fout);
	}
}


void change(char name[][30], MyDatabase &mdb,int which) {
	int num = 0;
	node* x = mdb.head->link;
	bool whether_insert=0;
	for (int i = 0; i < mdb.num; i++) {
		whether_insert = 1;
		for (int j = 0; j < num; j++) {
			if (strcmp(name[j], x->record.attrv[1]) == 0)
			{
				whether_insert = 0;
				break;
			}
		}
		if (whether_insert) {
			strcpy(name[num], x->record.attrv[1]);
			num++;
		}
		x = x->link;
	}
	if (which == 1)
		namenum1 = num;
	else namenum2 = num;
}
void loadData(int table,const char *file) {
	//TODO
	ifstream fin_data;
	fin_data.open(file);
	if (table == 1) {
		Record a;
		fin_data >> a;
		strcpy(mydatabase1.tablename, a.attrv[0]);
		mydatabase1.build(fin_data);
	}
	else {
		Record a;
		fin_data >> a;
		strcpy(mydatabase2.tablename, a.attrv[0]);
		mydatabase2.build(fin_data);
	}
	fin_data.close();
	//load data file
}

void Union(int firsttable,ofstream& fout) {
	if (firsttable == 1) {
		if (namenum1 > 0) {
			if (starttag == 1)
				fout << "\n";
			else starttag = 1;
			fout << name1[0];
		}
		for (int i = 1; i < namenum1; i++) {
			fout << "\n" << name1[i];
		}
		for (int i = 0; i < namenum2; i++) {
			bool repeat = 0;
			for (int j = 0; j < namenum1; j++) {
				if (strcmp(name1[j], name2[i]) == 0) {
					repeat = 1;
					break;
				}
			}
			if (repeat == 0) {
				fout << "\n" << name2[i];
			}
		}
	}
	else {
		if (namenum2 > 0) {
			if (starttag == 1)
				fout << "\n";
			else starttag = 1;
			fout << name2[0];
		}
		for (int i = 1; i < namenum2; i++) {
			fout << "\n" << name2[i];
		}
		for (int i = 0; i < namenum1; i++) {
			bool repeat = 0;
			for (int j = 0; j < namenum2; j++) {
				if (strcmp(name2[j], name1[i]) == 0) {
					repeat = 1;
					break;
				}
			}
			if (repeat == 0) {
				fout << "\n" << name1[i];
			}
		}
	}
}
void Inter(int firsttable,ofstream& fout) {
	if (firsttable == 1) {
		for (int i = 0; i < namenum1; i++) {
			bool repeat = 0;
			for (int j = 0; j < namenum2; j++) {
				if (strcmp(name1[i], name2[j]) == 0) {
					repeat = 1;
					break;
				}
			}
			if (repeat == 1) {
				if (starttag == 1)
					fout << "\n";
				else starttag = 1;
				fout << name1[i];
			}
		}
	}
	else {
		for (int i = 0; i < namenum2; i++) {
			bool repeat = 0;
			for (int j = 0; j < namenum1; j++) {
				if (strcmp(name2[i], name1[j]) == 0) {
					repeat = 1;
					break;
				}
			}
			if (repeat == 1) {
				if (starttag == 1)
					fout << "\n";
				else starttag = 1;
				fout << name2[i];
			}
		}
	}
}


int TEST_TYPE = 0;


int main(int argc, char const *argv[])
{

	/* code */
	Timer timer;
	timer.tic("All");

	SingleTester single_tester("single.txt", "result_single.txt");
	SectionTester section_tester1("section1.txt", "result_section1.txt");
	SectionTester section_tester2("section2.txt", "result_section2.txt");
	SetTester set_tester("set.txt", "result_set.txt");

	//单点测试
	starttag = 0;
	TEST_TYPE = 0;
	loadData(1,"data_single.txt");
	INFO("=====================================");
	single_tester.exec();
	mydatabase1.DelAll();
	INFO("=====================================\n");

	//区间测试1
	starttag = 0;
	TEST_TYPE = 1;
	loadData(1,"data_section1.txt");
	INFO("=====================================");
	section_tester1.exec();
	mydatabase1.DelAll();
	INFO("=====================================\n");

	//区间测试2
	starttag = 0;
	TEST_TYPE = 2;
	loadData(1,"data_section2.txt");
	INFO("=====================================");
	section_tester2.exec();
	mydatabase1.DelAll();
	INFO("=====================================\n");

	//集合测试
	starttag = 0;
	TEST_TYPE = 3;
	loadData(1,"data_set1.txt");
	loadData(2,"data_set2.txt");
	change(name1, mydatabase1, 1);
	change(name2, mydatabase2, 2);
	INFO("=====================================");
	set_tester.exec();
	mydatabase1.DelAll();
	mydatabase2.DelAll();
	INFO("=====================================\n");

	timer.toc();
	INFO("You are a genius!");
	system("pause");
	return 0;
}



