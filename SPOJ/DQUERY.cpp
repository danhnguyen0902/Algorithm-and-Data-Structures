#include <stdio.h>
#include <stdlib.h>
//=========================================================
void swap(int &x, int &y) {
	int temp;
	temp = x;
	x = y;
	y = temp;
}

void quickSort(int a[], int b[], int left, int right) {
    int i = left, j = right;
    int pivot = a[(left + right) / 2];

    /* partition */
    while (i <= j) {
		while (a[i] < pivot)
				i++;

		while (a[j] > pivot)
				j--;

		if (i <= j) {
			swap(a[i], a[j]);
			swap(b[i], b[j]);
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

void quickSort2(int a[], int b[], int c[], int left, int right) {
    int i = left, j = right;
	int pivot = a[(left + right) / 2];

    /* partition */
    while (i <= j) {
		while (a[i] > pivot)
				i++;

		while (a[j] < pivot)
				j--;

		if (i <= j) {
			swap(a[i], a[j]);
			swap(b[i], b[j]);
			swap(c[i], c[j]);
			i++;
			j--;
		}
    };

    /* recursion */
    if (left < j)
		quickSort2(a, b, c, left, j);

    if (i < right)
		quickSort2(a, b, c, i, right);
}

void quickSort3(int a[], int b[], int left, int right) {
    int i = left, j = right;
	int pivot = a[(left + right) / 2];

    /* partition */
    while (i <= j) {
		while (a[i] < pivot)
				i++;

		while (a[j] > pivot)
				j--;

		if (i <= j) {
			swap(a[i], a[j]);
			swap(b[i], b[j]);
			i++;
			j--;
		}
    };

    /* recursion */
    if (left < j)
		quickSort3(a, b, left, j);

    if (i < right)
		quickSort3(a, b, i, right);
}
//==========================================================
void build(int t[], int n) {
	for (int i = 1; i <= n; i++) {
		t[i] = 0;
	}
} 

void add(int t[], int n, int idx) { 
	while (idx <= n){
		t[idx] += 1;
		idx += (idx & -idx);
	}
}

void getOut(int t[], int n, int idx) { 
	while (idx <= n){
		t[idx] -= 1;
		idx += (idx & -idx);
	}
}

int getResult(int t[], int idx) { //do query 
	int sum = 0;
	while (idx > 0){
		sum += t[idx];
		idx -= (idx & -idx);
	}
	return sum;
}
//============================================================
int main() {
	int n, m, i, j, k, x, y, vt;
	//==================================================================
	scanf("%d", &n);
	int *a_value = new int [n + 2];
	int *a_before = new int [n + 2];
	int *a_after = new int [n + 2];
	int *temp = new int [n + 2];
	int *temp_idx = new int [n + 2];
	int *lc = new int [n + 2];

	a_value[0] = 0;
	a_value[n + 1] = 0;
	for (x = 1; x <= n; x++) {
		scanf("%d", &a_value[x]);
		temp[x] = a_value[x];
		temp_idx[x] = x;
	}

	quickSort(temp, temp_idx, 1, n); 

	vt = 1;
	for (i = 2; i <= n; i++) {
		if (temp[i] != temp[i - 1]) {
			quickSort(temp_idx, temp, vt, i - 1); 
			vt = i;
		}
	}
	for (i = 1; i <= n; i++) {
		if (temp[i] == temp[i - 1]) 
			a_before[temp_idx[i]] = temp_idx[i - 1];
		else 
			a_before[temp_idx[i]] = 0;

		if (temp[i] == temp[i + 1]) 
			a_after[temp_idx[i]] = temp_idx[i + 1];
		else
			a_after[temp_idx[i]] = n + 1;
	}
	//===================================================================
	int *t = new int [n + 1];
	build(t, n);
	//===================================================================
	scanf("%d", &m);
	int *q_l = new int [m];
	int *q_r = new int [m];
	int *q_idx = new int [m];
	int *q_result = new int [m];
	for (x = 0; x < m; x++) {
		scanf("%d%d", &q_l[x], &q_r[x]);
		q_idx[x] = x;
	}
	quickSort2(q_l, q_r, q_idx, 0, m - 1); 

	//============================================QUERY======================================================
	for (i = 0; i <= n + 1; i++)
		lc[i] = 0;
	y = n;
	for (x = 0; x < m; x++) {
		while (y >= q_l[x] && y >= 1) {
			if (a_before[y] < q_l[x]) {
				add(t, n, y); //add new element into the tree and update it
				lc[y] = 1;
			}
			if (a_after[y] != n + 1 && lc[a_after[y]] == 1) {
				getOut(t, n, a_after[y]); //remove the element out of the tree and update it
				lc[a_after[y]] = 0;
				lc[y] = 1;
			}
			
			y--;
		}
		q_result[x] = getResult(t, q_r[x]) - getResult(t, q_l[x] - 1);
	}

	quickSort3(q_idx, q_result, 0, m - 1); 
	for (x = 0; x < m; x++)
		printf("%d\n", q_result[x]);

	delete []a_value; a_value = NULL;
	delete []a_before; a_before = NULL;
	delete []a_after; a_after = NULL;
	delete []temp; temp = NULL;
	delete []temp_idx; temp_idx = NULL;
	delete []lc; lc = NULL;
	delete []q_l; q_l = NULL;
	delete []q_r; q_r = NULL;
	delete []q_idx; q_idx = NULL;
	delete []q_result; q_result = NULL;
	delete []t; t = NULL;

	return 0;
}





