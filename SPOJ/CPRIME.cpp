#include <stdio.h>
#include <bitset>
#include <math.h>
using namespace std;

#define maxN 6000000 //6 x 10^6, the number of primes <= 10^8 is about 10^8 / ln(10^8) = 6 x 10^6
#define maxX 100000000 //100,000,000  = 10 ^ 8
#define M 100000000 //13,000,000 (about 10^8 / 8)

bitset<M + 1> prime;
int primeNumber[maxN];
int X, K;
 
void sieve(int N) { //Atkin Sieve: O(n/loglogN)
	int x, y, d, n, nn, k;

	prime.reset();
	prime.set(2);
	prime.set(3);

 	d = (int)sqrt((double)N); 
    
	for (x = 1; x <= d; ++x) 
        for (y = 1; y <= d; ++y) {
            n = 4 * x * x + y * y;
            
			if (n <= N && (n % 12 == 1 || n % 12 == 5)) 
				prime.flip(n); 
            
			n = 3 * x * x + y * y;
            
			if (n <= N && n % 12 == 7) 
				prime.flip(n); 
            
			n = 3 * x * x - y * y;
            
			if (x > y && n <= N && n % 12 == 11) 
				prime.flip(n);
        }

    for (n = 5; n <= d; ++n) 
        if (prime.test(n)) 
			for (nn = n * n, k = nn; k <= N; k += nn) 
				prime.reset(k);
}

void prepare() {
	int i;

	K = 0;

	for (i = 2; i <= maxX; ++i) 
		if (prime.test(i)) {
			primeNumber[K] = i;
			K++;
		}
}

int binSearch(int X) {
	int left = 0, right = K - 1, mid;

	while (left <= right) {
		mid = (left + right) / 2;
		if (primeNumber[mid] < X)
			left = mid + 1;
		if (primeNumber[mid] > X)
			right = mid - 1;
		if (primeNumber[mid] == X)
			return mid + 1;
	}

	return (left - 1) + 1;
}

double abs(double x) {
	if (x < 0.0)
		x = - x;
	return x;
}

int main() {
	sieve(maxX);
	prepare();

	int function;
	double result;

	while (scanf("%d", &X) != EOF) {
		if (X == 0)
			break;

		function = binSearch(X); 
		result = abs(function - X / log((double)X)) / function * 100.0;
		printf("%.1lf\n", result);
	}

	return 0;
}
