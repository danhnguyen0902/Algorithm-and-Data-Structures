#include <stdio.h>
#include <string.h>
#define max_n 100000
#define modulo 1000000007

int *dp = new int [max_n + 1];
int *ans = new int [max_n + 1];
int *prev = new int [128];


int main() {
	int i, j, c, t, l;
	char str[max_n + 1];
	str[0] = ' '; // (**********************************)
	scanf("%d", &t);

	for (i = 1; i <= t; i++) {
		//fflush(stdin);
		//gets(str + 1);

		scanf("%s", str + 1); 

		dp[0] = 1; 
		ans[0] = 1;
		for (c = 0; c < 128; c++) 
			prev[c] = 0;
		l = strlen(str);

		for (j = 1; j < l; j++) { 
			if (prev[str[j]] == 0) 
				dp[j] = ans[j - 1];
			else {
				dp[j] = ans[j - 1] - ans[prev[str[j]] - 1];
				if (dp[j] < 0)
					dp[j] += modulo; 
			}
			ans[j] = (ans[j - 1] + dp[j]);
			if (ans[j] >= modulo)
				ans[j] -= modulo; 
			prev[str[j]] = j;
		}
		
		printf("%d\n", ans[l - 1]);
 	}

	return 0;
}
