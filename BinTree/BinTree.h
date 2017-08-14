#pragma once

#include "BinNode.h"

template <class T>
class BinTree  //������ģ����
{
protected:
	int _size;  BinNodePosi(T) _root;  //��ģ �����
	virtual int updateHeight(BinNodePosi(T) x); //���½ڵ�x�ĸ߶� ����Ϊ�鷽�������������������������ʵ���д
	void updateHeightAbove(BinNodePosi(T) x); //���½ڵ�x�����ȵĸ߶�

public:
	BinTree() : _size(0), _root(NULL) {};  //���캯��
	

	int size() const { return _size; }  //��ģ const��Ա����
	bool empty() const { return !_root; } //�п� û���Ϳ�
	BinNodePosi(T) root() const { return _root; } //����
	BinNodePosi(T) insertAsRoot(T const& e); //������ڵ�
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e��Ϊx�����ӣ�ԭ�ޣ�����
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e��Ϊx���Һ��ӣ�ԭ�ޣ�����
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T); //T��Ϊx����������
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T); //T��Ϊx����������
	int remove(BinNodePosi(T) x); //ɾ����λ��x���ڵ�Ϊ�������������ظ�����ԭ�ȵĹ�ģ

	template <class VST> //������
	void travLevel(VST& visit){ if (_root) _root->travLevel(visit); }  //��α���
	template <class VST> //������
	void travPrev(VST& visit){ if (_root) _root->travPre(visit); } //�������
	template <class VST> //������
	void travIn(VST& visit){ if (_root) _root->travIn(visit); } //�������

};
 





template<typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //���½ڵ�x�ĸ߶�
{
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
}

template<class T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while (x)  //��x�����������������ȡ����Ż�(һ���߶�δ�伴����ֹ)
	{ updateHeight(x); x = x->parent;}
}

template<class T> //��e�������ڵ����յĶ�����
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{
	_size=1; 
	return _root = new BinNode<T>(e);
}

template<class T>  //e��Ϊx�����ӣ�ԭ�ޣ�����
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsLC(e);
	updateHeightAbove(x);
	return x->lc;
}

template<class T>  //e��Ϊx���Һ��ӣ�ԭ�ޣ�����
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{
	_size++;
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rc;
}

