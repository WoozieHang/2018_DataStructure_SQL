#include "database.h"
MyDatabase::MyDatabase() {
	head = NULL;
	tail = NULL;
	num = 0;
	colnum = 0;
}
void MyDatabase::setattr(Record r) {
	colnum = r.attrc-2;
	for (int i = 0; i < colnum; i++) {
		strcpy(colname[i], r.attrv[i + 2]);
	}

}
void MyDatabase::build(ifstream &fin_data) {
	head = new node;
	head->link = NULL;
	tail = head;
	num = 0;
	colnum = 0;
//从文件读入链表
	Record record;
	fin_data >> record;
	setattr(record);
	while (fin_data >> record) {
		//插到链表
		tail->link = new node;
		tail = tail->link;
		tail->link = NULL;
		tail->record = record;
		num++;
	}
}
void MyDatabase::insert(Record record) {
	if (record.attrv[0][0] == '0') {
		int k = 0;
		for (; record.attrv[0][k] != '\0'; k++)
			record.attrv[0][k] = record.attrv[0][k + 1];
	}
	node* anode = new node;
	node* x = head->link;
	node* xbefore = head;
	anode->link = NULL;
	anode->record = record;
	while (x != NULL&&cmp(0, &record, &(x->record), 0)) {
		xbefore = x;
		x = x->link;
	}
		xbefore->link = anode;
		anode->link = x;
		if (tail->link != NULL)
			tail = tail->link;
	num++;
}
bool MyDatabase::cmp(int which_attr, Record* a, Record* b,bool is_min) {//比优先，优先级关系ismin==1,表示按小的优先，ismin=0则按大的优先
	assert(which_attr != 1);
	int avalue =str_to_num( a->attrv[which_attr]);
	int bvalue = str_to_num(b->attrv[which_attr]);

	if (avalue > bvalue)
		return 1 ^ is_min;
	else if (avalue < bvalue)
		return 0 ^ is_min;
	//值一样的按id排序,id小的优先
	//比较id长度，短的优先
	int ida = str_to_num(a->attrv[0]);
	int idb = str_to_num(b->attrv[0]);
	return ida < idb;
}
void MyDatabase::del(char* id) {
	node* x = head->link;
	node* beforex = head;
	while (x != NULL&&cmpch(x->record.attrv[0], id) == -1) {
		beforex = x;
		x = x->link;
	}
	//找到删除的点了，就删除，否则什么都不做
	if (x!=NULL&&strcmp(x->record.attrv[0], id) == 0) {
		beforex->link = x->link;
		if (tail == x)
			tail = beforex;
		for(int i=0;i<x->record.attrc;i++)
			delete []x->record.attrv[i];
		delete x;
	}
	num--;
}
void MyDatabase::del(char* id1, char* id2) {
	node* x = head->link;
	node* beforex = head;
	while (x != NULL&&cmpch(x->record.attrv[0], id1) == -1) {
		beforex = x;
		x = x->link;
	}
	//找到删除的点了，就删除，否则什么都不做
	while(x != NULL&&cmpch(x->record.attrv[0], id2)!= 1){
		beforex->link = x->link;
		for (int i = 0; i<x->record.attrc; i++)
			delete[] x->record.attrv[i];
		delete x;
		x = beforex->link;
		num--;
	}
	if (x == NULL)
		tail = beforex;
}
int MyDatabase::cmpch(char* a, char* b) {//比大
	int na = str_to_num(a);
	int nb = str_to_num(b);
	if (na > nb)
		return 1;
	else if (na == nb)
		return 0;
	else return -1;
}
void MyDatabase::set(char* id, char* keych, char* value) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	//确认了是哪一个列了
	if (key < colnum) {
		key += 2;
		//寻找id对应的记录
		node* x = head->link;
		while (x != NULL&&cmpch(x->record.attrv[0], id) == -1)
			x = x->link;
		if (x != NULL&&strcmp(x->record.attrv[0], id) == 0) {
			strcpy(x->record.attrv[key], value);
		}
	}


}
void MyDatabase::add(char* id, char* keych, char* value) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}

	if (key < colnum) {
		key += 2;
		//寻找id对应的记录
		node* x = head->link;
		while (x != NULL&&cmpch(x->record.attrv[0], id) == -1)
			x = x->link;
		if (x != NULL&&strcmp(x->record.attrv[0], id) == 0) {
			chadd(x->record.attrv[key], value);
		}
	}

}
void MyDatabase::set(char* id1, char* id2, char* keych, char* value) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	if (key < colnum) {
		key += 2;
		node* x = head->link;
		while (x != NULL&&cmpch(x->record.attrv[0], id1) == -1)
			x = x->link;
		while (x != NULL&&cmpch(x->record.attrv[0], id2) != 1) {
			strcpy(x->record.attrv[key], value);
			x = x->link;
		}
	}
}
void MyDatabase::add(char* id1, char* id2, char* keych, char* value) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	if (key < colnum) {
		key += 2;
		node* x = head->link;
		while (x != NULL&&cmpch(x->record.attrv[0], id1) == -1)
			x = x->link;
		while (x != NULL&&cmpch(x->record.attrv[0], id2) != 1) {
			chadd(x->record.attrv[key], value);
			x = x->link;
		}
	}
}
void MyDatabase::chadd(char* dest, char* src) {
	char temp[32];
	int dlen = strlen(dest);
	int slen = strlen(src);
	int len = dlen < slen ? dlen : slen;
	int carry = 0;
	for (int i = 0; i < len; i++) {
		int t = dest[dlen-1-i] - '0' + src[slen-1-i] - '0' + carry;
		temp[i] = t % 10 + '0';
		carry = t / 10;
	}
	if (dlen > slen) {
		for (int i = len; i < dlen; i++) {
			int t = dest[dlen-1-i] - '0' + carry;
			temp[i] = t % 10 + '0';
			carry = t / 10;
		}
		len = dlen;
		if (carry) {
			temp[len] = '1';
			len++;
		}
	}
	else if (slen > dlen) {
		for (int i = len; i < slen; i++) {
			int t = src[slen-1-i] - '0' + carry;
			temp[i] = t % 10 + '0';
			carry = t / 10;
		}
		len = slen;
		if (carry) {
			temp[len] = '1';
			len++;
		}
	}
	else if (carry) {
		temp[len] = '1';
		len++;
	}
	for (int i = 0; i < len; i++) {
		dest[i] = temp[len - 1-i];
	}
	dest[len] = '\0';
}
void MyDatabase::query_id(char* id, ofstream &fout) {
	node* x = head->link;
	while (x!=NULL&&cmpch(x->record.attrv[0], id)==-1)
		x = x->link;
	if (x != NULL&&strcmp(x->record.attrv[0], id) == 0) {
		fout << x->record;
	}
}
void MyDatabase::query_name(char* name, ofstream &fout) {
	node* x = head->link;
	while (x != NULL) {
		if (strcmp(x->record.attrv[1], name) == 0) {
			fout << x->record;
		}
		x = x->link;
	}
}
void MyDatabase::query_key(char* keych, int symb, char* value, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	if (key < colnum) {
		key += 2;
		node* x = head->link;
		switch (symb) {
		case 0:
				while (x != NULL) {
					if (cmpch(x->record.attrv[key], value) == 1)
						fout << x->record;
					x = x->link;
				}break;
		case 1:
				while (x != NULL) {
				if (cmpch(x->record.attrv[key], value) !=-1)
					fout << x->record;
					x = x->link;
				}break;
		case 2:
				while (x != NULL) {
				if (cmpch(x->record.attrv[key], value)== -1)
					fout << x->record;
				x = x->link;
				}break;
		case 3:
				while (x != NULL) {
					if (cmpch(x->record.attrv[key], value) != 1)
						fout << x->record;
					x = x->link;
				}break;
		case 4:
				while (x != NULL) {
					if (cmpch(x->record.attrv[key], value)== 0)
						fout << x->record;
					x = x->link;
				}break;
		case 5:
				while (x != NULL) {
					//if (strcmp(x->record.attrv[0], "998911") == 0&& strcmp(keych, "music") == 0) {
					//	int i = 0;
						//i++;
					//}
					if (cmpch(x->record.attrv[key], value) != 0)
						fout << x->record;
					x = x->link;
				}break;
		default:break;
		}
	}
}
void MyDatabase::query_key(char* id1, char*id2, char* keych, int symb, char* value, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	if (key < colnum) {
		key += 2;
		node* x = head->link;
		while (x!=NULL&&cmpch(x->record.attrv[0], id1) == -1)
			x = x->link;
		switch (symb) {
		case 0:
			while (x!=NULL&&cmpch(x->record.attrv[0], id2) != 1) {
				if (cmpch(x->record.attrv[key], value) == 1)
					fout << x->record;
				x = x->link;
			}break;
		case 1:
			while (x != NULL&&cmpch(x->record.attrv[0], id2) != 1) {
				if (cmpch(x->record.attrv[key], value) != -1)
					fout << x->record;
				x = x->link;
			}break;
		case 2:
			while (x != NULL&&cmpch(x->record.attrv[0], id2) != 1) {
				if (cmpch(x->record.attrv[key], value) == -1)
					fout << x->record;
				x = x->link;
			}break;
		case 3:
			while (x != NULL&&cmpch(x->record.attrv[0], id2) != 1) {
				if (cmpch(x->record.attrv[key], value) != 1)
					fout << x->record;
				x = x->link;
			}break;
		case 4:
			while (x != NULL&&cmpch(x->record.attrv[0], id2) != 1) {
				if (cmpch(x->record.attrv[key], value) == 0)
					fout << x->record;
				x = x->link;
			}break;
		case 5:
			while (x != NULL&&cmpch(x->record.attrv[0], id2) != 1) {
				if (cmpch(x->record.attrv[key], value) != 0)
					fout << x->record;
				x = x->link;
			}break;
		default:break;
		}
	}
}
void MyDatabase::query_the_max(char* keych, int k, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	if (key < colnum) {
		key += 2;
		Heap h;
		h.set(key);
		node* x = head->link;
		for (int i = 0; i < k; i++) {
			h.mininsert(&x->record);
			x = x->link;
		}
		while (x != NULL) {
			if (cmp(key, &x->record, h.h[0], 0) == 1)
			{
				h.minexchange(&x->record);
			}
			x = x->link;
		}
		fout<<*h.minfetch();
		//for (int i = 0; i < k; i++) {
		//	Record** max = new Record*[h.num];
		//	for (int i = 0; i < h.num; i++)
		//		max[i] = h.minfetch();
		//}
	}
}
void MyDatabase::query_the_min(char* keych, int k, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	if (key < colnum) {
		key += 2;
		Heap h;
		h.set(key);
		node* x = head->link;
		for (int i = 0; i < k; i++) {
			h.maxinsert(&x->record);
			x = x->link;
		}
		while (x != NULL) {
			if (cmp(key, &x->record, h.h[0], 1) == 1)
			{
				h.maxexchange(&x->record);
			}
			x = x->link;
		}
		fout << *h.maxfetch();
	}
}
void MyDatabase::query_the_max(char* id1, char*id2, char* keych, int k, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	node* index1;
	//node* index2;
	for (index1 = head->link; cmpch(index1->record.attrv[0], id1) == -1; index1 = index1->link);
	//for (index2=index1;cmpch(index2->record.attrv[0], id2) != 1;index2=index2->link);
	if (key < colnum) {
		key += 2;
		Heap h;
		h.set(key);
		node* x = index1;
		for (int i = 0; i < k; i++) {
			h.mininsert(&x->record);
			x = x->link;
		}
		while (cmpch(x->record.attrv[0],id2)!=1) {
			if (cmp(key, &x->record, h.h[0], 0) == 1)
			{
				h.minexchange(&x->record);
			}
			x = x->link;
		}
		fout << *h.minfetch();
	}
}
void MyDatabase::query_the_min(char*id1, char*id2, char* keych, int k, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	node* index1;
	//node* index2;
	for (index1 = head->link; cmpch(index1->record.attrv[0], id1) == -1; index1 = index1->link);
	//for (index2 = index1; cmpch(index2->record.attrv[0], id2) != 1; index2 = index2->link);
	if (key < colnum) {
		key += 2;
		Heap h;
		h.set(key);
		node* x = index1;
		for (int i = 0; i < k; i++) {
			h.maxinsert(&x->record);
			x = x->link;
		}
		while (cmpch(x->record.attrv[0],id2)!=1) {
			if (cmp(key, &x->record, h.h[0], 1) == 1)
			{
				h.maxexchange(&x->record);
			}
			x = x->link;
		}
		fout << *h.maxfetch();
	}
}
void MyDatabase::query_max(char* keych, int k, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	if (key < colnum) {
		key += 2;
		Heap h;
		h.set(key);
		node* x = head->link;
		for (int i = 0; i < k; i++) {
			h.mininsert(&x->record);
			x = x->link;
		}
		while (x != NULL) {
			if (cmp(key, &x->record, h.h[0], 0) == 1)
			{
				h.minexchange(&x->record);
			}
			x = x->link;
		}
		Record** max = new Record*[h.num];
		int hn = h.num;
		for (int i = 0; i < hn; i++)
			max[i] = h.minfetch();
		for (int i = hn - 1; i >= 0; i--) {
			fout << *max[i];
		}
		delete[]max;
	}
}
void MyDatabase::query_min(char* keych, int k, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	if (key < colnum) {
		key += 2;
		Heap h;
		h.set(key);
		node* x = head->link;
		for (int i = 0; i < k; i++) {
			h.maxinsert(&x->record);
			x = x->link;
		}
		while (x != NULL) {
			if (cmp(key, &x->record, h.h[0], 1) == 1)
			{
				h.maxexchange(&x->record);
			}
			x = x->link;
		}
		Record** min = new Record*[h.num];
		int hn = h.num;
		for (int i = 0; i < hn; i++)
			min[i] = h.maxfetch();
		for (int i = hn - 1; i >= 0; i--) {
			fout << *min[i];
		}
		delete[]min;
	}
}
void MyDatabase::query_max(char* id1, char*id2, char* keych, int k, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	node* index1;
	//node* index2;
	for (index1 = head->link; cmpch(index1->record.attrv[0], id1) == -1; index1 = index1->link);
	//for (index2 = index1; cmpch(index2->record.attrv[0], id2) != 1; index2 = index2->link);
	if (key < colnum) {
		key += 2;
		Heap h;
		h.set(key);
		node* x = index1;
		for (int i = 0; i < k; i++) {
			h.mininsert(&x->record);
			x = x->link;
		}
		while (cmpch(x->record.attrv[0], id2) != 1) {
			if (cmp(key, &x->record, h.h[0], 0) == 1)
			{
				h.minexchange(&x->record);
			}
			x = x->link;
		}
		Record** max = new Record*[h.num];
		int hn = h.num;
		for (int i = 0; i < hn; i++)
			max[i] = h.minfetch();
		for (int i = hn - 1; i >= 0; i--) {
			fout << *max[i];
		}
		delete[]max;
	}
}
void MyDatabase::query_min(char*id1, char*id2, char* keych, int k, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	node* index1;
	//node* index2;
	for (index1 = head->link; cmpch(index1->record.attrv[0], id1) == -1; index1 = index1->link);
	//for (index2 = index1; cmpch(index2->record.attrv[0], id2) != 1; index2 = index2->link);
	if (key < colnum) {
		key += 2;
		Heap h;
		h.set(key);
		node* x = index1;
		for (int i = 0; i < k; i++) {
			h.maxinsert(&x->record);
			x = x->link;
		}
		while (cmpch(x->record.attrv[0], id2) != 1) {
			if (cmp(key, &x->record, h.h[0], 1) == 1)
			{
				h.maxexchange(&x->record);
			}
			x = x->link;
		}
		Record** min = new Record*[h.num];
		int hn = h.num;
		for (int i = 0; i < hn; i++)
			min[i] = h.maxfetch();
		for (int i = hn-1; i >= 0; i--) {
			fout << *min[i];
		}
		delete[]min;
	}
}
void MyDatabase::sum(char* id1, char* id2, char * keych, ofstream &fout) {
	int key = 0;
	for (; key < colnum; key++) {
		if (strcmp(keych, colname[key]) == 0)
			break;
	}
	node* index1;
	//node* index2;
	for (index1 = head->link; cmpch(index1->record.attrv[0], id1) == -1; index1 = index1->link);
	//for (index2 = index1; cmpch(index2->record.attrv[0], id2) != 1; index2 = index2->link);
	if (key < colnum) {
		key += 2;
		node* x = index1;
		int ans = 0;
		while (cmpch(x->record.attrv[0], id2) != 1) {
			ans += str_to_num(x->record.attrv[key]);
			x = x->link;
		}
		if (starttag == 1)
			fout << "\n";
		else starttag = 1;
		fout << ans;
	}
}
void MyDatabase::DelAll() {
	node* del;
	while (head->link != NULL) {
		del = head->link;
		head->link = head->link->link;
		for (int i = 0; i < del->record.attrc; i++)
			delete[]del->record.attrv[i];
		delete del;
	}
	delete head;
	head = NULL;
	tail = NULL;
}



