#include <iostream>
#include <cstring>
#include <algorithm>
#include <cstdlib>
using namespace std;

#define maxN 100
#define infSuper 3000000000LL

int m, n, x, y;
long long a[maxN + 10][maxN + 10];
int trc[maxN + 10];
long long d[maxN + 10];

void input() {
	int i, j, k, v;

	freopen("input.txt", "rt", stdin);
	//freopen("output.txt", "wt", stdout);

	cin >> n >> m;
	cin >> x >> y;

	for (i = 1; i <= n; ++i)
		for (j = 1; j <= n; ++j)
			a[i][j] = infSuper;

	for (k = 1; k <= m; ++k) {
		cin >> i >> j >> v;
		a[i][j] = v;
		a[j][i] = v;
	}
}

void dijkstra() {
	int i, i0, j;
	int nhan[maxN + 10];
	long long minV;

	for (i = 1; i <= n; ++i)
		d[i] = infSuper;

	memset(nhan, 0, sizeof(nhan));
	memset(trc, 0, sizeof(trc));

	nhan[x] = 1;
	i0 = x;
	d[x] = 0;

	for (i = 1; i <= n - 1; ++i) {	//Cai hay la o vong lap i nay, vi duong di tu x toi y chi co toi da n - 1 canh nen chi can lap n - 1 lan la du
		for (j = 1; j <= n; ++j)
			if (d[i0] + a[i0][j] < d[j]) {
				d[j] = d[i0] + a[i0][j];
				trc[j] = i0;
			}

			minV = infSuper;

			for (j = 1; j <= n; ++j) {
				if (d[j] < minV && nhan[j] == 0) {
					minV = d[j];
					i0 = j;
				}
			}
	
			nhan[i0] = 1;
			if (i0 == y)
				break;
	}

	if (d[y] == infSuper) 
		cout << "KHONG CO DUONG DI TU " << x  << "TOI " << y << endl;
	else
		cout << d[y] << endl;
}

int main() {
	input();

	dijkstra();

	return 0;
}
