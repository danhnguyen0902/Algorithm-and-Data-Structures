#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

#define maxV 10000				//10^4: number of vertices
#define maxE 100000				//10^5: number of edges
#define maxEC 100000			//10^5: the potentially maximum cost on edges

#define mp make_pair
#define pb push_back

typedef pair<int, long long> ii;

vector<ii> adj[maxV + 10];

int maxC;
int d[maxV + 10];
int trace[maxV + 10];
bool freeVertex[maxV + 10];
int heap[maxV + 10];
int pos[maxV + 10];
int n, m, s, f, nHeap;

void loadGraph() {
	int i, u, v, c;

	//freopen("input.txt", "rt", stdin);
	//freopen("output.txt", "wt", stdout);

	scanf("%d%d%d%d", &n, &m, &s, &f);
	--s; --f;

	for (i = 1; i <= m; ++i) {
		scanf("%d%d%d", &u, &v, &c);
		--u; --v;
		adj[u].pb(mp(v, c));
		adj[v].pb(mp(u, c));
	}
}

void init() {
	int i;

	maxC = maxV * maxEC;		//the potentially maximum cost for the shortest path is (maxV - 1) * maxEC ----> infinity = maxV * maxEC is enough

	for (i = 0; i <= n - 1; ++i)
		d[i] = maxC;

	d[s] = 0;

	memset(freeVertex, true, sizeof(freeVertex));
	memset(pos, 0, sizeof(pos));
	nHeap = 0;
}

void update(int v) {
	int parent, child;
	child = pos[v];
	if (child == 0) {
		++nHeap; 
		child = nHeap;
	}

	parent = child / 2;

	while (parent > 0 && d[heap[parent]] > d[v]) {
		heap[child] = heap[parent];
		pos[heap[child]] = child;
		child = parent;
		parent = child / 2;
	}

	heap[child] = v;
	pos[v] = child;
}

int pop() {
	int r, c, v, top;

	top = heap[1];
	v = heap[nHeap];
	--nHeap;

	r = 1;
	while (r * 2 <= nHeap) {
		c = r * 2;
		if (c < nHeap && d[heap[c + 1]] < d[heap[c]]) 
			++c;
		if (d[v] <= d[heap[c]])
			break;
		heap[r] = heap[c];
		pos[heap[r]] = r;
		r = c;
	}

	heap[r] = v;
	pos[v] = r;

	return top;
}

void dijkstra() {
	int i, iv, u, min;
	ii v;

	update(s);

	do {
		u = pop();
		if (u == f)
			break;

		freeVertex[u] = false;

		i = adj[u].size();
		for (iv = 0; iv < i; ++iv) {
			v = adj[u][iv];

			if (freeVertex[v.first] && d[v.first] > d[u] + v.second) {
				d[v.first] = d[u] + v.second;

				trace[v.first] = u;
				update(v.first);
			}
		}
	} while (nHeap != 0);
}

void printResult() {
	if (d[f] == maxC)
		printf("There is no path from %d to %d\n", s + 1, f + 1);
	else {
		printf("Distance from %d to %d: %d\n", s + 1, f + 1, d[f]);
		while (f != s) {
			printf("%d <-", f + 1);
			f = trace[f];
		}

		printf("%d\n", s + 1);
	}
}

int main() {
	loadGraph();
	init();
	dijkstra();
	printResult();

	return 0;
}
