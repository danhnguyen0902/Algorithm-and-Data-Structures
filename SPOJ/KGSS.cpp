#include <iostream>
#include <stdio.h>
#include <fstream>
using namespace std;

struct data { 
	int max1;
	int max2;
};

int lonnhat(int x, int y, int z, int t) {
	int max = -2147483648;
	if (x > max) max = x;
	if (y > max) max = y;
	if (z > max) max = z;
	if (t > max) max = t;
	
	return max;
}

int lonnhi(int x, int y, int z, int t) {
	int max = lonnhat(x, y, z, t);
	if (x == max && y == max) return max;
	if (x == max && z == max) return max;
	if (x == max && t == max) return max;
	if (y == max && z == max) return max;
	if (y == max && t == max) return max;
	if (z == max && t == max) return max;

	int min = 2147483647;
	if (max - x != 0 && max - x < min) min = max - x;
	if (max - y != 0 && max - y < min) min = max - y;
	if (max - z != 0 && max - z < min) min = max - z;
	if (max - t != 0 && max - t < min) min = max - t;

	return (max - min);
}

void build(int node, int b, int e, data t[], int a[]) {
	if (b == e) {
		t[node].max1 = a[b];
		t[node].max2 = 0;
		return;
	}

    build(2 * node, b, (b + e) / 2, t, a);
    build(2 * node + 1, (b + e) / 2 + 1, e, t, a);
 
	t[node].max1 = lonnhat(t[2 * node].max1, t[2 * node + 1].max1, t[2 * node].max2, t[2 * node + 1].max2);
	t[node].max2 = lonnhi(t[2 * node].max1, t[2 * node + 1].max1, t[2 * node].max2, t[2 * node + 1].max2);
} 

void update(int node, int b, int e, data t[], int i, int x) {
	if (i < b || e < i)
		return;

	if (i == b && i == e) {
		t[node].max1 = x;
		return;
	}

	update(2 * node, b, (b + e) / 2, t, i, x);
	update(2 * node + 1, (b + e) / 2 + 1, e, t, i, x);

	t[node].max1 = lonnhat(t[2 * node].max1, t[2 * node + 1].max1, t[2 * node].max2, t[2 * node + 1].max2);
	t[node].max2 = lonnhi(t[2 * node].max1, t[2 * node + 1].max1, t[2 * node].max2, t[2 * node + 1].max2);
}

data getMax12(int node, int b, int e, data t[], int x, int y) { //do query 
    data p1, p2, p, flag;
	flag.max1 = -2147483648;
	flag.max2 = -2147483648;

    if (y < b || e < x)
		return flag;
   
    if (x <= b && e <= y) 
		return t[node];

    p1 = getMax12(2 * node, b, (b + e) / 2, t, x, y);
    p2 = getMax12(2 * node + 1, (b + e) / 2 + 1, e, t, x, y);

	if (p1.max1 == -2147483648 && p1.max2 == -2147483648)
		return p2;
	if (p2.max1 == -2147483648 && p2.max2 == -2147483648)
		return p1;

	p.max1 = lonnhat(p1.max1, p2.max1, p1.max2, p2.max2);
	p.max2 = lonnhi(p1.max1, p2.max1, p1.max2, p2.max2);
	return p;
}

int main() {
	int q, n, m, i, j, k, x,y;
	char c;
	data result;
	//fstream fin;
	//fin.open("", ios::in);
	//fstream fout;
	//fout.open("", ios::out);
	int *a;
	cin >> n;
	//scanf("%d", &n);
	//fin >> n;
	a = new int [n];
	for (i = 0; i < n; i++)
		cin >> a[i];
		//scanf("%d", &a[i]);
		//fin >> a[i];

	//data t[200000]; //Stack overflow
	data *t = new data [4 * n];

	build(1, 0, n - 1, t, a);
	
	cin >> m;
	//scanf("%d", &m);
	//fflush(stdin);
	//fin >> m;
	for (q = 1; q <= m; q++) {
		cin >> c;
		//scanf("%c", &c);
		//fin >> c;
		if (c == 'U') {
			cin >> i >> x;
			//scanf("%d%d", &i, &x); 
			//fin >> i >> x;
			update(1, 0, n - 1, t, i - 1, x);
		}
		if (c == 'Q') {
			cin >> x >> y;
			//scanf("%d%d", &x, &y); 
			//fin >> x >> y;
			result = getMax12(1, 0, n - 1, t, x - 1, y - 1);
			cout << result.max1 + result.max2 << endl;
			//printf("%d\n", result.max1 + result.max2); 
			//fout << result.max1 + result.max2 << endl;
		}
		//fflush(stdin);
	}

	delete []t;
	t = NULL;

	return 0;
}

