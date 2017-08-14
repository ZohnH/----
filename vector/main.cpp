#include "vector.h"
#include <iostream>
using std::cout; using std::endl;



int main()
{
	int A[10] = {1,5,9,7,3,10,4,8,6,2};
	Vector<int> V(A, 10);
	V.bubbleSort_fast(0, 10);
	for (int i = 0; i < 10; ++i)
		cout << V[i] << endl;

	return 0;
}

