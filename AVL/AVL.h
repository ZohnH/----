#pragma once 

#include "../BST����������/BST.h" //����BSTʵ��AVL��

#define Balanced(x) ( stature((x).lc) )==( stature((x).rc) )//����ƽ������
#define BalFac(x) ( stature((x).lc) - stature((x).rc) )  //ƽ������
#define AvlBalanced(x) ( (-2 < BalFac(x)) && (BalFac(x) < 2) )  //AVLƽ������

/**********************************************************
* �����Һ�����ȡ������
* ��AVLƽ�����ǰ�����ȷ���ع�����
*********************************************************/
#define tallerChild(x) (\
	stature((x)->lc) > stature((x)->rc) ? (x)->lc : ( /*���*/ \
	stature((x)->lc) < stature((x)->rc) ? (x)->rc : ( /*�Ҹ�*/ \
	IsLChild(*(x)) ? (x)->lc : (x)->rc  /*�ȸߣ��븸��xͬ���ߣ�zIg-zIg��zAg-zAg������*/ \
) ) )

template <class T>
class AVL : public BST<T>  //��BST����AVL��ģ����
{
public:
	BinNodePosi(T) insert(const T& e);  //���루��д�� ���ඨ����virtual����
	bool remove(const T& e);  //ɾ������д��
	// BST::search()������ӿڿ�ֱ������

};



template <class T>
BinNodePosi(T) AVL<T>::insert(const T& e)  //���ؼ���e����AVL����
{
	BinNodePosi(T)& x = search(e);  
	if (x)
		return x;   //AVL�����Ѵ��ڹؼ���e
	BinNodePosi(T) xx = x = new BinNode<T>(e, _hot);  //�����½ڵ�x
	_size++;
// ��ʱ��x�ĸ���_hot�����ߣ������游�п���ʧ��
	for (BinNodePosi(T) g = _hot; g; g = g->parent)
	{ //��x֮���������ϣ�������������g
		if (!AvlBalanced(*g))  //һ������gʧ�⣬�򣨲��á�3 + 4���㷨��ʹ֮���⣬��������
		{   //FromParentTo()�����ǵĸ����ܹ���������������
			FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g))); //���½���ԭ��
			break;    //g����󣬾ֲ������߶ȱ�Ȼ��ԭ�������������ˣ��ʵ����漴����#############
		}
		else   //����g��Ȼƽ�⣩��ֻ��򵥵�
			updateHeight(g); //������߶ȣ�ע�⣺����gδʧ�⣬�߶���������ӣ�
	}   //����ֻ��һ�ε�����������������������ȫ���߶ȱ�Ȼ��ԭ
	return xx; //�����½ڵ�λ��
}  //����e�Ƿ������ԭ���У�����AVL::insert(e)->data == e


template <class T>
bool AVL<T>::remove(const T& e) //��AVL����ɾ���ؼ���e
{
	BinNodePosi(T)& x = search(e); //����ʱ������ֵָ�����нڵ㣨������ͨ���ڱ�����_hotָ���丸��
	if (!x)
		return false;  //ȷ��Ŀ����ڣ�����_hot�����ã�
	removeAt(x, _hot);  //�Ȱ�BST����ɾ��֮���˺�ԭ�ڵ�֮��_hot�������Ⱦ�����ʧ�⣩
	_size--;
	for (BinNodePosi(T) g = _hot; g; g = g->parent)
	{  //��_hot�������ϣ�������������g
		if (!AvlBalanced(*g))  //һ������gʧ�⣬�򣨲��á�3 + 4���㷨��ʹ֮���⣬��������������
			g = FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));  //ԭ����
		updateHeight(g);  //��������߶ȣ�ע�⣺����gδʧ�⣬�߶�����ܽ��ͣ�
	}  //��������Omega(logn)�ε������������Ƿ�����������ȫ���߶Ⱦ����ܽ���
	return true; //ɾ���ɹ�
} //��Ŀ��ڵ�����ұ�ɾ��������true�����򷵻�false