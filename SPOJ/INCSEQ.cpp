#include <stdio.h>
#define max_n 10000
#define max_s 100000
#define modulo 5000000

void inp(int a[], int &n, int &k) {
	scanf("%d%d", &n, &k);
	for (int i = 1; i <= n; i++)
		scanf("%d", &a[i]);
}

void update(int i, int value, int x, int **t) {
	while (x > 0) {
		//t[i][x] = (t[i][x] % 1000000000 + value % 1000000000) % 1000000000;
		t[i][x] += value;
		if (t[i][x] >= modulo)
			t[i][x] -= modulo;
		x -= x & (-x);
	}
}

int get(int i, int x, int **t) {
	int sum = 0;
	while (x <= max_s) {
		//sum = (sum % 1000000000 + t[i - 1][x] % 1000000000) % 1000000000;
		sum += t[i - 1][x];
		if (sum >= modulo)
			sum -= modulo;
		x += x & (-x);
	}
	return sum;
}

void solve(int a[], int **f, int n, int k, int **t) {
	int i, j;
	
	int kq = 0;
	for (j = 1; j <= n; j++) { 
		for (i = 1; i <= k; i++) { 
			f[i][j] = get(i, a[j] + 1, t);
			if (i == 1) 
				update(i, 1, a[j], t);
			else
				update(i, f[i][j], a[j], t);
		}
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
	int i, j, n, k;
	int *a = new int [max_n + 1];
	int *b = new int [max_n + 1];
	int **f;

	inp(a, n, k);

	f = new int *[k + 1];
	for (i = 0; i <= k; i++) {
		f[i] = new int [max_n + 1];
	}

	if (k == 1) {
		printf("%d", n);
		return 0;
	}

	//Dao day lai de dua ve bai KINV:
	j = 1;
	for (i = n; i >= 1; i--) {
		b[j] = a[i];
		j++;
	}

	int **t;
	t = new int *[k + 1];
	for (i = 0; i <= k; i++) {
		t[i] = new int [max_s + 1];
		for (j = 0; j <= max_s; j++)
			t[i][j] = 0;
	}

	solve(b, f, n, k, t);

	return 0;
}
