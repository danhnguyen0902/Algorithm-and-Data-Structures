#include <stdio.h>

#define max 100 //2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 > 10 ^ 12 ----> the number of prime factorizations of a number N <= 10 ^ 12 is at 
				// most 11 (because I chose the smallest primes multiplying together to make it > 10 ^ 12, and there are just 11 of them). I still
				//100 for sure anyway.
long long power[max]; //power ko can long long ma de cho chac thoi
long long factor[max];
int m;

long long modulo(long long n, long long i) { //compute n % i
	long long t = n / i;

	return (n - t * i);
}

void factorize(long long n) {
	long long i, p; //p ko can long long ma de cho chac thoi

	m = 0;

	for (i = 2LL; i * i <= n; ++i) {
		if (modulo(n, i) == 0LL) { //cu de if (n % i == 0) van OK
			factor[m] = i;
			for (p = 0LL; modulo(n, i) == 0LL; n /= i, ++p); //cu de (n % i == 0) van OK
			power[m] = p;
			m++;
		}
	}
	if (n > 1LL) {
		factor[m] = n;
		power[m] = 1LL;
		m++;
	}
}	

bool check() {
	int i;
	int flag = 0;
	
	//neu n la so nguyen to
	//if (m == 1 && power[0] == 1 && modulo(factor[0], 4LL) == 1LL) 
		//return true;

	//neu n la hop so
	for (i = 0; i < m; i++) { 
		if (modulo(factor[i], 4LL) == 3LL) { //cu de if (factor[i] % 4 == 3) van OK
			if (modulo(power[i], 2LL) == 1LL) { //cu de if (power[i] % 2 == 1) van OK
				flag = 0;
				break;
			}
			else
				flag = 1;
		}
		else
			if (modulo(factor[i], 4LL) == 1LL) //cu de if (factor[i] % 4 == 1) van OK
				flag = 1;
			//else //ko con else duoc nua, so nguyen to chi co 2 dang 4k + 3 va 4k + 1 thoi
	}

	if (flag == 1)
		return true;
	return false;
}

int main() {
	int i, c;
	long long n;

	scanf("%d", &c);

	for (i = 1; i <= c; i++) {
		scanf("%lld", &n);

		if (n <= 2LL) {
			printf("Yes\n");
			continue;
		}

		factorize(n);

		if (check()) 
			printf("Yes\n");
		else
			printf("No\n");
	}

	return 0;
}

