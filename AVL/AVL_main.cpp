#include "AVL.h"
#include <time.h>
#include <iostream>
using namespace std;


template <class T>
void printAVL(const AVL<T>& avl)
{
	BinNodePosi(T) root = avl.root();
	Queue<BinNodePosi(T)> Q;
	Q.enqueue(root);
	while (!Q.empty())
	{
		BinNodePosi(T) x = Q.dequeue();
		cout << x->data << " ";
		if (HasLChild(*x))
			{cout << "(L) "; Q.enqueue(x->lc);}
		if (HasRChild(*x))
			{cout << "(R) "; Q.enqueue(x->rc);}
	}
}


int main()
{
	srand((unsigned int)time(NULL));
	int n = 15;
	AVL<int> avl;
	while (avl.size() < n)
	{
		avl.insert(rand() % (n * 3));
	}
	printAVL(avl); cout << endl;

	int m = rand() % (n * 3);
	cout << "remove: "<< m << endl;
	cout << "remove " << avl.remove(m) << endl;
	printAVL(avl); cout << endl;

	return 0;
}