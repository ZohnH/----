#include "Splay.h"
#include <iostream>
using namespace std;


template <class T>
void printSplay(const Splay<T>& splay)
{
	BinNodePosi(T) root = splay.root();
	Queue<BinNodePosi(T)> Q;
	if (root)
		Q.enqueue(root);
	while (!Q.empty())
	{
		BinNodePosi(T) x = Q.dequeue();
		cout << x->data << " ";
		if (HasLChild(*x))
		{
			cout << "(L)"; Q.enqueue(x->lc);
		}
		if (HasRChild(*x))
		{
			cout << "(R)"; Q.enqueue(x->rc);
		}
	}
}


template <class T>
void testSplayPeriod(int n) //�����Է��ʲ���
{
	Splay<T> splay;
	for (int i = 1; i <= n; ++i)
		splay.insert((T)i);
	printSplay<T>(splay); cout << endl; cout << endl;

	for (int i = 1; i <= n; ++i)
	{
		splay.search((T)i); 
		cout << "Search " << i << " : ";
		printSplay<T>(splay); 
		cout << endl;
	}
	cout << endl;
	printSplay<T>(splay); cout << endl; cout << endl;

	//Ϊ���Ч�ʣ����������������������㣬���������������õ����������ƣ���λ������ԭ���ķ���λ������
	//���Ǹ���ʱ�������������Ľ����ͬ�� ����취�Ǽ�У������2^k-1
	int m = n < 0 ? (n + 1) >> 1 : n >> 1;
	 
	splay.remove(m);
	printSplay<T>(splay); cout << endl; cout << endl;
}


template <class T>
void testSplayRandom(int n) //������ʲ���
{

}


int main()
{
	int n = 8;
	testSplayPeriod<int>(n);

	return 0;
}