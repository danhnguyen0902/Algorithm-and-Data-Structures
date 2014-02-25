#include <stdio.h>
//#include <vector>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;

#define maxN 1000000 //1,000,000

int N, M;
//vector<int> factor;
//vector<int> power;
int factor;
int power;
int prime[maxN + 10];
int soNguyenTo[100000]; 
long long powerPrime[1010][30]; 
long long GCDSUM[maxN + 10];
long long g[maxN + 10];

void sieve(int n) { //Eratosthenes Sieve: O(nloglogn)
	int i, j;
	memset(prime, 1, sizeof(prime));
	for (i = 2; i * i <= n; ++i) 
		if (prime[i]) 
			for (j = i * i; j <= n; j += i) 
				prime[j] = 0; 
}	

void factorOut(int x) { 
	int i, p, flag = 0;

	if (prime[x]) 
		return;

	for (i = 0; i < M; ++i) {
		if (x % soNguyenTo[i] == 0) {
			factor = soNguyenTo[i];
			for (p = 0; x % soNguyenTo[i] == 0; x /= soNguyenTo[i], ++p);
			power = p;
			flag = 1;
			break; 
		}
	}
}

void prepare() {	
	int i, j;
	long long t;

	//First, find the primes
	sieve(maxN);

	M = 0;
	for (i = 2; i <= maxN; i++) 
		if (prime[i]) {
			soNguyenTo[M] = i;
			M++;
		}
	
	//Then, find the powers of the primes that <= sqrt(maxN) ( = 1000 ):
	for (i = 2; i <= 1000; i++)
		if (prime[i]) {
			powerPrime[i][0] = 1LL;
			j = 1;
			t = 1LL;
			while (1) {
				t = t * i;
				
				if (t > maxN) 
					break;

				powerPrime[i][j] = t;

				j++;
			}
		}

	g[1] = 1LL; 
	t = 2LL;
	for (i = 2; i <= maxN; i++) {

		factorOut(i);

		if (prime[i])
			g[i] = (t * i - 1LL); 
		else
			g[i] = ((power + 1) * powerPrime[factor][power] - power * powerPrime[factor][power - 1]) * g[i / powerPrime[factor][power]]; //(*)
	}
}

void find() {
	int i;

	GCDSUM[1] = 0LL;

	for (i = 2; i <= maxN; i++) 
		GCDSUM[i] = GCDSUM[i - 1] + g[i] - i;
}

int main() {
	//clock_t start, end;
	//start = clock();
	prepare();
	//end = clock();
	//printf("The time was: %lf seconds\n", (end - start) / (double)CLOCKS_PER_SEC);
	find();


	while (scanf("%d", &N) != EOF) {
		if (N == 0)
			break;

		printf("%lld\n", GCDSUM[N]);
	}

	return 0;
}
