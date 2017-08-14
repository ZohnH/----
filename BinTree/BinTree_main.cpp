#include "BinTree.h"
#include "rand.h"
#include <iostream>
using namespace std;


template <class T> 
struct Increase //�������󣺵���һ��T�����
{
	virtual void operator() (T& e) { e++; }
}; //����T��ֱ�ӵ�����������++


// ������������߶�Ϊh�Ķ�����
template <class T>  
bool randomBinTree(BinTree<T> & bt, BinNodePosi(T) x, int h)
{
	if (0 >= h) return false; //����h��
	if (0 < dice(h)) //��1/h�ĸ�����ֹ��ǰ��֧������  dice()�Զ��庯�����������
		randomBinTree(bt, bt.insertAsLC(x, dice((T)h * h * h)), h - 1);
	if (0 < dice(h)) //��1/h�ĸ�����ֹ��ǰ��֧������
		randomBinTree(bt, bt.insertAsRC(x, dice((T)h * h * h)), h - 1);
	return true;
}

//// �ڶ����������ȷ��һ���ڵ�λ��
//template <class T> 
//BinNodePosi(T) randomPosiInBinTree(BinNodePosi(T) root)
//{
//	if (!HasChild(*root)) return root;
//	if (!HasLChild(*root))
//		return dice(6) ? randomPosiInBinTree(root->rc) : root;
//	if (!HasRChild(*root))
//		return dice(6) ? randomPosiInBinTree(root->lc) : root;
//	return dice(2) ?
//		randomPosiInBinTree(root->lc):
//		randomPosiInBinTree(root->rc);
//}


template <class T>  //���������ӡ��Ԫ��
void printTreePrev(const BinTree<T>& t)
{
	BinNodePosi(T) root = t.root();
	Stack<BinNodePosi(T)> S;
	if (root)
		S.push(root);
	BinNodePosi(T) x;
	while (!S.empty())
	{
		x = S.pop();
		cout << x->data << "  ";
		if (HasRChild(*x)) //ջ���ص���������Ҫ�ֽ��Һ�����ջ
			{cout << "(R) "; S.push(x->rc);}
		if (HasLChild(*x))
			{cout << "(L) "; S.push(x->lc);}
	}
}

template <class T>  //��α�����ӡ��Ԫ��
void printTreeLevel(const BinTree<T>& t)
{
	BinNodePosi(T) root = t.root();
	Queue<BinNodePosi(T)> Q;
	if (root)
		Q.enqueue(root);
	BinNodePosi(T) x;
	while (!Q.empty())
	{
		x = Q.dequeue();
		cout << x->data << "  ";
		if (HasLChild(*x))
			{cout << "(L) "; Q.enqueue(x->lc);}
		if (HasRChild(*x))
			{cout << "(R) "; Q.enqueue(x->rc);}
	}
}


int main()
{
	srand((unsigned int)time(NULL));
	int h = 7;
	BinTree<int> bt;
	bt.insertAsRoot(dice(h * h * h));
	randomBinTree(bt, bt.root(), h);  printTreeLevel(bt);  printf("\n");

	bt.travPrev(Increase<int>()); printTreeLevel(bt);  printf("\n");


	return 0;
}