#include <stdio.h>

struct data { 
	int flag; 
	int kq; do luu tru
};

void build(int node, int b, int e, data t[]) {
	if (b == e) {
		t[node].flag = 0;
		t[node].kq = 0;
		return;
	}

    build(2 * node, b, (b + e) / 2, t);
    build(2 * node + 1, (b + e) / 2 + 1, e, t);
 
	t[node].flag = 0;
	t[node].kq = 0;
} 

void update(int node, int b, int e, data t[], int i, int j) {
//===========================================UPDATEB============================================================
	if (t[node].flag != 0) { 
		if (b < e) {
			if (t[2 * node].flag == 0)
				t[2 * node].flag = 1;
			else
				t[2 * node].flag = 0;

			if (t[2 * node + 1].flag == 0)
				t[2 * node + 1].flag = 1;
			else
				t[2 * node + 1].flag = 0;
		}
		//updateB:
		t[node].kq = (e - b + 1) - t[node].kq;
		t[node].flag = 0;
	}
//============================================================================================================

	if (j < b || e < i ) 
		return;

	if (i <= b && e <= j) {
		//updateA:
		t[node].kq = (e - b + 1) - t[node].kq;
		if (b < e) {
			if (t[2 * node].flag == 0)
				t[2 * node].flag = 1;
			else
				t[2 * node].flag = 0;

			if (t[2 * node + 1].flag == 0)
				t[2 * node + 1].flag = 1;
			else
				t[2 * node + 1].flag = 0; 
		}
		return;
	}

	update(2 * node, b, (b + e) / 2, t, i, j);
	update(2 * node + 1, (b + e) / 2 + 1, e, t, i, j);

	t[node].kq = t[2 * node].kq + t[2 * node + 1].kq;
}

int get(int node, int b, int e, data t[], int i, int j) { //do query 
	int p1, p2;
//===========================================UPDATEB============================================================
	if (t[node].flag != 0) {
		if (b < e) {
			if (t[2 * node].flag == 0)
				t[2 * node].flag = 1;
			else
				t[2 * node].flag = 0;

			if (t[2 * node + 1].flag == 0)
				t[2 * node + 1].flag = 1;
			else
				t[2 * node + 1].flag = 0;
		}
		//updateB:
		t[node].kq = (e - b + 1) - t[node].kq;
		t[node].flag = 0;
	}
//=============================================================================================================

    if (j < b || e < i)
		return 0;
   
    if (i <= b && e <= j) 
		return t[node].kq;

    p1 = get(2 * node, b, (b + e) / 2, t, i, j);
    p2 = get(2 * node + 1, (b + e) / 2 + 1, e, t, i, j);

	t[node].kq = t[2 * node].kq + t[2 * node + 1].kq;

	return p1 + p2; 
}

int main() {
	int q, n, m, i, j, k;

	scanf("%d%d", &n, &m);

	//data t[200000]; //Stack overflow
	data *t = new data [4 * n];

	build(1, 0, n - 1, t);
	
	for (q = 1; q <= m; q++) {
		scanf("%d", &k);
		if (k == 0) {
			scanf("%d%d", &i, &j); 
			update(1, 0, n - 1, t, i - 1, j - 1); 
		}
		if (k == 1) {
			scanf("%d%d", &i, &j); 
			printf("%d\n", get(1, 0, n - 1, t, i - 1, j - 1));
		}
	}

	delete []t;
	t = NULL;

	return 0;
}

