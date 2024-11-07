#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include <string.h>

#define TRUE  1
#define FALSE 0
#define OK 1
#define Error 0
typedef int Status;
typedef int ElemType;

typedef struct  {
	char ch;
	int weight;
	int parent, lchild, rchild;
}hefumanNode,*pNode;

typedef struct infoNode{
	char ch;
	int weight;
	struct infoNode* next;
}infoNode, *pInfo;

typedef struct {
	pInfo head;
	pInfo tail;
	int count;
}infoList;

typedef pNode nodeArr;

Status initinfoList(infoList& il,int count) {
	il.count = count;
	il.head = NULL;
	il.tail = NULL;
	pInfo pi;
	char ch;
	int we;
	int end;
	for (int i = 0; i < count; i++) {
		pi = (pInfo)malloc(sizeof(infoNode));
		printf("输入字符>>");
		ch = getchar();
		printf("输入该字符的权重>>");
		while ((end = getchar()) != '\n' && end != EOF);
		scanf_s("%d", &we);

		pi->ch = ch;
		pi->weight = we;
		pi->next = NULL;
		if (il.head == NULL) {
			il.head = pi;
			il.tail = pi;
		}
		else {
			il.tail->next = pi;
			il.tail = pi;
		}
		while ((end = getchar()) != '\n' && end != EOF);
	}
	return OK;
}

Status makeNode(pNode he,char c, int we, int pa, int ld, int rd) {
	he = (pNode)malloc(sizeof(hefumanNode));
	he->ch = c;
	he->weight = we;
	he->parent = pa;
	he->lchild = ld;
	he->rchild = rd;
	return OK;
}

Status initHefumanTree(nodeArr& arr, infoList& il, int n) {
	int m = 2* n-1;
	arr = (pNode)malloc(sizeof(hefumanNode) * m);
	pInfo pi = il.head;
	pNode p = arr;
	for (int i = 0; i < n; i++) {
		p->ch = pi->ch;
		p->weight = pi->weight;
		p->parent = NULL;
		p->rchild = NULL;
		p->lchild = NULL;
		pi = pi->next;
		p += 1;
	}
	for (int j = 0; j < m - n; j++) {
		p->ch = NULL;
		p->weight = NULL;
		p->parent = NULL;
		p->rchild = NULL;
		p->lchild = NULL;
		p += 1;
	}
	for (int k = n; k < m; k++) {
		int c1=-1;
		int c2=-1;
		int temp = 0;
		for (int u = 0; u < k; u++) {
			if (arr[u].parent == NULL) {
				if (c1 == -1 ) {
					c1 = u;
				}
				temp = arr[u].weight;
				if (temp < arr[c1].weight) {
					c1 = u;
				}
			}
		}
		for (int o = 0; o < k; o++) {
			if (arr[o].parent == NULL) {
				if (o != c1) {
					if (c2 == -1) {
						c2 = o;
					}
					temp = arr[o].weight;
					if (temp < arr[c2].weight) {
						c2 = o;
					}
				}
			}
		}
		if (arr[c1].weight < arr[c2].weight) {
			int tem1 = c1;
			c1 = c2;
			c2 = tem1;
		}
		arr[k].weight = arr[c1].weight + arr[c2].weight;
		arr[k].lchild = c1;
		arr[k].rchild = c2;
		arr[c1].parent = k;
		arr[c2].parent = k;
	}
	return OK;
}

void printarr(nodeArr& arr,int n) {
	for (int i = 0; i < 2 * n - 1; i++) {
		
		if (arr[i].ch == NULL) {
			printf("b");
		}
		else if (arr[i].ch != NULL) {
			printf("%c", arr[i].ch);
		}
		else {
			printf("a");
		}
	}
	printf("\n");
}

void hefumancoding(nodeArr na,int n,char* c,int* pi) {
	char* ctemp = c;
	for(int i = 0; i < n; i++) {
		hefumanNode temp=na[i];
		int ei = i;
		for (int j = 0; j < n - 1; j++) {
			if (temp.parent != NULL) {
				if (ei == na[temp.parent].lchild) {
					*c = '1';
					*(pi + i) = *(pi + i) + 1;
				}
				else if (ei == na[temp.parent].rchild) {
					*c = '0';
					*(pi + i) = *(pi + i) + 1;
				}
				ei = temp.parent;
				temp = na[temp.parent];
			}
			c++;
		}
	}
	for (int i = 0; i < n; i++) {
		char* l = ctemp + i * (n - 1);
		char* r = ctemp + i * (n - 1) + *(pi + i) - 1;
		while (l <= r) {
			char ctemp = *l;
			*l = *r;
			*r = ctemp;
			l += 1;
			r -= 1;
		}
	}
}

void writeBaseCode(FILE* f, char* c, nodeArr arr, int n) {
	for (int i = 0; i < n; i++) {
		char c1 = arr[i].ch;
		fputc(c1, f);
		fputc('>', f);
		for (int j = 0; j < n - 1; j++) {
			fputc(*c, f);
			c++;
		}
		fputc('\n', f);
	}
}

