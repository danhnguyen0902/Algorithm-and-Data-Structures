//#include <fstream>
//#include <iostream>
//using namespace std;
#include <stdio.h>

struct poster {
	int l; //vi tri cua l(i) trong mang 1 chieu da sap xep
	int r; //vi tri cua r(i) trong mang 1 chieu da sap xep
};

void init(poster p[], int n) {
	for (int i = 1; i <= n; i++) {
		p[i].l = -1;
		p[i].r = -1;
	}
}

void quickSort2(int a[], int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = a[(left + right) / 2];

    /* partition */
    while (i <= j) {
		while (a[i] < pivot)
				i++;

		while (a[j] > pivot)
				j--;

		if (i <= j) {
			tmp = a[i]; a[i] = a[j]; a[j] = tmp;
			i++;
			j--;
		}
    };

    /* recursion */
    if (left < j)
		quickSort2(a, left, j);

    if (i < right)
		quickSort2(a, i, right);
}

void quickSort(int a[], int b[], int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = a[(left + right) / 2];

    /* partition */
    while (i <= j) {
		while (a[i] < pivot)
				i++;

		while (a[j] > pivot)
				j--;

		if (i <= j) {
			tmp = a[i]; a[i] = a[j]; a[j] = tmp;
			tmp = b[i]; b[i] = b[j]; b[j] = tmp;
			i++;
			j--;
		}
    };

    /* recursion */
    if (left < j)
		quickSort(a, b, left, j);

    if (i < right)
		quickSort(a, b, i, right);
}

//===============================================================DAN_POSTER===============================================================================
struct data { 
	int flag; //vua danh dau, vua mang gia tri can phai updateB luon
	int color;
};

void build(int node, int b, int e, data t[]) {
	if (b == e) {
		t[node].flag = 0;
		t[node].color = 0;
		return;
	}

    build(2 * node, b, (b + e) / 2, t);
    build(2 * node + 1, (b + e) / 2 + 1, e, t);
 
	t[node].flag = 0;
	t[node].color = 0; 
} 

void update(int node, int b, int e, data t[], int i, int j, int h) {//Tang moi phan tu tu vi tri i den vi tri j len k don vi 
//===========================================UPDATEB============================================================
	if (t[node].flag != 0) { //di ngang wa ma node nay bi danh dau can phai update thi update, khong thi thoi
		//neu node nay chua phai la leaf node thi danh dau cho 2 thang con o duoi:
		if (b < e) {
			t[2 * node].flag = t[node].flag;
			t[2 * node + 1].flag = t[node].flag; 

		}
		//updateB:
		t[node].color = t[node].flag;
		//update xong roi thi xoa:
		t[node].flag = 0;
	}
//============================================================================================================

	if (j < b || e < i ) //khong thuoc doan can tang, thoat ra
		return;

	//Neu thuoc doan can tang thi tang:
	if (i <= b && e <= j) {
		//updateA:
		t[node].color = h;
		//neu node nay chua phai la leaf node thi danh dau cho 2 thang con o duoi:
		if (b < e) {
			t[2 * node].flag = h;
			t[2 * node + 1].flag = h; 
		}
		return;
	}

	update(2 * node, b, (b + e) / 2, t, i, j, h);
	update(2 * node + 1, (b + e) / 2 + 1, e, t, i, j, h);

	if (t[2 * node].color == t[2 * node + 1].color)
		t[node].color = t[2 * node].color; // or = t[2 * node + 1].color
	else 
		t[node].color = -1;
}

void getColor(int node, int b, int e, data t[], int d[]) { //do query 
//===========================================UPDATEB============================================================
	if (t[node].flag != 0) { //di ngang wa ma node nay bi danh dau can phai update thi update, khong thi thoi
		//neu node nay chua phai la leaf node thi danh dau cho 2 thang con o duoi:
		if (b < e) {
			t[2 * node].flag = t[node].flag;
			t[2 * node + 1].flag = t[node].flag; 

		}
		//updateB:
		t[node].color = t[node].flag;
		//update xong roi thi xoa:
		t[node].flag = 0;
	}
//============================================================================================================

    if (b == e) {
		d[b] = t[node].color; //or d[e]
		return;
	}

    getColor(2 * node, b, (b + e) / 2, t, d);
    getColor(2 * node + 1, (b + e) / 2 + 1, e, t, d);
 
	if (t[2 * node].color == t[2 * node + 1].color)
		t[node].color = t[2 * node].color; // or = t[2 * node + 1].color
	else 
		t[node].color = -1; 
}
//======================================================================================================================================================

int main() {
	int i, j, k, h, x, y, n, test, count, lc;
	//fstream fin;
	//fin.open("TEST.txt", ios::in);
	//fin >> test;
	scanf("%d", &test);
	poster *p; 
	int *a;
	int *b;
	int *d;
	data *t;

	for (k = 1; k <= test; k++) {
		//fin >> n;
		scanf("%d", &n);
		p = new poster [n + 1]; //p[i] voi i  tu 1 toi n
		init(p, n);
		a = new int [2 * n];
		b = new int [2 * n];
		d = new int [4 * n]; //3 * n khong du, neu de 3 * n se bi loi "Run time error" (SIGSEGV)
		x = 0;
		//(1):
		for (h = 1; h <= n; h++) {
			//fin >> i >> j;
			scanf("%d%d", &i, &j);
			a[x] = i;
			a[x + 1] = j;
			b[x] = h;
			b[x + 1] = h;

			x += 2; 
		}
		// Sau vong lap  tren, x = 2 * n
		//(2);
		quickSort(a, b, 0, x - 1); // Sort mang a co x phan tu tu a[0] toi a[x - 1]
		//(3):
		d[0] = a[0];
		p[b[0]].l = 0;
		y = 1;
		lc = 0;
		for (h = 1; h < x; h++) {
			if (a[h] == a[h - 1]) {
				if (lc == 0)
					lc = y - 1;
				if (p[b[h]].l == -1)
					p[b[h]].l = lc;
				else
					p[b[h]].r = lc;
				continue;
			}

			lc = 0;
			if (a[h] - a[h - 1] > 1) {
				y++; 
				d[y] = a[h]; //that ra mang d nay cung chang dung lam gi ca
				y++;
			}
			else { // a[h] - a[h - 1] == 1
				d[y] = a[h]; //that ra mang d nay cung chang dung lam gi ca
				y++;
			}

			if (p[b[h]].l == -1)
				p[b[h]].l = y - 1;
			else
				p[b[h]].r = y - 1;
		}

//======================================SOLVE=========================================
		t = new data [4 * y]; // == 4 * (4 * n)

		build(1, 0, y - 1, t);

		for (h = 1; h <= n; h++) 
			update(1, 0, y - 1, t, p[h].l, p[h].r, h);
		
		getColor(1, 0, y - 1, t, d);

		quickSort2(d, 0, y - 1); 

		if (d[0] == 0) //neu nhu o nay khong duoc dan poster len thi khong dem
			count = 0;
		else 
			count = 1;
		for (h = 1; h < y; h++) {
			if (d[h] == d[h - 1] || d[h] == 0)
				continue;
			else
				count++;
		}

		printf("%d\n", count);
				
//====================================================================================

		delete []p; p = NULL;
		delete []a; a = NULL;
		delete []b; b = NULL;
		delete []d; d = NULL;
		delete []t; t = NULL;
	}

	return 0;
}

