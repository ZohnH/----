#pragma once 
#include "BinNode_macro_BASIC.h"
#include <stdio.h>
#include "../stack/stack@list.h"
#include "../queue/queue.h"

#define BinNodePosi(T) BinNode<T>*  //�ڵ�λ��
#define stature(p) ((p) ? (p)->height : -1) //�ڵ�߶ȣ��롰�����߶�Ϊ-1����Լ����ͳһ��
#define max(a,b) (a>b?a:b)

typedef enum{RB_RED, RB_BLACK} RBColor; //�ڵ���ɫ

template<class T> 
struct BinNode
{
	T data; 
	int height;  //���� �߶�
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc; //���� ����
	int npl; //Null Path Length����ʽ�ѣ�Ҳ��ֱ����height���棩
	RBColor color; //��ɫ���������

// ���캯��
	BinNode() :
		parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) { }
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, int h = 0,
		 int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }

// �����ӿ�
	int size();   //ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ(�����Լ���
	BinNodePosi(T) insertAsLC(T const&); //��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
	BinNodePosi(T) insertAsRC(T const&); //��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
	BinNodePosi(T) succ(); //ȡ��ǰ�ڵ��ֱ�Ӻ��
	template <class VST> void travLevel(VST&); //������α���
	template <class VST> void travPre(VST&); //����������� preorder
	template <class VST> void travIn(VST&);  //����������� inorder
	template <class VST> void travPost(VST&);  //����������� postorder

// �Ƚ������е���
	bool operator< (BinNode const& bn){ return data < bn.data; }  //С��
	bool operator> (BinNode const& bn){ return data > bn.data; }  //����
	bool operator== (BinNode const& bn){ return data == bn.data; } //����

	BinNodePosi(T) zig();  //˳ʱ����ת
	BinNodePosi(T) zag();  //��ʱ����ת
};






/*
 �ӿ�ʵ�� 
 ������export�ؼ��ֺ���Խ�ʵ�ַ��ڵ���һ���ļ���
*/

template <class T>
int BinNode<T>::size() //ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ(�����Լ���
{
	int s = 1; //���뱾��
	if (lc) s += lc->size(); //�ݹ������������ģ
	if (rc) s += rc->size(); //�ݹ������������ģ
	return s;
}  //O(n) = |size()|

template <class T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{ return lc = new BinNode(e, this); } //���죬data=e�� parent=this

template <class T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{ return rc = new BinNode(e, this); } //���죬data=e�� parent=this

template<class T>
BinNodePosi(T) BinNode<T>::succ()  //��λ�ڵ�v��ֱ�Ӻ��(��̰������������
{
	BinNodePosi(T) s = this; //��¼��̵���ʱ����
	if (rc) //�����Һ��ӣ���ֱ�Ӻ�̱����������У�����ؾ���
	{
		s = rc;   //��������
		while (HasLChild(*s))
			s = s->lc;  //�����С���Ľڵ�
	}
	else  //����ֱ�Ӻ��Ӧ�ǡ�����ǰ�ڵ���������������е�������ȡ�������ؾ���
	{
		while (IsRChild(*s)) 
			s = s->parent;  //������������֧�����ϳ����Ϸ��ƶ�
		s = s->parent; //����ٳ����Ϸ��ƶ�һ�������ִ�ֱ�Ӻ�̣�������ڣ�
	}
	return s;
}


//��������������㷨��������#1��  ʹ��ջ�ṹʵ��
template <class T, class VST> //Ԫ�����͡�������
void travPre_I1(BinNodePosi(T) x, VST& visit)
{   
	Stack<BinNodePosi(T)> S;  //����ջ
	if (x)
		S.push(x); //���ڵ���ջ(�Ӹ��ڵ㿪ʼ��
	while (!S.empty()) //��ջ���֮ǰ����ѭ��
	{
		x = S.pop();
		visit(x->data);  //���������ʵ�ǰ�ڵ㣬��ǿպ��ӵ���ջ����Ϊ���Һ���
		if (HasRChild(*x))
			S.push(x->rc);
		if (HasLChild(*x))
			S.push(x->lc);
	}
}

//��������������㷨��������#2��
//�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�Ľڵ㣻��;�ڵ���������������
template <class T, class VST> //Ԫ�����͡�������
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S)
{
	while (x)
	{
		visit(x->data); //���ʵ�ǰ�ڵ�
		S.push(x->rc);  //�Һ�����ջ�ݴ棨���Ż���ͨ���жϣ�����յ��Һ�����ջ��
		x = x->lc;  //�����֧����һ��
	} 
}
template <class T, class VST> //Ԫ�����͡�������
void travPre_I2(BinNodePosi(T) x, VST& visit)
{
	Stack<BinNodePosi(T)> S;  //����ջ
	while (true)
	{
		visitAlongLeftBranch(x, visit, S);  //�ӵ�ǰ�ڵ��������������
		if (S.empty())
			break;       //ֱ��ջ��
		x = S.pop();   //������һ�������
	}
}

