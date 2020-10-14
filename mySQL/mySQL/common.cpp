#include "common.h"
#include <cstring>
#include <assert.h>

void INFO(const char* msg) {
	cout << msg << endl;
}

Record::Record() { attrc = 0; }

Record::~Record() {
	//for (int i = 0; i < attrc; i++) {
	//	delete[] attrv[i];
	//}
}

ifstream & operator >> (ifstream &input, Record &record) {
	char tmp[MAX_ATTR_LEN * MAX_COL_SIZE], *p;
	const char * dlim = " ,:";
	int i = 0;
	input.getline(tmp, MAX_ATTR_LEN*MAX_COL_SIZE);
	p = strtok(tmp, dlim);
	while (p) {
		record.attrv[i] = new char[MAX_ATTR_LEN];
		strcpy(record.attrv[i++], p);
		p = strtok(NULL, dlim);
	}
	record.attrc = i;
	
	return input;
}

ofstream & operator << (ofstream &output, Record &record) {
	if (starttag==1) {
		output << "\n";
	}
	else starttag = 1;
	output << record.attrv[0];
	for (int i = 1; i < record.attrc; i++) {
		output << " ";
		output << record.attrv[i];
	}
	return output;
}
//void output(Record &record) {
//	for (int i = 0; i < record.attrc; i++) {
//		cout << record.attrv[i] << " ";
//	}
//	cout << "\n";
//}

Command::Command() { argc = 0; }

Command::~Command() {
	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}
}

ifstream & operator >> (ifstream &input, Command &command) {
	if (command.argc > 0) {
		for (int ii = 0; ii < command.argc; ii++) {
			delete[] command.argv[ii];
		}
		command.argc = 0;
	}

	assert(command.argc == 0);

	char tmp[MAX_ATTR_LEN * MAX_COL_SIZE], *p;
	const char * dlim = " ,:	";
	input.getline(tmp, MAX_COMMAND_SEG * MAX_COMMAND_SEG_LEN);
	p = strtok(tmp, dlim);
	int i = 0;
	while (p) {
		command.argv[i] = new char[MAX_COMMAND_SEG_LEN];
		strcpy(command.argv[i++], p);
		p = strtok(NULL, dlim);
	}
	command.argc = i;
	return input;
}


//温馨提示：如果在VS编译采用strcpy、strtok无法通过，建议在头文件处增加#pragma warning(disable:4996)