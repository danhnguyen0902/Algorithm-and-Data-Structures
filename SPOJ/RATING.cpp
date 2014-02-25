#define max_n 300000
#define max_point 100000
#include <stdio.h>

int a[max_n + 2];
int h[max_n + 1];
int idx[max_n + 1];
int result[max_n + 1];
int t[max_point + 1];

void swap(int &x, int &y) {
	int temp = x;
	x = y;
	y = temp;
}

void quickSort(int arr[], int h[], int idx[], int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = arr[(left + right) / 2];

    /* partition */
    while (i <= j) {
		while (arr[i] < pivot)
				i++;

		while (arr[j] > pivot)
				j--;

		if (i <= j) {
			swap(arr[i], arr[j]);
			swap(h[i], h[j]);
			swap(idx[i], idx[j]);
			i++;
			j--;
		}
    };

    /* recursion */
    if (left < j)
		quickSort(arr, h, idx, left, j);

    if (i < right)
		quickSort(arr, h, idx, i, right);
}

void update(int x, int t[]) {
	while (x <= max_point) {
		t[x] += 1;
		x += x & (-x);
	}
}

int get(int x, int t[]) {
	int sum = 0;
	while (x > 0) {
		sum += t[x];
		x -= x & (-x);
	}
	return sum;
}

int main() {
	int i, n;
	scanf("%d", &n);
	for (i = 1; i <= n; i++) {
		scanf("%d%d", &a[i], &h[i]);
		idx[i] = i;
	}
	//===================================================
	quickSort(a, h, idx, 1, n);
	int vt = 1;
	a[n + 1] = -1;
	for (i = 2; i <= n + 1; i++) {
		if (a[i] != a[i - 1]) {
			quickSort(h, a, idx, vt, i - 1);
			vt = i;
		}
	}
	//===================================================
	for (i = 0; i <= max_point; i++)
		t[i] = 0;
	a[0] = -1;
	h[0] = -1;
	for (i = 1; i <= n; i++) {
		if (a[i] == a[i - 1] && h[i] == h[i - 1])
			result[i] = result[i - 1];
		else
			result[i] = get(h[i], t);
		update(h[i], t);
	}
	quickSort(idx, result, a, 1, n); //de dai mang a vay vao cho du thoi, khong co gi ca
	//=======================================================
	for (i = 1; i <= n; i++)
		printf("%d\n", result[i]);

	return 0;
}