//��������������㷨���ݹ�棩
template <class T, class VST> //Ԫ�����͡�������
void travPre_R(BinNodePosi(T) x, VST& visit)
{
	if (!x)
		return;
	visit(x->data);
	travPre_R(x->lc, visit);
	travPre_R(x->rc, visit);
}


//��������������㷨ͳһ���  ע�����template�����Σ�������������������
template <class T>   template <class VST> //Ԫ�����͡�������
void BinNode<T>::travPre(VST& visit) 
{
	switch (rand() % 3)
	{
	case 1: 
		travPre_I1(this, visit); break;
	case 2: 
		travPre_I2(this, visit); break;
	default: 
		travPre_R(this, visit); break;
	}
}


template<class T>  template<class VST>  //Ԫ�����͡�������
void BinNode<T>::travLevel(VST& visit) //��������α����㷨
{
	Queue<BinNodePosi(T)> Q; //��������
	Q.enqueue(this);  //���ڵ����
	while (!Q.empty()) //�ڶ����ٴα��֮ǰ����������
	{
		BinNodePosi(T) x = Q.dequeue(); 
		visit(x->data); //ȡ�����׽ڵ㲢����֮
		if (HasLChild(*x))
			Q.enqueue(x->lc); //�������
		if (HasRChild(*x))
			Q.enqueue(x->rc); //�Һ������
	}
}


template<class T>  //�ӵ�ǰ�ڵ�����������֧�������룬ֱ��û�����֧�Ľڵ�
static void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S)
{
	while (x)
	{  //��ǰ�ڵ���ջ���漴������֧���룬����ֱ��������
		S.push(x);
		x = x->lc;
	}
}

template<class T, class VST> //��������������㷨��������#1��
void travIn_I1(BinNodePosi(T) x, VST& visit)
{
	Stack<BinNodePosi(T)> S;
	while (true)
	{
		goAlongLeftBranch(x, S); //�ӵ�ǰ�ڵ������������ջ
		if (S.empty()) 
			break;      //ֱ�����нڵ㴦�����
		x = S.pop();
		visit(x->data); //����ջ���ڵ㲢����֮
		x = x->rc;      //ת��������
	} 
}

template<class T, class VST>  //��������������㷨��������#2��
void travIn_I2(BinNodePosi(T) x, VST& visit)
{
	Stack<BinNodePosi(T)> S; //����ջ
	while(true)
	{
		if (x)
		{
			S.push(x); //���ڵ��ջ
			x = x->lc; //�������������
		}
		else if (!S.empty())
		{
			x = S.pop();    //��δ���ʵ�������Ƚڵ���ջ
			visit(x->data);  //���ʸ����Ƚڵ�
			x = x->rc;    //�������ȵ�������
		}
		else
			break;  //�������
	}
}

template<class T, class VST>  //��������������㷨��������#3��
void travIn_I3(BinNodePosi(T) x, VST& visit)
{
	bool backtrack = false; //ǰһ���Ƿ�մ����������ݡ�---ʡȥջ����O(1)�����ռ�
	while (true)
	{
		if (!backtrack && HasLChild(*x)) //�����������Ҳ��Ǹոջ��ݣ���
			x=x->lc;                     //�������������
		else   //���򡪡�---����������ոջ��ݣ��൱������������
		{
			visit(x->data);   //���ʸýڵ�
			if (HasRChild(*x))   //�����������ǿգ���
			{
				x = x->rc;  //������������������
				backtrack = false; //���رջ��ݱ�־
			}
			else   //���������գ���
			{
				if (!(x = x->succ()))
					break;    //���ݣ����ִ�ĩ�ڵ�ʱ���˳����أ�
				backtrack = true;  //�����û��ݱ�־
			}
		}
	}
}

template<class T, class VST>  //���������������������#4������ջ���־λ��
void travIn_I4(BinNodePosi(T) x, VST& visit)
{
	while (true)
	{
		if (HasLChild(*x))
			x = x->lc;
		else
		{
			visit(x->data);
			while (!HasRChild(*x))
			{
				if (!(x = x->succ()))
					return;
				else
					visit(x->data);
			}
			x = x->rc;
		}
	}
}

template<class T, class VST>
void travIn_R(BinNodePosi(T) x, VST& visit) //��������������㷨���ݹ�棩
{
	if (!x)
		return;
	travIn_R(x->lc, visit);
	visit(x->data);
	travIn_R(x->rc, visit);
}

