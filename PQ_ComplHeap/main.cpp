#include "PQ_ComplHeap.h"
#include <time.h>
#include <stdlib.h>

template <class PQ, class T>//堆类型、词条类型
void teseHeap(int n)
{
	T *A = new T[n];
	for (int i = 0; i < n; ++i)
		A[i] = rand() % ((T)5 * n); 
	PQ heap(A, n); //批量建堆（PQ_ComplHeap实现了Robert Floyd算法）
	delete[]A;
}


template <class T>
void testInset(int n)
{
	PQ_ComplHeap<T> pq;
	T elem;
	for (int i = 0; i < n; ++i)
	{
		elem = rand() % ((T)5 * n);
		pq.insert(elem);
	}
	for (int i = 0; i < n; ++i)
		printf("%d  ", pq[i]);
	printf("\n");
	pq.heapSort();
	for (int i = 0; i < n; ++i)
		printf("%d  ", pq[i]);
	printf("\n");
}

int main()
{
	srand((unsigned int)time(NULL));
	int m = 15;

//	teseHeap<PQ_ComplHeap<int>, int>(m);

	testInset<int>(m);

	return 0;
}