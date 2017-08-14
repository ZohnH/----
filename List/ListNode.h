#ifndef __LISTNODE__H__
#define __LISTNODE__H__


typedef int Rank;  //秩
#define ListNodePosi(T) ListNode<T>*  //列表结点位置

template <typename T> 
struct ListNode  //列表节点模板类（以双向链表形式实现）
{
//成员
	T data;
	ListNodePosi(T) pred;
	ListNodePosi(T) succ;

//构造函数
	ListNode() {}  //针对header和trailer的构造
	ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
		: data(e), pred(p), succ(s) {}  //默认构造器

//操作接口
	ListNodePosi(T) insertAsPred(T const& e)  //紧靠当前节点之前插入新节点  o(1)
	{
		ListNodePosi(T) x = new ListNode(e, pred, this);
		pred->succ = x; pred = x; return x;
	}

	ListNodePosi(T) insertAsSucc(T const& e)  //紧随当前节点之后插入新节点
	{
		ListNodePosi(T) x = new ListNode(e, this, succ);
		succ->pred = x; succ = x; return x;
	}
};

#endif