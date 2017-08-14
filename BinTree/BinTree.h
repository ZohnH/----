#pragma once

#include "BinNode.h"

template <class T>
class BinTree  //二叉树模板类
{
protected:
	int _size;  BinNodePosi(T) _root;  //规模 根结点
	virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度 定义为虚方法便于派生类对这个方法进行适当重写
	void updateHeightAbove(BinNodePosi(T) x); //更新节点x及祖先的高度

public:
	BinTree() : _size(0), _root(NULL) {};  //构造函数
	

	int size() const { return _size; }  //规模 const成员函数
	bool empty() const { return !_root; } //判空 没根就空
	BinNodePosi(T) root() const { return _root; } //树根
	BinNodePosi(T) insertAsRoot(T const& e); //插入根节点
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e作为x的左孩子（原无）插入
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e作为x的右孩子（原无）插入
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T); //T作为x左子树接入
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T); //T作为x右子树接入
	int remove(BinNodePosi(T) x); //删除以位置x处节点为根的子树，返回该子树原先的规模

	template <class VST> //操作器
	void travLevel(VST& visit){ if (_root) _root->travLevel(visit); }  //层次遍历
	template <class VST> //操作器
	void travPrev(VST& visit){ if (_root) _root->travPre(visit); } //先序遍历
	template <class VST> //操作器
	void travIn(VST& visit){ if (_root) _root->travIn(visit); } //先序遍历

};
 





template<typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //更新节点x的高度
{
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
}

template<class T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while (x)  //从x出发，覆盖历代祖先。可优化(一旦高度未变即可终止)
	{ updateHeight(x); x = x->parent;}
}

template<class T> //将e当作根节点插入空的二叉树
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{
	_size=1; 
	return _root = new BinNode<T>(e);
}

template<class T>  //e作为x的左孩子（原无）插入
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsLC(e);
	updateHeightAbove(x);
	return x->lc;
}

template<class T>  //e作为x的右孩子（原无）插入
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rc;
}

