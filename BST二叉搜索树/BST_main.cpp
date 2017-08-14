#include "BST.h"
#include <time.h>
#include <iostream>
using namespace std;
#include "../BinTree/rand.h"


template <class T>
void printBST(const BST<T>& bst)
{
	BinNodePosi(T) root = bst.root();
	Queue<BinNodePosi(T)> Q;
	if (root)
		Q.enqueue(root);
	while (!Q.empty())
	{
		BinNodePosi(T) x = Q.dequeue();
		cout << x->data << " ";
		if (HasLChild(*x))
			{cout << "(L)"; Q.enqueue(x->lc);}
		if (HasRChild(*x))
			{cout << "(R)"; Q.enqueue(x->rc);}
	}

}


int main()
{
	srand((unsigned int)time(NULL));
	BST<int> bst;
	int n = 13;
	while (bst.size() < n)
		bst.insert(rand() % (n * 3));
	printBST(bst);
	cout << endl;

	return 0;
}