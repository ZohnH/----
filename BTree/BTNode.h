#pragma once 

#include "../vector/vector.h"

#define BTNodePosi(T) BTNode<T>* //B-���ڵ�λ��

template <class T>
struct BTNode      //B-���ڵ�ģ����
{// ��Ա��Ϊ���������ͳһ���ţ��ɸ�����Ҫ��һ����װ structĬ��public��
	BTNodePosi(T) parent; //���ڵ�
	Vector<T> key;  //�ؼ�������
	Vector<BTNodePosi(T)> child; //�����������䳤���ܱ�key��һ��
// ���캯����ע�⣺BTNodeֻ����Ϊ���ڵ㴴�������ҳ�ʼʱ��0���ؼ����1���պ���ָ�룩
	BTNode() { parent = nullptr; child.insert(0, nullptr); } //�����Ǿ��и��պ��ӽڵ�
	BTNode(T e, BTNodePosi(T) lc = nullptr, BTNodePosi(T) rc = nullptr)
	{ 
		parent = nullptr;  //��Ϊ���ڵ㣬���ҳ�ʼʱ
		key.insert(0, e);  //ֻ��һ���ؼ��룬�Լ�
		child.insert(0, lc);  child.insert(1, rc); //��������
		if (lc) lc->parent = this;
		if (rc) rc->parent = this;
	}
};