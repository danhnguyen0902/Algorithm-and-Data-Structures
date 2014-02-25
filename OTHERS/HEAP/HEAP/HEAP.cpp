/*--------------------------------------MIN HEAP--------------------------------------------*/
//The first index in heap: 1
//i's children: 2i and 2i + 1
//This code is a Min Heap code. To make it be a Max Heap, simply modify these places:
//- in upHeap(int i) function:		if (i == 1 || H[i] < H[i / 2])
//- in downHeap(int i) function:	if (j < last && H[j + 1] > H[j]) and if (H[j] > H[i])

#include <iostream>
#include <string.h>
using namespace std;

#define maxN 100000

struct Heap {
	int  H[maxN + 10];
	int last;										//number of elements in heap

	Heap() {
		memset(H, 0, sizeof(H));
		last = 0;
	}

	void swap(int &x, int &y) {
		int tmp = x;
		x = y;
		y = tmp;
	}

	void upHeap(int i) {							
		if (i == 1 || H[i] > H[i / 2])				//(i div 2) node is i's parent
			return; 
		swap(H[i], H[i / 2]);								

		upHeap(i / 2);
	}

	void downHeap(int i) {
		int j = i * 2;
		if (j > last)
			return;
		if (j < last && H[j + 1] < H[j])			//among two children nodes, pick the node that has smaller value
			j = j + 1;

		if (H[j] < H[i])							//DON'T EVER FORGET THIS ONE
			swap(H[i], H[j]);

		downHeap(j);
	}

	void push(int value) {							//add one more element to the heap
		++last;
		H[last] = value;							//make it the new last element
		
		upHeap(last);								//adjust heap
	}

	int pop(int pos) {								//return the value at the pos_th position in heap
		int tmp = H[pos];
		H[pos] = H[last];							//bring the last element's value to the pos_th element in heap
		--last;										//delete the last element
		
		upHeap(pos);								//***This step is not necessary if the pos_th element is always the top of the heap
		downHeap(pos);

		return tmp;
	}

	void display() {
		for (int i = 1; i <= last; ++i)
			printf("%d ", H[i]);
		
		printf("\n");
	}
};

int main() {
	Heap a;		//automatically runs into Heap() constructor

	/*a.push(1);
	a.push(6);
	a.push(8);
	a.push(9);
	a.push(7);
	a.push(5);
	a.push(2);
	a.push(10);
	a.push(11);
	a.push(16);
	a.push(15);
	a.push(17);
	a.push(19);
	a.push(12);*/

	a.push(15);
	a.push(10);
	a.push(17);

	a.display();

	int tmp = a.pop(1);

	a.display();

	return 0;
}