#include <iostream>  
#include <algorithm>  
using namespace std;  

long long num[1000000];  
int cnt = 0;  
long long Maxn = 1000000000000LL;  
long long a, b, sum;  
char str[20];  

void dfs1(int n) {  
	if (n == 12) 
		return;

	str[n] = '4';  
	str[n + 1] = 0; //cnt = x
	sscanf(str, "%lld", &num[cnt++]); //num[cnt] = str, cnt = x + 1
	
	dfs1(n + 1);  
	
	str[n] = '7';  
	str[n + 1] = 0;  
	sscanf(str, "%lld", &num[cnt++]);  
	
	dfs1(n + 1);  
}  

void dfs2(long long t, int pos) {  
	for (int i = pos; i < sum; i++) {  
		long long temp = t * num[i];

		if (temp > 0 && temp <= Maxn) {  
			num[cnt++] = temp;  
			dfs2(temp, i);  
		}  
		else  
			break;  
	}  
}  

void init() {  
	dfs1(0);  
	sum = cnt; 
	sort(num, num + cnt);  

	dfs2(1, 0);  
	sort(num, num + cnt);  
	cnt = unique(num, num + cnt) - num;          
	
	num[cnt] = 10000000000010LL;  
}

int Bsearch(long long t, int index) {  
	int low = 0, high = cnt, mid, i;  

	while (low + 1 < high) {  
		mid = (low + high) >> 1;  
		if (num[mid] <= t)  
			low = mid;  
		else  
			high = mid;  
	}  

	if (index == 1) {      //B  
		for (int i = low; ;i++)  
			if (num[i] > t)  
				return i;  
	}  
	else {                 //A    
		for(i = low; ;i++)  
			if (num[i] >= t)  
				return i;  
	}  
}  

int solve() {  
	int ans = Bsearch(b, 1) - Bsearch(a, 0);  

	if (ans < 0)  
		ans = 0;

	return ans;  
}  

int main() {  
	init();  
	
	int t;  
	scanf("%d", &t);  

	while (t--) {  
		scanf("%lld%lld", &a, &b);  
		printf("%d\n", solve());  
	}  
}  

