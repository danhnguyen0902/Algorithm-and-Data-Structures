#include <stdio.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

const int V = 10010;
const int inf = -0x7fffffff;

vector<int> adj[V];
vector<int> cost[V];
vector<int> vertex[V];

int* IT[V];

int idx[V]; //to save the index of the original tree vertices on the Interval Trees
int number[V]; //to save the number of elements on each IT

int parent[V];
int heavy[V], heavy_cost[V];
int light[V], light_cost[V];
int depth[V], size[V];  
int chain[V], head[V];  
//Where chain[u] is u's chain number and head[u] is the node closest to root in u's chain.

int N, number_of_chains;

FILE *fin = fopen("inputFile.txt", "r");
FILE *fout = fopen("outputFile.txt", "w");

int min(int x, int y) {
	if (x < y)
		return x;
	else
		return y;
}

int max(int x, int y) {
	if (x > y)
		return x;
	else
		return y;
}

void swap(int &x, int &y) {
	int temp = x;
	x = y;
	y = temp;
}

void quickSortRandomPivot(int k, int left, int right) {
	int i = left, j = right;

	//or: int pivot = u[left + roundDouble(((right - left) * ((double)rand() / RAND_MAX)))]; 

	int pivot = adj[k][rand() % (right - left + 1) + left]; //adj[k][(left + right) / 2]; 
	
	/* partition */
    while (i <= j) {
		while (adj[k][i] < pivot)
				i++;

		while (adj[k][j] > pivot)
				j--;

		if (i <= j) {
			swap(adj[k][i], adj[k][j]);
			swap(cost[k][i], cost[k][j]);
			
			i++;
			j--;
		}
    };

    /* recursion */
    if (left < j)
		quickSortRandomPivot(k, left, j);

    if (i < right)
		quickSortRandomPivot(k, i, right);
}

int binary_search(int i, int j) {
	int left = 0, right = adj[i].size() - 1, mid;

	while (left <= right) {
		mid = (left + right) / 2;
		if (adj[i][mid] < j)
			left = mid + 1;
		else if (adj[i][mid] > j)
			right = mid - 1;
			else
				return mid;
	}
}

void init(int test) {
	int i;
	for (i = 0; i < N; i++) {   
		adj[i].resize(0); 
		cost[i].resize(0);
		vertex[i].resize(0);

		if (IT[i] != NULL) {
			delete []IT[i];
			IT[i] = NULL;
		}
	}

	memset(idx, 0, sizeof(idx));
	memset(number, 0, sizeof(number));
	memset(parent, 0, sizeof(parent));
	memset(heavy_cost, inf, sizeof(heavy_cost)); //it's not gonna be inf becthe function fills the block of memory using the unsigned char conversion of that value.
	memset(light_cost, inf, sizeof(light_cost));
	memset(depth, 0, sizeof(depth));
	memset(chain, 0, sizeof(chain));
	memset(head, 0, sizeof(head)); 
	memset(size, 0, sizeof(size));
	memset(heavy, -1, sizeof(heavy));
	memset(light, -1, sizeof(light));
}

int q[V], *qf, *qb; // BFS queue

void heavylight_BFS() {
	int i, j, k, x, y, temp;
	qf = qb = q;
	parent[0] = -1;
	depth[0] = 0;
	*qb++ = 0;

	while (qf < qb)
		for (i = *qf++, k = 0; k < adj[i].size(); k++) {
			j = adj[i][k];
			if (j == parent[i]) 
				continue;

			parent[j] = i;
			depth[j] = depth[i] + 1;
			*qb++ = j;
		}

	for (k = N - 1; k > 0; k--) {
		j = q[k], 
		i = parent[q[k]];

		size[j]++;
		size[i] += size[j];
		
		x = binary_search(i, j);

		if (heavy[i] == -1) {
			heavy[i] = j;
			heavy_cost[j] = cost[i][x];
			continue;
		}
		
		if (size[j] > size[heavy[i]]) { //the subtree of the "heavy vertex" has the biggest size
			
			light[heavy[i]] = i;
			light_cost[heavy[i]] = heavy_cost[heavy[i]];

			heavy_cost[heavy[i]] = inf;
			heavy[i] = j; //luu vay la vi there is at most one heavy edge of all (or: out of) the edges joining any given vertex to its children.
			heavy_cost[j] = cost[i][x];
			
		}
		else {
			light[j] = i; //luu ngc lai tu duoi len cho de tai vi moi node chi co duy nhat 1 parent
			light_cost[j] = cost[i][x];
		}
	}

	number_of_chains = 0;
	for (i = 0; i < N; i++) {
 		if (parent[i] == -1 || heavy[parent[i]] != i) {
			for (k = i; k != -1; k = heavy[k]) 
				chain[k] = number_of_chains, head[k] = i; 
			number_of_chains++;
		}
	}
}

int lca(int i, int j, int &count1, int &count2) {
	while (chain[i] != chain[j]) {
		if (depth[head[i]] > depth[head[j]]) {
			count1 += idx[i] - idx[head[i]] + 1;
			i = parent[head[i]];
		}
		else {
			count2 += idx[j] - idx[head[j]] + 1;
			j = parent[head[j]];
		}
	}

	if (depth[i] < depth[j]) 
		count2 += idx[j] - idx[i];
	else
		count1 += idx[i] - idx[j];

	return depth[i] < depth[j] ? i : j;
}

