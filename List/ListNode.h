#ifndef __LISTNODE__H__
#define __LISTNODE__H__


typedef int Rank;  //��
#define ListNodePosi(T) ListNode<T>*  //�б���λ��

template <typename T> 
struct ListNode  //�б�ڵ�ģ���ࣨ��˫��������ʽʵ�֣�
{
//��Ա
	T data;
	ListNodePosi(T) pred;
	ListNodePosi(T) succ;

//���캯��
	ListNode() {}  //���header��trailer�Ĺ���
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		: data(e), pred(p), succ(s) {}  //Ĭ�Ϲ�����

//�����ӿ�
	ListNodePosi(T) insertAsPred(T const& e)  //������ǰ�ڵ�֮ǰ�����½ڵ�  o(1)
	{
		ListNodePosi(T) x = new ListNode(e, pred, this);
		pred->succ = x; pred = x; return x;
	}

	ListNodePosi(T) insertAsSucc(T const& e)  //���浱ǰ�ڵ�֮������½ڵ�
	{
		ListNodePosi(T) x = new ListNode(e, this, succ);
		succ->pred = x; succ = x; return x;
	}
};

#endif