#include <stdio.h>

struct data { 
	int sum;
	int max;
	int maxLeft;
	int maxRight;
};

int max(int x, int y, int z) {
	int maxxx = -2147483648;
	if (x > maxxx) 
		maxxx = x;
	if (y > maxxx) 
		maxxx = y;
	if (z > maxxx) 
		maxxx = z;

	return maxxx;
}

void build(int node, int b, int e, data t[], int a[]) {
	if (b == e) {
		t[node].sum = a[b];
		t[node].max = a[b]; //or a[e]
		t[node].maxRight = a[b]; //or a[e]
		t[node].maxLeft = a[b]; //or a[e]
		return;
	}

    build(2 * node, b, (b + e) / 2, t, a);
    build(2 * node + 1, (b + e) / 2 + 1, e, t, a);
 
	t[node].sum = t[2* node].sum + t[2 * node + 1].sum;
	//=============================================================
	t[node].max = max(t[2 * node].max, t[2 * node + 1].max, t[2 * node].maxRight + t[2 * node + 1].maxLeft);
	//==============================================================
	t[node].maxRight = max(t[2 * node + 1].maxRight,  t[2 * node + 1].sum + t[2 * node].maxRight, -2147483648);
	//==============================================================
	t[node].maxLeft = max(t[2 * node].maxLeft, t[2 * node].sum + t[2 * node + 1].maxLeft, -2147483648);
} 

data getMax(int node, int b, int e, data t[], int i, int j) { //do query 
	data p1, p2, p;
	data flag = {-2147483648, -2147483648, -2147483648, -2147483648};

    if (j < b || e < i)
		return flag;
   
    if (i <= b && e <= j) {
		return t[node];
	}

    p1 = getMax(2 * node, b, (b + e) / 2, t, i, j);
    p2 = getMax(2 * node + 1, (b + e) / 2 + 1, e, t, i, j);

	if (p1.sum == flag.sum && p1.max == flag.max && p1.maxLeft == flag.maxLeft && p1.maxRight == flag.maxRight)
		return p2;
	if (p2.sum == flag.sum && p2.max == flag.max && p2.maxLeft == flag.maxLeft && p2.maxRight == flag.maxRight)
		return p1;

	
	p.sum = p1.sum + p2.sum;
	//=============================================================
	p.max = max(p1.max, p2.max, p1.maxRight + p2.maxLeft);
	//==============================================================
	p.maxRight = max(p2.maxRight,  p2.sum + p1.maxRight, -2147483648);
	//==============================================================
	p.maxLeft = max(p1.maxLeft, p1.sum + p2.maxLeft, -2147483648);

	return p;
}

int main() {
	int q, n, m, i, j, k;
	int a[50000];

	scanf("%d", &n);
	for (i = 0; i < n; i++) 
		scanf("%d", &a[i]);

	scanf("%d", &m);

	//data t[200000]; //Stack overflow
	data *t = new data [4 * n];

	build(1, 0, n - 1, t, a);

	data p1, p2;

	for (q = 1; q <= m; q++) {
		p1.max = 0; p1.maxLeft = 0; p1.maxRight = 0; p1.sum = 0;
		p2.max = 0; p2.maxLeft = 0; p2.maxRight = 0; p2.sum = 0;
		scanf("%d%d", &i, &j); 
		printf("%d\n", getMax(1, 0, n - 1, t, i - 1, j - 1).max);
	}

	delete []t;
	t = NULL;

	return 0;
}

