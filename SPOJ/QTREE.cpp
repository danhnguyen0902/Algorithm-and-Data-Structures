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

int* IT[V];

int idx[V]; //to save the index of the original tree vertices on the Interval Trees
int number[V]; //to save the number of elements on each IT

int parent[V];
int heavy[V], heavy_cost[V];
int light[V], light_cost[V];
int depth[V], size[V];  
int chain[V], head[V];  
//Where chain[u] is u's chain number and head[u] is the node closest to root in u's chain.
int link_a[V];
int link_b[V];

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
	memset(link_a, 0, sizeof(link_a));
	memset(link_b, 0, sizeof(link_b));
	memset(size, 0, sizeof(size));
	memset(heavy, -1, sizeof(heavy));
	memset(light, -1, sizeof(light));
}

void DFS(int i) {
	int x;
	size[i] = 1;
	for (int k=0; k<adj[i].size(); ++k) {
		int j = adj[i][k];
		if (j == parent[i]) continue;

		parent[j] = i;
		depth[j] = depth[i] + 1;

		DFS(j);

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
}

void heavylight_DFS() {
	parent[0] = -1;
	depth[0] = 0;
	DFS(0);

	number_of_chains = 0; 
	for (int i=0; i<N; ++i)
		if (parent[i] == -1 || heavy[parent[i]] != i) {
			for (int k = i; k != -1; k = heavy[k])
				chain[k] = number_of_chains, head[k] = i;
			number_of_chains++;
		}
}

int lca(int i, int j) {
	while (chain[i] != chain[j]) {
		if (depth[head[i]] > depth[head[j]])
			i = parent[head[i]];
		else
			j = parent[head[j]];
	}

	return depth[i] < depth[j] ? i : j;
}

void build(int node, int b, int e, int i, int heavy_path[]) {
	if (b == e) {
		IT[i][node] = heavy_path[b];
		return;
	}

	build(2 * node, b, (b + e) / 2, i, heavy_path);
	build(2 * node + 1, (b + e) / 2 + 1, e, i, heavy_path);

	IT[i][node] = max(IT[i][2 * node], IT[i][2 * node + 1]);
} 

void update(int node, int b, int e, int i, int u, int ti) { //Change the value at node (or: vertex) u to ti
	if (u < b || e < u) //get out if it's a wrong way
		return;

	if (b == e && e == u) {
		IT[i][node] = ti;
		return;
	}

	update(2 * node, b, (b + e) / 2, i, u, ti);
	update(2 * node + 1, (b + e) / 2 + 1, e, i, u, ti);

	IT[i][node] = max(IT[i][2 * node], IT[i][2 * node + 1]);
}

int get_max(int node, int b, int e, int i, int u, int v) { //stdind the maximum edge from node (or: vertex) u to node v
	int p1, p2;

	if (v < b || e < u)
		return inf;

	if (u <= b && e <= v) 
		return IT[i][node];

	p1 = get_max(2 * node, b, (b + e) / 2, i, u, v);
	p2 = get_max(2 * node + 1, (b + e) / 2 + 1, e, i, u, v);

	//IT[i][node] = max(IT[i][2 * node], IT[i][2 * node + 1]);

	return max(p1, p2);
}

void prepare() {
	int i, j, k, h;
	int mark[V]; memset(mark, 0, sizeof(mark));
	int heavy_path[V];

	//----------------------Sort-------------------------------------//
	for (i = 0; i < N; i++) {
		quickSortRandomPivot(i, 0, adj[i].size() - 1);
	}

	heavylight_DFS(); //--------Heavy Light Decomposition-----------//

	//------------------------------Build Interval Trees for Heavy-Path groups---------------------------------//
	for (i = 0; i < N; i++) {
		h = head[i];
		if (i == h && heavy[i] == -1) 
			continue;

		if (mark[h] == 0) {
			mark[h] = 1;

			k = 0;
			idx[h] = k;
			while (heavy[h] != -1) {
				j = heavy[h];
				mark[j] = 1;
				heavy_path[k] = heavy_cost[j];
				k++;
				idx[j] = k;
				h = j;
			}

			number[chain[i]] = k; //save the number of elements on each IT
			IT[chain[i]] = new int[4 * k]; //or IT[chain[h]]
			build(1, 0, k - 1, chain[i], heavy_path);
		}
	}//end for
}

int run_up(int LCA, int max_edge, int x) {
	while (chain[x] != chain[LCA]) {
		if (head[x] != x) { 
			max_edge = max(max_edge, get_max(1, 0, number[chain[x]] - 1, chain[x], idx[head[x]], idx[x] -1)); //IT[chain[x]][1]); --> WRONG
			x = head[x];
		}
		else {
			max_edge = max(max_edge, light_cost[x]);
			x = light[x];
		}
	}

	if (x != LCA) 
		max_edge = max(max_edge, get_max(1, 0, number[chain[x]] - 1, chain[x], idx[LCA], idx[x] - 1));

	return max_edge;
}

void solve() {
	char s[10];  
	int i, ti, a, b, temp, LCA, max_edge;

	while(fscanf(stdin, "%s", s) /*scanf("%s", s)*/ != EOF && strcmp(s, "DONE")) {  
		if (s[0] == 'C') {  
			fscanf(stdin, "%d%d", &i, &ti);  
			//scanf("%d%d", &i, &ti); 

			if (chain[link_a[i]] != chain[link_b[i]]) {  //if a is connected with b by a light path (diff chains means they're connected by a light path)
				if (light[link_a[i]] != link_b[i])
					swap(link_a[i], link_b[i]);

				light_cost[link_a[i]] = ti;
			}
			else { 
				temp = min(idx[link_a[i]], idx[link_b[i]]);
				update(1, 0, number[chain[link_a[i]]] - 1, chain[link_a[i]], temp, ti); 
			}
		}  
		else {  
			fscanf(stdin, "%d%d", &a, &b);  
			//scanf("%d%d", &a, &b); 

			a--; //the index of the first vertex is 0
			b--; //the index of the first vertex is 0

			max_edge = inf;
			LCA = lca(a, b);

			max_edge = run_up(LCA, max_edge, a);
			max_edge = run_up(LCA, max_edge, b);

			fprintf(stdout, "%d\n", max_edge);
			//printf("%d\n", max_edge);
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

			link_a[j] = a;
			link_b[j] = b;
		}

		prepare();

		solve();

		//end = clock();
		//printf("The time was: %lf seconds\n", (end - start) / (double)CLOCKS_PER_SEC);
		//start = end;
	}
}



