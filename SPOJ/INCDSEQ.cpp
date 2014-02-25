#include <stdio.h>
#define max_n 10001
#define modulo 5000000

void inp(int a[], int idx[], int &n, int &k) {
	scanf("%d%d", &n, &k);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i]);
		idx[i] = i;
	}
}

void swap(int &x, int &y) {
	int temp;
	temp = x;
	x = y;
	y = temp;
}

void quickSort(int a[], int idx[], int left, int right) {
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
			swap(idx[i], idx[j]);
			i++;
			j--;
		}
	};
	/* recursion */
	if (left < j)
		quickSort(a, idx, left, j);
	if (i < right)
		quickSort(a, idx, i, right);
}

void update(int i, int value, int x, int **t) {
	while (x > 0) {
		t[i][x] += value;
		if (t[i][x] >= modulo)
			t[i][x] -= modulo;
		x -= x & (-x);
	}
}

int get(int i, int x, int **t) {
	int sum = 0;
	while (x <= max_n) {
		sum += t[i - 1][x];
		if (sum >= modulo)
			sum -= modulo;
		x += x & (-x);
	}
	return sum;
}

void solve(int a[], int **f, int n, int k, int **t) {
	int i, j, temp;
	int kq = 0;
	int *prev = new int [max_n + 1];
	for (i = 1; i <= max_n; i++)
		prev[i] = 0;

	for (j = 1; j <= n; j++) {
		for (i = 1; i <= k; i++) {
			if (j < i) {
				f[i][j] = 0;
				continue;
			}
			if (i == 1) {
				if (prev[a[j]] == 0) 
					f[i][j] = 1;
				else {
					f[i][j] = 1;
					f[i][prev[a[j]]] = 0;
					continue;
				}
			}
			else {
				if (prev[a[j]] == 0) 
					f[i][j] = get(i, a[j] + 1, t);
				else {
					f[i][j] = get(i, a[j] + 1, t);

					temp = f[i][j] - f[i][prev[a[j]]];

					if (temp < 0)
						temp += modulo;

					update(i, temp, a[j], t);

					f[i][prev[a[j]]] = 0;
					continue;
				}
			}

			update(i, f[i][j], a[j], t);
		}

		prev[a[j]] = j;
	}

	for (int j = 1; j <= n; j++) {
		//kq = (kq % 1000000000 + f[k][j] % 1000000000) % 1000000000;
		kq += f[k][j];
		if (kq >= modulo)
			kq -= modulo;
	}
	printf("%d", kq);
}

int main() {
	int i, j, n, k, x;
	int *a = new int [max_n + 1];
	int *b = new int [max_n + 1];
	int *idx = new int [max_n + 1];
	int **f;

	inp(a, idx, n, k);

	f = new int *[k + 1];
	for (i = 0; i <= k; i++) {
		f[i] = new int [max_n + 1];
	}

	j = 1;
	for (i = n; i >= 1; i--) {
		b[j] = a[i];
		j++;
	}

	quickSort(b, idx, 1, n);
	a[idx[1]] = 1;
	for (i = 2; i <= n; i++)
		if (b[i] == b[i - 1])
			a[idx[i]] = a[idx[i - 1]];
		else
			a[idx[i]] = i;

	if (k == 1) {
		int *d = new int [max_n + 1];
		for (i = 1; i <= max_n; i++)
			d[i] = 0;

		int count = 0;
		for (i = 1; i <= n; i++)
			if (d[a[i]] == 0) {
				count++;
				d[a[i]] = 1;
			}
			printf("%d", count);
			return 0;
	}

	int **t;
	t = new int *[k + 1];
	for (i = 0; i <= k; i++) { 
		t[i] = new int [max_n + 1];
		for (j = 1; j <= max_n; j++)
			t[i][j] = 0;
	}

	solve(a, f, n, k, t);

	return 0;
} 




