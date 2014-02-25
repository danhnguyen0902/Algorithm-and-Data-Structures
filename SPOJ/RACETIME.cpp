#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//#include <time.h>

int n, q, t, n_1;
int *idx, *trace;
int *arr;

void input_array() {
	scanf("%d%d", &n, &q);

	n_1 = n - 1;
	t = (int)sqrt(100.0 * n);

	arr = new int[n];
	idx = new int[n];

	for (int i = 0; i < n; i++) {
		scanf("%d", &arr[i]);
		idx[i] = i;
	}

	fflush(stdin);
}

void swap(int &x, int &y) {
	int temp;
	temp = x;
	x = y;
	y = temp;
}

void quickSort(int arr[], int idx[], int left, int right) {
	int i = left, j = right;
	int pivot = arr[rand() % (right - left + 1) + left];

	/* partition */
	while (i <= j) {
		while (arr[i] > pivot)
			i++;

		while (arr[j] < pivot)
			j--;

		if (i <= j) {
			swap(arr[i], arr[j]);
			swap(idx[i], idx[j]);
			i++;
			j--;
		}
	};

	/* recursion */
	if (left < j)
		quickSort(arr, idx, left, j);

	if (i < right)
		quickSort(arr, idx, i, right);
}

void prepare() {
	int j = 0;
	for (int i = 0; i < n; i++) {
		if (i - j + 1 == t || i ==  n_1) {
			quickSort(arr, idx, j, i);
			j = i + 1;
		}
	}

	trace = new int[n];
	for (int i = 0; i < n; i++) 
		trace[idx[i]] = i;
}

int block_number(int i) {
	int j = -1;
	while (i > j && j + t <= n_1)
		j = j + t;

	if (i > j) //position i is in the last block
		return ((n_1) / t);
	else
		return (j / t);
}

void update(int i, int v) {
	arr[trace[i]] = v;
	
	int block = block_number(i);
	int j, block_t = block * t, block_1_t = (block + 1) * t - 1;
	
	for (j = trace[i]; j > block_t; j--) {
		if (arr[j] > arr[j - 1]) {
			swap(arr[j], arr[j - 1]);
			trace[idx[j]] = j - 1;
			trace[idx[j - 1]] = j;
			swap(idx[j], idx[j - 1]);
		}
		else
			break;
	}

	for (j = trace[i]; (j < block_1_t) && (j < n_1); j++) {
		if (arr[j] < arr[j + 1]) {
			swap(arr[j], arr[j + 1]);
			trace[idx[j]] = j + 1;
			trace[idx[j + 1]] = j;
			swap(idx[j], idx[j + 1]);
		}
		else
			break;
	}
}

int binary_search(int begin, int end, int k) {
	int left = begin;
	int right = end;
	int mid;

	while (right >= left) {
		mid = (left + right) / 2;
		if (arr[mid] <= k)
			right = mid - 1;
		else
			left = mid + 1;
	}

	if (right < begin)
		return begin;
	if (left > end)
		return (end + 1);
	return (right + 1);
}

int get_answer(int i, int k) {
	if (i == -1) 
		return 0;

	int block = block_number(i);
	int block_1 = block - 1;
	int begin = 0, end = t - 1;
	int j, sum = 0;

	for (j = 0; j <= block_1; j++) {
		sum = sum + end - binary_search(begin, end, k) + 1;
		begin = end + 1;
		end = begin + (t - 1);
	}

	if (end > n_1)
		end = n_1;

	for (j = begin; j <= end /*&& j <= n_1*/; j++) {
		if (arr[j] <= k && idx[j] <= i)
			sum++;
	}

	return sum;
}

void input_query() {	
	int i, j, result;
	int v, k;
	char c[2];

	for (int l = 1; l <= q; l++) {
		scanf("%s", &c);
		if (c[0] == 'M') {
			scanf("%d%d", &i, &v);
			//fflush(stdin);
			i--;
			update(i, v);
		}
		if (c[0] == 'C') {
			scanf("%d%d%d", &i, &j, &k);
			//fflush(stdin);
			i--; 
			j--;
			result = get_answer(j, k) - get_answer(i - 1, k);
			printf("%d\n", result);
		}
	}
}

int get_answer2(int i, int j, int k) {
	int sum = 0;

	for (int l = i; l <= j; l++)
		if (arr[l] <= k)
			sum++;

	return sum;
}

void input_query2() {	
	int i, j;
	int v, k;
	char c[2];
	
	for (int l = 1; l <= q; l++) {
		scanf("%s", &c);
		if (c[0] == 'M') {
			scanf("%d%d", &i, &v);
			//fflush(stdin);
			i--;
			arr[i] = v;
		}
		if (c[0] == 'C') {
			scanf("%d%d%d", &i, &j, &k);
			//fflush(stdin);
			i--; 
			j--;
			printf("%d\n", get_answer2(i, j, k));
		}
	}
}

int main() {
	//end, start, CLOCKS_PER_SEC are integers
	//clock_t start, end;
	//start = clock();

	input_array(); 
	
	if (n > 100) {
		prepare();
		input_query();
	}
	else
		input_query2();

	//end = clock();
	//printf("The time was: %d seconds\n", (end - start) / (int)CLOCKS_PER_SEC);
	
	return 0;
}