void readBaseCode(FILE* f, char* c1, char* c2,int n) {
	char cget = fgetc(f);
	int count = 1;
	while (count<=n*(n+2)) {
		if (count % (n + 2) == 1) {
			*c1 = cget;
			c1++;
			cget = fgetc(f);
			count++;
		}
		else if (count % (n + 2) == 0) {
			cget = fgetc(f);
			count++;
		}
		else if (count % (n + 2) == 2) {
			cget = fgetc(f);
			count++;
		}
		else {
			*c2 = cget;
			c2++;
			cget = fgetc(f);
			count++;
		}
	}
}

void code(char* c1, char* c2, FILE* fr, FILE* fw, int n) {
	char r=fgetc(fr);
	char* ctemp=c2;
	while (r != EOF) {
		for (int i = 0; i < n; i++) {
			if (r == *(c1 + i)) {
				ctemp = c2 + i * (n - 1);
				for (int j = 0; j < n - 1; j++) {
					if (*ctemp != '/') {
						fputc(*ctemp, fw);
						ctemp++;
					}
				}
			}
		}
		r = fgetc(fr);
	}
}

void decode(char* c1, char* c2, FILE* fr, FILE* fw,int* i,int n) {
	int nums = 1;
	int flag = 0;
	char c = fgetc(fr);
	int* itemp = i;
	while (c != EOF) {
		for (int i = 0; i < n - 1; i++) {
			int j = 0;
			for (j = 0; j < n; j++) {
				if (*(itemp + j) != 0) {
					if (*(c2 + j * (n - 1) + i) == c) {
						continue;
					}
					else {
						*(itemp + j) = 0;
					}
				}
			}
			c = fgetc(fr);
			int count = 0;
			int index = 0;
			for (int t = 0; t < n; t++) {
				if (*(itemp + t) == 1) {
					count += 1;
					index = t;
				}
			}
			if (count == 1) {
				fputc(*(c1 + index), fw);
				for (int y = 0; y < n; y++) {
					*(itemp + y) = 1;
				}
				break;
			}
		}
	}
}

void initarr(char*& c,int n) {
	c = (char*)malloc(sizeof(char) * n*(n-1));
	char* ctemp=c;
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n-1; y++) {
			*ctemp = '/';
			ctemp++;
		}
	}
}

void initHefumanTreeAndCode(infoList& il,nodeArr& na,int n) {
	int end;
	printf("输入n个字符和对应的权重\n");
	while ((end = getchar()) != '\n' && end != EOF);

	initinfoList(il, n);

	initHefumanTree(na, il, il.count);
	
	char* c=NULL;
	int* p;
	p = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) {
		*(p + i) = 0;
	}
	initarr(c, n);
	char* ctemp=c;
	hefumancoding(na, n, c, p);

	FILE* f;
	f = fopen("密码表.txt", "w");
	writeBaseCode(f, c, na, n);
	fclose(f);
}

void coding(int n,char* c1,char* c2) {
	FILE* f;
	f = fopen("密码表.txt", "r");

	readBaseCode(f, c1, c2, n);
	fclose(f);

	FILE* forread;
	FILE* forwrite;
	forread = fopen("原文.txt", "r");
	forwrite = fopen("编码后的原文.txt", "w");
	code(c1, c2, forread, forwrite,n);
	fclose(forread);
	fclose(forwrite);
}

void decoding(int n,char* c1,char* c2) {
	FILE* forread;
	FILE* forwrite;

	forread = fopen("编码后的原文.txt", "r");
	forwrite = fopen("译码得到的文本.txt", "w");
	int* p;
	p = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) {
		*(p + i) = 1;
	}
	decode(c1, c2, forread, forwrite, p,n);

	fclose(forread);
	fclose(forwrite);
}

int main() {
	infoList litest;
	nodeArr testarr;
	int choose;
	int n;
	char* c1 = NULL;
	char* c2 = NULL;
	printf("0-》输入字符集和权重，生成赫夫曼编码,并存入密码表\n1-》对原文按照密码表进行编码，并将结果写入文件\n2-》读取编码进行译码操作，并将结果写入文件\n3-》退出\n");
	printf(">>");
	scanf("%d", &choose);

	while (choose != 3) {
		switch (choose) {
		case 0:
			printf("输入字符集字符数量》");
			scanf("%d", &n);
			c1 = (char*)malloc(sizeof(char) * n);
			c2 = (char*)malloc(sizeof(char) * n * (n - 1));
			initHefumanTreeAndCode(litest, testarr, n);
			break;
		case 1:
			coding(n,c1,c2);
			printf("codingover");
			break;
		case 2:
			decoding(n, c1, c2);
			printf("decodingover");
			break;
		default:
			break;
		}
		scanf("%d", &choose);
	

	return 0;
}