template<class T>  template<class VST>  //Ԫ�����͡�������
void BinNode<T>::travIn(VST& visit) //��������������㷨ͳһ���
{
	switch (rand() % 5)
	{
	case 1:travIn_I1(this, visit); break;
	case 2:travIn_I2(this, visit); break;
	case 3:travIn_I3(this, visit); break;
	case 4:travIn_I4(this, visit); break;
	default:travIn_R(this, visit); break;
	}
}


template<class T, class VST>
void travPost_R(BinNodePosi(T) x, VST& visit) //��������������㷨���ݹ�棩
{
	if (!x)	return;
	travPost_R(x->lc, visit);
	travPost_R(x->rc, visit);
	visit(x->data);
}

template<class T> //����Sջ���ڵ�Ϊ���������У��ҵ�������ɼ�Ҷ�ڵ�
static void gotoHLVFL(Stack <BinNodePosi(T)>& S) //highest leaf visible from left
{   //��;�����ڵ�������ջ
	while (BinNodePosi(T) x = S.top()) //�Զ����£�������鵱ǰ�ڵ㣨��ջ����
	{
		if (HasLChild(*x))  //����������
		{
			if (HasRChild(*x))
				S.push(x->rc);  //�����Һ��ӣ�������ջ
			S.push(x->lc); //Ȼ���ת������
		}
		else   //ʵ������
			S.push(x->rc); //������
	}
	S.pop(); //����֮ǰ������ջ���Ŀսڵ�
}

template<class T, class VST>
void travPost_I(BinNodePosi(T) x, VST& visit) //�������ĺ�������������棩
{
	Stack<BinNodePosi(T)> S;  //����ջ
	if (x)
		S.push(x); //���ڵ���ջ
	while (!S.empty())
	{
		if (S.top() != x->parent) //��ջ���ǵ�ǰ�ڵ�֮�������Ϊ�����֣�����ʱ��
			gotoHLVFL< T>(S);  //����������Ϊ��֮�����У��ҵ�HLVFL���൱�ڵݹ��������У�
		          //ģ�庯����׼���ã�gotoHLVFL< T >(S);  ����ָ��ģ���������ɶ�Ӧ�ĺ�����
		          //ģ�庯���Ǳ�׼���ã�gotoHLVFL(S);  ���ݲ��������Զ�ʶ������һ���µ����͵ĺ�����
		x = S.pop();
		visit(x->data);   //����ջ������ǰһ�ڵ�֮��̣���������֮
	}
}

template<class T>  template<class VST>  //Ԫ�����͡�������
void BinNode<T>::travPost(VST& visit) //��������������㷨ͳһ���
{
	switch (rand() % 2)
	{
	case 1:travPost_I(this, visit); break;
	default:travPost_R(this, visit); break;
	}
}

template<class T>
BinNodePosi(T) BinNode<T>::zig() //˳ʱ����ת
{
	BinNodePosi(T) lChild = lc;  //�µĵ�ǰ�ڵ�Ϊԭ�ȵ�ǰ�ڵ������
	lChild->parent = this->parent;
	if (lChild->parent)  //ȷ���µĵ�ǰ�ڵ��븸�ڵ�Ĺ�ϵ
		((this == lChild->parent->rc) ? lChild->parent->rc : lChild->parent->lc) = lChild;
	lc = lChild->rc;  //�µ�ǰ�ڵ���Һ��ӱ�Ϊԭ�ȵ�ǰ�ڵ�����ӣ�������
	if (lc)
		lc->parent = this;
	lChild->rc = this;     //ԭ�ȵ�ǰ�ڵ�Ϊ�µ�ǰ�ڵ���Һ���
	this->parent = lChild;
	return lChild;
}

template<class T>
BinNodePosi(T) BinNode<T>::zag() //��ʱ����ת
{
	BinNodePosi(T) rChild = rc;     //�µĵ�ǰ�ڵ�Ϊԭ�ȵ�ǰ�ڵ���Һ���
	rChild->parent = this->parent;
	if (rChild->parent)    //ȷ���µĵ�ǰ�ڵ��븸�ڵ�Ĺ�ϵ
		((this == rChild->parent->rc) ? rChild->parent->rc : rChild->parent->lc) = rChild;
	rc = rChild->lc;   //ԭ�ȵ�ǰ�ڵ���Һ������µ�ǰ�ڵ������
	if (rc)
		rc->parent = this;
	rChild->lc = this;    //ԭ�ȵ�ǰ�ڵ�Ϊ�µ�ǰ�ڵ������
	this->parent = rChild;
	return rChild; 
}