/* BST:Binary Search Tree����������
	˳���ԣ���һ�ڵ����С�������������������Һ����ע���Ǻ���������Ǻ��ӣ�
*/

#pragma  once

#include "..\BinTree\BinTree.h"

#define EQUAL(e,v) (!(v) || (e)==(v)->data)  //�ڵ�v��������ͨ���ڱ����Ĺؼ������e


template <class T>
class BST : public BinTree<T> //��BinTree����BSTģ����
{
public:  //�����ӿڣ���virtual���Σ�ǿ��Ҫ�����������ࣨBST���֣����ݸ��ԵĹ��������д
	virtual BinNodePosi(T)& search(const T& e); //����
	virtual BinNodePosi(T) insert(const T& e); //����
	virtual bool remove(const T& e); //ɾ��

protected:
	BinNodePosi(T) _hot; //�����С��ڵ�ĸ���###########################
	BinNodePosi(T) connect34( //���ա�3 + 4���ṹ������3���ڵ㼰�Ŀ�����
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
	BinNodePosi(T) rotateAt(BinNodePosi(T) x);  //��x���丸�ס��游��ͳһ��ת����

};





//static�������������Ǳ�Դ�ļ�����������Ϊ��������е�private�����Ϳ����ˡ�
//static�������Ժܺõؽ����ͬԴ�ļ��к���ͬ�������⣬��Ϊһ��Դ�ļ���������Դ�ļ��е�static�����ǲ��ɼ��ġ�
//����ĺ�������static�ؼ��֣������ǽ�������͵ݹ��ĺ���ȫ����������searchIn
template <class T>  //����vΪ���ģ�AVL��SPLAY��rbTree�ȣ�BST�����в��ҹؼ���e
static BinNodePosi(T)& searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot)
{
	if (EQUAL(e, v)) //�˻���������������ڵ�v�����л���v������
		return v;  //���������ִ��return 
	hot = v;   //��������ʹ��hotʼ��ָ�����һ��ʧ�ܽڵ㣬�����нڵ�ĸ��ڵ�
	
	while (1) //һ��أ��������ϵ�
	{
		BinNodePosi(T)& c = (e < hot->data) ? hot->lc : hot->rc;//ȷ�����뷽��
		if (EQUAL(e, c)) //���з��أ���������һ��
			return c;
		hot = c;
	} //hotʼ��ָ�����һ��ʧ�ܽڵ�
} //����ʱ������ֵָ�����нڵ㣨������ͨ���ڱ�����hotָ���丸�ף��˻�ʱΪ��ʼֵNULL��

template <class T>  //�ݹ�汾--���͵�β�ݹ飬������֮
static BinNodePosi(T)& searchIn_R(BinNodePosi(T) & v, const T& e, BinNodePosi(T) & hot)
{
	if (!v || (e == v->data))
		return v;   //�ݹ�����ڽڵ�v��������ͨ��ڵ㣩������
	hot = v;     //һ��������ȼ��µ�ǰ���ǿգ��ڵ㣬Ȼ����
	return searchIn_R(((e < v->data) ? v->lc : v->rc), e, hot);//����һ�㣬�ݹ����
}//����ʱ������ֵָ�����нڵ㣨������ͨ���ڱ�����hotָ���丸�ף��˻�ʱΪ��ʼֵNULL��

template <class T>
BinNodePosi(T)& BST<T>::search(const T& e)  //��BST�в��ҹؼ���e
{//����涨��//����ʱ������ֵָ�����нڵ㣨������ͨ���ڱ�����hotָ���丸��
	return searchIn(_root, e, _hot = nullptr);
}


//���룺�Ƚ���search(e)ȷ������λ�ü������ٽ��½ڵ���ΪҶ�Ӳ���(�涨�� ��ֹ��ͬԪ�أ�
//��e�в����ڣ���_hotΪ�½ڵ�ĸ��ף� v=search(e)��_hot���º��ӵ����ã���������
template <class T>
BinNodePosi(T) BST<T>::insert(const T& e)
{
	BinNodePosi(T)& x = search(e); //ȷ��Ŀ�겻���ڣ������_hot�����ã�
	if (x)
		return x;     
	x = new BinNode<T>(e, _hot); //�����½ڵ�x����eΪ�ؼ��룬��_hotΪ��
	_size++;  //����ȫ����ģ
	updateHeightAbove(x);   //����x�����������ȵĸ߶�
	return x; //�²���Ľڵ㣬��ΪҶ��
} //����e�Ƿ������ԭ���У�����ʱ����x->data == e


