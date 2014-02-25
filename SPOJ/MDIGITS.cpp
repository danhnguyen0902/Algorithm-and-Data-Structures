#include <iostream>
using namespace std;

int xDigit[11];

long long f[11];
long long sum[11];
long long d[10][1];
long long powerOfTen[11];
long long result[10][1];

long long scs[11] = {0LL, 9LL, 180LL, 2700LL, 36000LL, 450000LL, 5400000LL, 63000000LL, 720000000LL, 8100000000LL, 10LL};
int num[11] = {0, 9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999, 1000000000};

void init() {
	int i;

	f[1] = 1LL;
	sum[0] = 0LL;
	sum[1] = f[1];
	long long j = 10LL;
	long long t = 0LL;
	for (i = 2; i <= 10; i++) {
		t += f[i - 1];
		f[i] = j + 9LL * t;
		j = j * 10LL;
		
		sum[i] = sum[i - 1] + f[i];
	}
	//======================
	j = 1LL;
	for (i = 0; i <= 10; i++) {
		powerOfTen[i] = j;
		j = j * 10LL;
	}
	//=====================
}

void restart() {
	for (int i = 0; i <= 9; i++)
		d[i][0] = 0LL;

	for (int i = 0; i <= 10; i++) 
		xDigit[i] = 0;
}

int soChuSo(int x) {
	int temp[11];
	int i, j, count = 0;
	while (x > 0) {
		temp[count + 1] = x % 10;
		x = x / 10;
		count++;
	}
	//Dao lai:
	i = count;
	for (j = 1; j <= count; j++) {
		xDigit[j] = temp[i];
		i--;
	}

	return count;
}

void count(int x) {
	int i, j;
	int t = soChuSo(x);
	long long temp = powerOfTen[t - 1];
	int k = t - 1;

	for (i = 1; i <= t; i++) {
		d[xDigit[i]][0] += x % temp + 1;
		
		for (j = 1; j <= xDigit[i] - 1; j++) 
			d[j][0] += temp;

		for (j = 1; j <= 9; j++)
			d[j][0] += xDigit[i] * sum[k];

		k--;
		temp = temp / 10LL;
	}
}

void swap(int &x, int &y) {
	int temp = x;
	x = y;
	y = temp;
}

long long count0(int x) {
	int i;
	long long t = 0LL;
	for (i = 1; i <= 10; i++) {
		if (num[i] > x) 
			break;
		t += scs[i];
	}

	i--;
	t += (x - (num[i] + 1) + 1) * (i + 1);

	return t;
}

int main() {
	int a, b, i;
	long long temp;

	init();

	while (cin >> a >> b) {
		if (a == 0 && b == 0)
			break;
		
		if (a > b) 
			swap(a, b);

		restart();	
		count(b);
		for (i = 1; i <= 9; i++)
			result[i][0] = d[i][0];

		restart();	
		count(a - 1);
		for (i = 1; i <= 9; i++)
			result[i][0] = result[i][0] - d[i][0];
		//------------------------------------------------------------------------------------------------------------------------
		
		temp = 0LL;
		for (i = 1; i <= 9; i++)
			temp += result[i][0];

		result[0][0] = count0(b) - count0(a - 1) - temp;
		//------------------------------------------------------------------------------------------------------------------------
		
		//cout << result[0][0];
		//for (i = 1; i <= 9; i++)
			//cout << " " << result[i][0];
		
		for (i = 0; i <= 9; i++)
			cout << result[i][0] << " "; 

		cout << endl;
	}

	return 0;
}
