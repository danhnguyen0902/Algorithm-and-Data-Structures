#include <iostream>
#include <string.h>
using namespace std;

#define maxN 15 //maximum number is a + n * d = 10^9 + 10^9 * 20,000 = 2.0001 * 10^13
#define maxD 20000
#define LL long long

int a, d, n, x;
LL count1[maxN + 10][maxD + 10], count2[maxN + 10][maxD + 10];
LL count12[maxN + 10][maxD + 10], count22[maxN + 10][maxD + 10];
LL powerOfTen[maxN + 10] = {1E0, 1E1, 1E2, 1E3, 1E4, 1E5, 1E6, 1E7, 1E8, 1E9, 1E10, 1E11, 1E12, 1E13, 1E14, 1E15};

FILE *fin = fopen("inputFile.txt", "r");
FILE *fout = fopen("outputFile.txt", "w");

void prepare() {
	int i, j, k, digit;

	//ini:
	for (digit = 0; digit <= 9; ++digit) {
		++count1[1][digit % d];

		if (digit == x)
			++count2[1][digit % d];
	}

	//do dp:
	for (i = 1; i <= maxN - 1; ++i) {
		for (k = 0; k <= d - 1; ++k) {
			for (digit = 0; digit <= 9; ++digit) {
				j = ((k * (10 % d)) % d + digit % d) %  d;
				count1[i + 1][j] += count1[i][k];
				count2[i + 1][j] += count2[i][k];

				if (digit == x)
					count2[i + 1][j] += count1[i][k];
			}
		}
	}
}

void prepare2() {
	int i, j, k, digit;
	
	//ini:
	for (digit = 1; digit <= 9; ++digit) {		
		++count12[1][digit % d];

		if (digit == x)
			++count22[1][digit % d];
	}

	//do dp:
	for (i = 1; i <= maxN - 1; ++i) {
		for (k = 0; k <= d - 1; ++k) {
			for (digit = 0; digit <= 9; ++digit) {
				j = ((k * (10 % d)) % d + digit % d) %  d;
				count12[i + 1][j] += count12[i][k];
				
				count22[i + 1][j] += count22[i][k];
				
				if (digit == x) {
					count22[i + 1][j] += count12[i][k];
				}
			}
		}
	}

	if (x == 0 && a % d == 0)
		count22[1][0] = 1;
}

int digit[maxN + 10];
int m;
int count; 

void convertToArray(LL val) {
	//int tmp[maxN + 10];
	
	memset(digit, 0, sizeof(digit));
	m = 0;
	count = 0;

	while (val > 0) {
		++m;
		digit[m] = val % 10;

		if (digit[m] == x)
			++count;

		val = val / 10;
	}

	//reverse the array:
	//int i, j = 0;

	//for (i = m; i >= 1; --i) {
	//	++j;
	//	digit[j] = tmp[i];
	//}
}

LL find(LL val) {
	int i, j, k;
	LL result;
	LL tmp;

	convertToArray(val);

	result = 0;

	//first, we'll take care of the last digit because we don't have count2[0][]
	val -= val % 10;

	if (digit[1] == x)
		--count;

	for (j = 0; j <= digit[1]; ++j) {
		if ((val + j) % d == a % d) {
			if (j == x)
				++result;

			result += count;
		}
	}

	//and then we'll take care of the rest
	for (i = 2; i <= m - 1; ++i) {
		val -= val % powerOfTen[i];

		if (digit[i] == x)
			--count;

		for (j = 0; j <= digit[i] - 1; ++j) { 
			k = ((val % d) + (j % d) * (powerOfTen[i - 1] % d)) % d;
			tmp = (a % d - k + d) % d;

			result = result + count2[i - 1][tmp] + count * count1[i - 1][tmp];

			if (j == x) {
				//if (j > 0)
					result = result + count1[i - 1][tmp];// + count * count1[i - 1][tmp];
				//else
				//result += 1 + count * count1[i - 1][tmp];
			}

			//result = result + count2[i - 1][tmp] + count * count1[i - 1][tmp];
		}
	}

	//==
	if (m > 1) {
		val -= val % powerOfTen[m];

		if (digit[m] == x)
			--count;

		for (j = 0; j <= digit[m] - 1; ++j) { 
			k = ((val % d) + (j % d) * (powerOfTen[m - 1] % d)) % d;
			tmp = (a % d - k + d) % d;

			if (j == 0) {
				for (int p = 1; p <= m - 1; ++p) 
					result = result + count22[p][tmp];// + count * count12[p][tmp];
			}
			else
				result = result + count2[m - 1][tmp];// + count * count1[m - 1][tmp];

			if (j == x && j > 0) 
				result = result + count1[m - 1][tmp];// + count * count1[m - 1][tmp];

			
		}
	}

	return result;
}

void solve() {
	LL lowerBound = a - 1;
	LL upperBound = a + (LL)n * d;

	LL result = find(upperBound) - find(lowerBound);
	cout << result;
}

int main() {
	fscanf(stdin, "%d%d%d%d", &a, &d, &n, &x);
	//cin >> a >> d >> n >> x;

	prepare();
	prepare2();

	solve();

	return 0;
}