/******************************************************************************************
* BST�ڵ�ɾ���㷨��ɾ��λ��x��ָ�Ľڵ㣨ȫ�־�̬ģ�庯����������AVL��Splay��RedBlack�ȸ���BST��
* Ŀ��x�ڴ�ǰ�����Ҷ�λ����ȷ�Ϸ�NULL���ʱ�ɾ���ɹ�����searchIn��ͬ������֮ǰ���ؽ�hot�ÿ�
* ����ֵָ��ʵ�ʱ�ɾ���ڵ�Ľ����ߣ�hotָ��ʵ�ʱ�ɾ���ڵ�ĸ���----���߾��п�����NULL
******************************************************************************************/
template <class T>
static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot)
{
	BinNodePosi(T) w = x; //ʵ�ʱ�ժ���Ľڵ㣬��ֵͬx
	BinNodePosi(T) succ = nullptr;  //ʵ�ʱ�ɾ���ڵ�Ľ�����
	if (!HasLChild(*x)) //��*x��������Ϊ�գ����
		succ = x = x->rc; //ֱ�ӽ�*x�滻Ϊ��������
	else if (!HasRChild(*x))   //��������Ϊ�գ����
		succ = x = x->lc; //�ԳƵش�����ע�⣺��ʱsucc != NULL
	else  //���������������ڣ���ѡ��x��ֱ�Ӻ����Ϊʵ�ʱ�ժ���ڵ㣬Ϊ����Ҫ
	{ 
		w = w->succ();  //�����������У��ҵ�*x��ֱ�Ӻ��*w
		swap(x->data, w->data);  //����*x��*w������Ԫ��
		BinNodePosi(T) u = w->parent;
		((u == x) ? u->rc : u->lc) = succ = w->rc; //����ڵ�*w(w��û����)
	}       //��x���Һ���û������ʱu==x����x���Һ���������ʱu��=x��
	hot = w->parent;  //��¼ʵ�ʱ�ɾ���ڵ�ĸ���(���Ӧ����w��������������൱���Ѿ�����w��x��
	if (succ)
		succ->parent = hot;   //������ɾ���ڵ�Ľ�������hot����
	delete w;
	return succ; //�ͷű�ժ���ڵ㣬���ؽ�����
}

template <class T>
bool BST<T>::remove(const T& e)  //��BST����ɾ���ؼ���e
{
	BinNodePosi(T)& x = search(e);
	if (!x)
		return false;   //ȷ��Ŀ����ڣ�����_hot�����ã�
	removeAt(x, _hot);	_size--;  //ʵʩɾ��
	updateHeightAbove(_hot); //����_hot�����������ȵĸ߶�
	return true;
}  //ɾ���ɹ�����ɷ���ֵָʾ


/**********************************************************
* ���ա�3 + 4���ṹ����3���ڵ㼰���Ŀ���������������֮��ľֲ��������ڵ�λ�ã���b��
* �������ڵ����ϲ�ڵ�֮���˫�����ӣ��������ϲ���������
* ������AVL��RedBlack�ľֲ�ƽ�����
***************************************************************/
template <class T>
BinNodePosi(T) BST<T>::connect34(BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c, 
	BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3)
{
	a->lc = T0;
	if (T0)
		T0->parent = a;
	a->rc = T1;
	if (T1)
		T1->parent = a;
	updateHeight(a);
	c->lc = T2;
	if (T2)
		T2->parent = c;
	c->rc = T3;
	if (T3)
		T3->parent = c;
	updateHeight(c);
	b->lc = a;
	a->parent = b;
	b->rc = c;
	c->parent = b;
	updateHeight(b);
	return b;      //�������µĸ��ڵ�
}


/**************************************************
* BST�ڵ���ת�任ͳһ�㷨��3�ڵ� + 4�����������ص���֮��ֲ��������ڵ��λ��
* ע�⣺��������������ȷָ���ϲ�ڵ㣨������ڣ�������������������ϲ㺯�����
*********************************************************/
template <class T>   //��������ͼ  ����connect34�е�����ͼ�ҳ���Ӧ�Ĳ���
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)  //vΪ�ǿ��ﱲ�ڵ�
{
	if (!v)
	{
		printf("\a\nFail to rotate a null node\n");
		exit(-1);
	}
	BinNodePosi(T) p = v->parent; //����
	BinNodePosi(T) g = p->parent; //�游   //��v��p��g�����λ�÷��������
	if (IsLChild(*p)) //zig
	{
		if (IsLChild(*v)) //zig-zig
		{
			p->parent = g->parent;   //�������� ��p����������Ƚڵ㣩
			return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
		}
		else //zig-zag
		{
			v->parent = g->parent;   //�������� ��v����������Ƚڵ㣩
			return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
		}
	}
	else  //zag
	{
		if (IsRChild(*v)) //zag-zag
		{
			p->parent = g->parent; //��������
			return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
		}
		else  //zag-zig
		{
			v->parent = g->parent; //��������
			return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
		}
	}
}