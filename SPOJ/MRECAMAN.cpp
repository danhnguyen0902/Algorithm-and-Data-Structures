#include <stdio.h>

#define maxN 500010
#define maxV 6000000

int a[maxN];
int mark[maxV];

void precompute() {
	int i;

	a[0] = 0;

	for (i = 1; i <= 500000; i++) {
		if (a[i - 1] - i > 0 && mark[a[i - 1] - i] == 0) 
			a[i] = a[i - 1] - i;
		else
			a[i] = a[i - 1] + i;

		mark[a[i]] = 1;
	}
}

int main() {
	int k;

	precompute();

	while (scanf("%d", &k) != EOF) {
		if (k == -1)
			break;

		printf("%d\n", a[k]);
	}

	return 0;
}
