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
void testSplayPeriod(int n) //周期性访问测试
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

	//为提高效率，采用右移运算代替除法运算，带符号型整数采用的是算术右移（空位补的是原来的符号位的数）
	//当是负数时，与我们期望的结果不同。 处理办法是加校正量：2^k-1
	int m = n < 0 ? (n + 1) >> 1 : n >> 1;
	 
	splay.remove(m);
	printSplay<T>(splay); cout << endl; cout << endl;
}


template <class T>
void testSplayRandom(int n) //随机访问测试
{

}


int main()
{
	int n = 8;
	testSplayPeriod<int>(n);

	return 0;
}