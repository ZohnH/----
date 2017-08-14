#include "BinTree.h"
#include "rand.h"
#include <iostream>
using namespace std;


template <class T> 
struct Increase //函数对象：递增一个T类对象
{
	virtual void operator() (T& e) { e++; }
}; //假设T可直接递增或已重载++


// 随机生成期望高度为h的二叉树
template <class T>  
bool randomBinTree(BinTree<T> & bt, BinNodePosi(T) x, int h)
{
	if (0 >= h) return false; //至多h层
	if (0 < dice(h)) //以1/h的概率终止当前分支的生长  dice()自定义函数生成随机数
		randomBinTree(bt, bt.insertAsLC(x, dice((T)h * h * h)), h - 1);
	if (0 < dice(h)) //以1/h的概率终止当前分支的生长
		randomBinTree(bt, bt.insertAsRC(x, dice((T)h * h * h)), h - 1);
	return true;
}

//// 在二叉树中随机确定一个节点位置
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


template <class T>  //先序遍历打印各元素
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
		if (HasRChild(*x)) //栈的特点先入后出，要现将右孩子入栈
			{cout << "(R) "; S.push(x->rc);}
		if (HasLChild(*x))
			{cout << "(L) "; S.push(x->lc);}
	}
}

template <class T>  //层次遍历打印各元素
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