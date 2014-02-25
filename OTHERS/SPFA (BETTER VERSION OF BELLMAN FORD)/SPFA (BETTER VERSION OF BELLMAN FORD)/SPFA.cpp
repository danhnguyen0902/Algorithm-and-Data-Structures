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
int queue[maxV + 10];
int trace[maxV + 10];
bool inQueue[maxV + 10];
int n, m, s, f, nHeap;

void loadGraph() {
	int i, u, v, c;

	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

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
	memset(inQueue, false, sizeof(inQueue)); 
	inQueue[s] = true;

	maxC = maxV * maxEC;		//the potentially maximum cost for the shortest path is (maxV - 1) * maxEC ----> infinity = maxV * maxEC is enough

	for (int i = 0; i <= n - 1; ++i)
		d[i] = maxC;
	d[s] = 0;
}

void SPFA() {
	int x, y, u, i, iv;
	ii v;

	x = 0; y = 0; 
	queue[0] = s; 

	while (x != (y + 1) % n) {
		u = queue[x];
		inQueue[u] = false; 
		x = (x + 1) % n;

		i = adj[u].size();
		
		for (iv = 0; iv < i; ++iv) {
			v = adj[u][iv];
			
			if (d[v.first] > d[u] + v.second) {
				d[v.first] = d[u] + v.second;
				
				trace[v.first]=u;

				if (!inQueue[v.first]) {
					y = (y + 1) % n;
					queue[y] = v.first;
					inQueue[v.first] = true;
				}
			}
		}
	}
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
	SPFA();
	printResult();
	return 0;
}
