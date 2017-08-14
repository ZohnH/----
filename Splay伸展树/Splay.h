#pragma once 

#include "../BST����������/BST.h" //����BSTʵ��Splay

template <class T>
class Splay : public BST<T> //��BST������Splay��ģ����
{
protected:
	BinNodePosi(T) splay(BinNodePosi(T) v); //��v��չ����

public:
	BinNodePosi(T)& search(const T& e); //���ң���д�� ���ඨ����virtual����
	BinNodePosi(T) insert(const T& e);   //���루��д��
	bool remove(const T& e);  //ɾ������д��

};


template <class NodePosi> inline  //�ڽڵ�*p��*lc������Ϊ�գ�֮�佨���������ӹ�ϵ
void attachAsLChild(NodePosi p, NodePosi lc){ p->lc = lc; if (lc) lc->parent = p; }

template <class NodePosi> inline //�ڽڵ�*p��*rc������Ϊ�գ�֮�佨�������ң��ӹ�ϵ
void attachAsRChild(NodePosi p, NodePosi rc){ p->rc = rc; if (rc) rc->parent = p; }

template <class T> //Splay����չ�㷨���ӽڵ�v���������չ
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v)  //v����������ʶ�����չ�Ľڵ�λ��
{
	if (!v)
		return NULL;
	BinNodePosi(T) p; BinNodePosi(T) g;  //*v�ĸ������游
	while ((p = v->parent) && (g = p->parent))  //���¶��ϣ�������*v��˫����չ
	{
		BinNodePosi(T) gg = g->parent; //ÿ��֮��*v����ԭ���游��great-grand parent��Ϊ��
		if (IsLChild(*v))
		{
			if (IsLChild(*p)) //zig-zig
			{
				attachAsLChild(g, p->rc);  //��gΪ����zig�������
				attachAsLChild(p, v->rc); //������������pΪ����zig����
				attachAsRChild(p, g);
				attachAsRChild(v, p);
			}
			else //p->lc=v g->rc=p  zig-zag
			{
				attachAsLChild(p, v->rc);
				attachAsRChild(g, v->lc);
				attachAsLChild(v, g);
				attachAsRChild(v, p);
			}
		}
		else if (IsRChild(*p)) //zag-zag
		{
			attachAsRChild(g, p->lc);
			attachAsRChild(p, v->lc);
			attachAsLChild(p, g);
			attachAsLChild(v, p);
		}
		else  //zag-zig   p->rc=v g->lc=p 
		{ 
			attachAsRChild(p, v->lc);
			attachAsLChild(g, v->rc);
			attachAsRChild(v, g);
			attachAsLChild(v, p);
		}
		if (!gg)
			v->parent = NULL; //��*vԭ�ȵ����游*gg�����ڣ���*v����ӦΪ����
		else  //����*gg�˺�Ӧ����*v��Ϊ����Һ���
			(g == gg->lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
		updateHeight(g);  updateHeight(p);  updateHeight(v);
	}
// ˫����չ����ʱ������g == NULL����p���ܷǿ�
	if (p = v->parent) //��p����ǿգ����������һ�ε���
	{
		if (IsLChild(*v))
		{
			attachAsLChild(p, v->rc);
			attachAsRChild(v, p);
		}
		else
		{
			attachAsRChild(p, v->lc);
			attachAsLChild(v, p);
		}
		updateHeight(p);  updateHeight(v);
	}
	v->parent = NULL;
	return v;
} //����֮��������ӦΪ����չ�Ľڵ㣬�ʷ��ظýڵ��λ���Ա��ϲ㺯����������


template <class T>  //����չ���в���e
BinNodePosi(T)& Splay<T>::search(const T& e)
{
	BinNodePosi(T) p = searchIn(_root, e, _hot = nullptr);
	_root = splay(p ? p : _hot);  //�����һ�������ʵĽڵ���չ����
	return _root;
}//������BST��ͬ�����۲��ҳɹ����_root��ָ����󱻷��ʵĽڵ�


template <class T>  //���ؼ���e������չ����
BinNodePosi(T) Splay<T>::insert(const T& e)
{
	if (!_root) //����ԭ��Ϊ�յ��˻����
	{
		_size++;
		return _root = new BinNode<T>(e);
	}
	//�ӿ�Splay::search()�Ѽ�����splay()��չ���ܣ��ʲ��ҷ��غ������ڵ�Ҫô���ڲ���Ŀ�꣨���ҳɹ�����
	//Ҫô����_hot������ǡΪ�����ڵ��ֱ��ǰ����ֱ�Ӻ�̣�����ʧ�ܣ��� 
	if (e == search(e)->data)  //ȷ��Ŀ��ڵ㲻����
		return _root;
	_size++;
	BinNodePosi(T) t = _root; //�����½ڵ㡣���µ���<=7��ָ������ɾֲ��ع�
	if (_root->data < e) //�����¸�����t��t->rcΪ���Һ���
	{
		t->parent = _root = new BinNode<T>(e, nullptr, t, t->rc); //2 + 3��
		if (HasRChild(*t))   //<= 2��
		{
			t->rc->parent = _root;
			t->rc = nullptr;
		}
	}
	else  //�����¸�����t->lc��tΪ���Һ���
	{
		t->parent = _root = new BinNode<T>(e, nullptr, t->lc, t); //2 + 3��
		if (HasLChild(*t)) //<= 2��
		{
			t->lc->parent = _root;
			t->lc = nullptr;
		}
	}
	updateHeightAbove(t); //����t�������ȣ�ʵ����ֻ��_rootһ�����ĸ߶�
	return _root;  //�½ڵ��Ȼ��������������֮
} //����e�Ƿ������ԭ���У�����ʱ����_root->data == e


template <class T>  //����չ����ɾ���ؼ���e
bool Splay<T>::remove(const T& e)
{
	if (!_root || e != search(e)->data)
		return false; //�����ջ�Ŀ�겻���ڣ����޷�ɾ��
////assert: ��search()��ڵ�e�ѱ���չ������_root
	BinNodePosi(T) w = _root;
	if (!HasLChild(*w))  //��������������ֱ��ɾ��
	{
		_root = _root->rc;
		if (_root)
			_root->parent = nullptr;
	}
	else if (!HasRChild(*w)) //������������Ҳֱ��ɾ��
	{
		_root= _root->lc;
		if (_root)
			_root->parent = nullptr;
	}
	else //����������ͬʱ���ڣ���
	{
		BinNodePosi(T) lTree = _root->lc;
		lTree->parent = nullptr;  _root->lc = nullptr;    //��ʱ���������г�
		_root = _root->rc;  _root->parent = nullptr; //ֻ����������
		search(w->data); //��ԭ����ΪĿ�꣬��һ�Σ��ض�ʧ�ܵģ�����(����ʵ��ԭ�����������н���)
///// assert: ���ˣ�����������С�ڵ����չ�������ң�������ͬ�ڵ㣩���������ؿգ�����
		_root->lc = lTree;  lTree->parent = _root; //ֻ�轫ԭ�������ӻ�ԭλ����
	}
	delete w;  _size--; //�ͷŽڵ㣬���¹�ģ
	if (_root)
		updateHeight(_root);  //�˺������ǿգ��������ĸ߶���Ҫ����
	return true; 
}//��Ŀ��ڵ�����ұ�ɾ��������true�����򷵻�false