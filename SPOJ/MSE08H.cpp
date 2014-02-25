#include <stdio.h>

#define max 100
#define module 1000000007

int power[max];
int factor[max];
int m;

void factorize(int n) {
	int i, p;

	m = 0;

	for (i = 2; i * i <= n; ++i) {
		if (n % i == 0) {
			factor[m] = i;
			for (p = 0LL; n % i == 0; n /= i, ++p);
			power[m] = p;
			m++;
		}
	}
	if (n > 1) {
		factor[m] = n;
		power[m] = 1;
		m++;
	}
}

long long calc(int a, int x, int m) { //tinh (a ^ x) % m
	long long d;

	if (x == 0)
		return (1 % m);

	if (x % 2 == 1)
		return ((a * calc(a, x - 1, m)) % m);

	d = calc(a, x / 2, m);

	return ((d * d) % m);
}

int main() {
	int i, j, n, x;
	long long result, eulerFunc, phiModule;

	factorize(module);
	phiModule = module;
	for (j = 0; j < m; j++) {
		phiModule /= factor[j];
		phiModule *= (factor[j] - 1);	
	}

	while (scanf("%d", &n) != EOF) {
		result = 1;

		for (i = 1; i <= n; i++) {
			scanf("%d", &x);

			factorize(x);

			eulerFunc = x;

			for (j = 0; j < m; j++) {
				eulerFunc = ((eulerFunc % module) * calc(factor[j], phiModule - 1, module)) % module;
				eulerFunc = ((eulerFunc % module) * ((factor[j] - 1) % module)) % module;	
			}

			result = ((result % module) * (eulerFunc % module)) % module;
		}

		printf("%d\n", result);
	}

	return 0;
}