void build(int node, int b, int e, int i, int heavy_path[]) {
	if (b == e) {
		IT[i][node] = heavy_path[b];
		return;
	}

	build(2 * node, b, (b + e) / 2, i, heavy_path);
	build(2 * node + 1, (b + e) / 2 + 1, e, i, heavy_path);

	IT[i][node] = IT[i][2 * node] + IT[i][2 * node + 1];
} 

int get_sum(int node, int b, int e, int i, int u, int v) { //stdind the maximum edge from node (or: vertex) u to node v
	int p1, p2;

	if (v < b || e < u)
		return 0;

	if (u <= b && e <= v) 
		return IT[i][node];

	p1 = get_sum(2 * node, b, (b + e) / 2, i, u, v);
	p2 = get_sum(2 * node + 1, (b + e) / 2 + 1, e, i, u, v);

	return p1 + p2;
}

void prepare() {
	int i, j, k, h;
	int mark[V]; memset(mark, 0, sizeof(mark));
	int heavy_path[V];

	//----------------------Sort-------------------------------------//
	for (i = 0; i < N; i++) {
		quickSortRandomPivot(i, 0, adj[i].size() - 1);
	}

	heavylight_BFS(); //--------Heavy Light Decomposition-----------//

	//------------------------------Build Interval Trees for Heavy-Path groups---------------------------------//
	for (i = 0; i < N; i++) {
		h = head[i];
		if (i == h) 
			continue;

		if (mark[h] == 0) {
			mark[h] = 1;

			k = 0;
			idx[h] = k;
			vertex[chain[i]].push_back(h);
			while (heavy[h] != -1) {
				j = heavy[h];
				mark[j] = 1;
				heavy_path[k] = heavy_cost[j];
				k++;
				idx[j] = k;
				vertex[chain[i]].push_back(j);
				h = j;
			}

			number[chain[i]] = k; //save the number of elements on each IT
			IT[chain[i]] = new int[4 * k]; //or IT[chain[h]]
			build(1, 0, k - 1, chain[i], heavy_path);
		}
	}//end for
}

int distance_btw(int LCA, int x) {
	int distance = 0;

	while (chain[x] != chain[LCA]) {
		if (head[x] != x) { 
			distance += get_sum(1, 0, number[chain[x]] - 1, chain[x], idx[head[x]], idx[x] - 1); 
			x = head[x];
		}
		else {
			distance += light_cost[x];
			x = light[x];
		}
	}

	if (x != LCA) 
		distance += get_sum(1, 0, number[chain[x]] - 1, chain[x], idx[LCA], idx[x] - 1);

	return distance;
}

int run_up(int x, int LCA, int count, int k) {
	if (count == k)
		return x;

	while (chain[x] != chain[LCA]) {
		if (x != head[x]) { 
			if (count + (idx[x] - idx[head[x]]) > k) 
				return vertex[chain[x]][idx[x] - (k - count)];
			else {
				count += idx[x] - idx[head[x]];
				x = head[x];
			}
		}
		else {
			count += 1;
			x = light[x];
		}

		if (count == k) 
			return x;
	}

	if (x != LCA) {
		if (count + (idx[x] - idx[LCA]) > k) 
			return vertex[chain[x]][idx[x] - (k - count)];
		else {
			count += idx[x] - idx[LCA];
			return LCA;
		}
	}
}

void solve() {
	char s[10];  
	int a,b, LCA, distance, k, kth_node, count1, count2;

	while(fscanf(stdin, "%s", s) != EOF && strcmp(s, "DONE")) {  
		if (s[0] == 'D') {  
			fscanf(stdin, "%d%d", &a, &b);  

			a--; //the index of the first vertex is 0
			b--; //the index of the first vertex is 0

			count1 = 1;
			count2 = 1;

			LCA = lca(a, b, count1, count2);

			distance = 0;
			distance += distance_btw(LCA, a);
			distance += distance_btw(LCA, b);
			
			fprintf(stdout, "%d\n", distance);
		}  
		else {  
			fscanf(stdin, "%d%d%d", &a, &b, &k);  

			a--; //the index of the first vertex is 0
			b--; //the index of the first vertex is 0

			count1 = 1;
			count2 = 1;
			LCA = lca(a, b, count1, count2);
			count2--;

			if (count1 < k) {
				kth_node = run_up(b, LCA, 1, count2 - (k - count1) + 1);
			}
			else
				kth_node = run_up(a, LCA, 1, k);
			
			
			fprintf(stdout, "%d\n", kth_node + 1);
		}  
	}
}

int main() {
	int i, j, t, a, b, c;

	//end, start, CLOCKS_PER_SEC are integers
	//clock_t start, end;
	//start = clock();

	fscanf(stdin, "%d", &t);
	//scanf("%d", &t);

	for (i = 1; i <= t; i++) {
		fscanf(stdin, "%d", &N);
		//scanf("%d", &N);

		init(i);

		for (j = 1; j <= N - 1; j++) {
			fscanf(stdin, "%d%d%d", &a, &b, &c);
			//scanf("%d%d%d", &a, &b, &c);

			a--; //the index of the first vertex is 0
			b--; //the index of the first vertex is 0

			adj[a].push_back(b);
			adj[b].push_back(a);

			cost[a].push_back(c);
			cost[b].push_back(c);
		}

		prepare();

		solve();

		fprintf(stdout, "\n");

		//end = clock();
		//printf("The time was: %lf seconds\n", (end - start) / (double)CLOCKS_PER_SEC);
		//start = end;
	}
}



