#include <iostream>
using namespace std;

#define LL long long

struct TNode {
	int value;
	int len;
	bool rev;
	LL sum;

	TNode *P, *L, *R;
};

TNode sentinel;
TNode *root, *nilT;

void swap(TNode *&x, TNode *&y) {
	TNode *tmp = x;
	x = y;
	y = tmp;
}

void init() {
	nilT = &sentinel;
	/*nilT->len = 0;
	nilT->sum = 0;
	nilT->value = 0;
	nilT->rev = false;*/

	root = nilT;
}

void pushDown(TNode *&T) {
	if (T->rev) {
		swap(T->L, T->R);
		T->L->rev = !T->L->rev;
		T->R->rev= !T->R->rev;
		T->rev = false;
	}
} 

void update(TNode *x) {
	x->len = x->L->len + x->R->len + 1;
	x->sum = x->L->sum + x->R->sum + x->value;
}

void setL(TNode *x, TNode *y) {
	y->P = x;
	x->L = y;
}

void setR(TNode *x, TNode *y) {
	y->P = x;
	x->R = y;
}

TNode *nodeAt(TNode *x, int i) {
	int ord;

	do {
		pushDown(x);	//(***)

		ord = x->L->len + 1;

		if (ord == i)
			break;

		if (i < ord)
			x = x->L;
		else {
			i = i - ord;
			x = x->R;
		}
	} while(true);

	return x;
}

void upTree(TNode *&x) {
	TNode *y, *z;

	y = x->P;
	z = y->P;

	if (x == y->L) {
		setL(y, x->R);
		setR(x, y);
	}
	else {
		setR(y, x->L);
		setL(x, y);
	}

	if (z->L == y)
		setL(z, x);
	else
		setR(z, x);

	update(y);
	update(x);
}

void splay(TNode *&x) {
	TNode *y, *z;

	do {
		y = x->P;
		if (y == nilT)
			break;

		z = y->P;
		if (z != nilT) {
			if ((y == z->L) == (x == y->L))	//equivalent to: if ((y == z->L && x == y->L) || (y == z->R && x == y->R))
				upTree(y);
			else 
				upTree(x);
		}

		upTree(x);
	} while (true);
}

void split(TNode *T, int i, TNode *&T1, TNode *&T2) {	//split T into T1 and T2. T1 has the first i nodes in T, and T2 has the rest of them.
	if (i == 0) {
		T1 = nilT;
		T2 = T;
	}
	else {
		T1 = nodeAt(T, i);
		splay(T1);
		T2 = T1->R;
		T1->R = nilT;
		T2->P = nilT;
		update(T1);
	}
}

TNode *join(TNode *T1, TNode *T2) {
	if (T1 == nilT)
		return T2;

	pushDown(T1);	//(***)
	while (T1->R != nilT) {
		T1 = T1->R;
		pushDown(T1);	//(***)
	}
	splay(T1);

	setR(T1, T2);
	update(T1);

	return T1;
}

void reverse(int i, int j) {
	TNode *T1 = nilT, *T2 = nilT, *T3 = nilT;

	split(root, j, T2, T3);
	split(T2, i - 1, T1, T2);
	T2->rev = !T2->rev;
	
	root = join(join(T1, T2), T3);
}

void insertElement(int i, int v) {		//insert an element to the list
	TNode *T1 = nilT, *T2 = nilT;

	if (i > root->len)
		i = root->len + 1;

	split(root, i - 1, T1, T2);

	root = new TNode();	//new TNode(); will set every variables in struct to: 0, NULL, false (default constructor). 
						//In the other hand, new TNode; will give every variable in struct random values.
	root->value = v;
	root->P = nilT;
	setL(root, T1);
	setR(root, T2);
	update(root);
}

void deleteElement(int i) {		//delete an element from the list
	TNode *x, *T1, *T2;

	x = nodeAt(root, i);
	
	splay(x);
	T1 = x->L;
	T1->P = nilT;
	T2 = x->R;
	T2->P = nilT;

	delete(x);
	x = NULL;

	root = join(T1, T2);
}

void updateElement(int i, int v) {		//change the value of an existing element in the list
	root = nodeAt(root, i);

	splay(root);
	root->value = v;
	update(root);
}

LL query(int i, int j) {		//find the sum a[i] + a[i + 1] + ... + a[j]
	TNode *T1 = nilT, *T2 = nilT, *T3 = nilT;
	LL result;

	split(root, j, T2, T3);
	split(T2, i - 1, T1, T2);
	result = T2->sum;
	root = join(join(T1, T2), T3);

	return result;
}

void printOut(TNode *T) {
	pushDown(T);	//(***)

	if (T->L != nilT)
		printOut(T->L);

	printf("%d ",T->value);

	if (T->R != nilT)
		printOut(T->R);
} 

void solve() {
	char c;
	int i, j, k, p, v;

	freopen("input.txt", "rt", stdin);
	//freopen("output.txt", "wt", stdout);

	//cout << "Number of operations: ";
	cin >> k;
	for (p = 1; p <= k; ++p) {
		//cout << "operation: ";
		cin >> c;

		switch (c) {
			case 'I':	cin >> i >> v; 
				insertElement(i, v);
				break;
			case 'D':	cin >> i;
				deleteElement(i);
				break;
			case 'U':	cin >> i >> v;
				updateElement(i, v);
				break;
			case 'Q':	cin >> i >> j;	
				cout << query(i, j) << endl;
				break;
			case 'R':	cin >> i >> j;	
				reverse(i, j);
				break;
			case 'P':	printOut(root);
				printf("\n");
				break;
		}
	}
}

void freeMemory(TNode *x) {
	if (x == nilT)
		return;

	freeMemory(x->L);
	freeMemory(x->R);

	delete(x); 
	x = NULL;
}

int main() {
	init();

	solve();

	freeMemory(root);

	return 0;
}