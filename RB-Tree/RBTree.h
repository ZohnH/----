#pragma once 

#include "../BST����������/BST.h"//����BSTʵ��RedBlack

#define IsBlack(p) (!(p) || (p)->color == RB_BLACK)  //�ⲿ�ڵ�Ҳ�����ڽڵ�
#define IsRed(p) ( ! IsBlack(p) )  //�Ǻڼ���############
#define BlackHeightUpdated(x) ( \
	(stature((x).lc) == stature((x).rc)) && \
	((x).height == (IsRed(&x) ? stature((x).lc) : stature((x).lc)+1)) \
	) //������߶ȸ�������

template <class T>
class RBTree : public BST<T>
{
protected:
	void solveDoubleRed(BinNodePosi(T) x); //˫������
	void solveDoubleBlack(BinNodePosi(T) x);  //˫������
	int updateHeight(BinNodePosi(T) x); //���½ڵ�x�ĸ߶�

public:
	BinNodePosi(T) insert(const T& e); //���루��д��
	bool remove(const T& e); //ɾ������д��
// BST::search()������ӿڿ�ֱ������
};
;
;
;



template <class T>
BinNodePosi(T) RBTree<T>::insert(const T& e)//��e��������
{
	BinNodePosi(T)& x = search(e);
	if (x)
		return x;  //ȷ��Ŀ�겻���ڣ������_hot�����ã�
	x = new BinNode<T>(e, _hot, nullptr, nullptr, -1);//������ڵ�x(��ɫĬ��λ��)����_hotΪ�����ڸ߶�-1
	_size++;  
	solveDoubleRed(x);
	return x ? x : _hot->parent;  //��˫�������󣬼��ɷ���
/* �������������xλNULLʱΪ�η���_hot->parent��
   ��x��p��g�����ڵ�λzig-zag����zag-zigʱ������3+4�ع�֮��x�䵽��p��parentλ�á�
   Ҳ���Ը��Ĵ��룺BinNodePosi(T) old_x = x; solveDoubleRed (x); return old_x;  
 */
}//����e�Ƿ������ԭ���У�����ʱ����x->data == e


/********************************************************************
* RedBlack˫������㷨������ڵ�x���丸��Ϊ��ɫ�����⡣��Ϊ�����������
*    RR-1��2����ɫ��ת��2�κڸ߶ȸ��£�1~2����ת�����ٵݹ�
*    RR-2��3����ɫ��ת��3�κڸ߶ȸ��£�0����ת����Ҫ�ݹ�
*********************************************************************/
template <class T>
void RBTree<T>::solveDoubleRed(BinNodePosi(T) x)
{
	if (IsRoot(*x)) //���ѣ��ݹ飩ת������������ת�ڣ������ڸ߶�Ҳ��֮����
	{ _root->color = RB_BLACK; _root->height++; return; }
	BinNodePosi(T) p = x->parent;  //����x�ĸ���p�ش���
	if (IsBlack(p))  /*���IsRoot������IsBlack�����Ĳ�����ͬ����Ϊ����ʱ��ͬ��ע�⣡����*/
		return;  //��pΪ�ڣ������ֹ����������
	BinNodePosi(T) g = p->parent; //��ȻpΪ�죬��x���游�ش��ڣ��ұ�Ϊ��ɫ
	BinNodePosi(T) u = uncle(x);
//���£���x�常u����ɫ�ֱ���
	BinNodePosi(T) = uncle(x);
	if (IsBlack(u)) //uΪ��ɫ����NULL��ʱ
	{
		if (IsLChild(*x) == IsLChild(*p))//��x��pͬ�ࣨ��zIg-zIg��zAg-zAg������
			p->color = RB_BLACK; //p�ɺ�ת�ڣ�x���ֺ�
		else //��x��p��ࣨ��zIg-zAg��zAg-zIg������
			x->color = RB_BLACK; //x�ɺ�ת�ڣ�p���ֺ�
		g->color = RB_RED;//g�ض��ɺ�ת��
/////// �����䱣֤�ܹ�����Ⱦɫ�������������ж϶��ò���ʧ
/////// ����ת�󽫸��úڡ������ú죬��������Ⱦɫ��Ч�ʸ���
		BinNodePosi(T) gg = g->parent;  //���游��great-grand parent��
		BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);  //��������������ڵ�
		r->parent = gg;  //��ԭ���游����
	}
	else //��uΪ��ɫ
	{
		p->color = RB_BLACK; p->height++; //p�ɺ�ת��
		u->color = RB_BLACK; u->height++; //u�ɺ�ת��
		if (!IsRoot(*g))  
			g->color = RB_RED;//g���Ǹ�����ת��
		solveDoubleRed(g); //��������g��������β�ݹ飬���Ż�Ϊ������ʽ��
	}
}


template <class T>
bool RBTree<T>::remove(const T& e) //�Ӻ������ɾ���ؼ���e
{
	BinNodePosi(T)& x = search(e);
	if (!x)    //ȷ��Ŀ����ڣ�����_hot�����ã�
		return false; 
	BinNodePosi(T) r = removeAt(x, _hot);//ʵʩɾ��
	if (!(--_size))
		return true;
//assert: _hotĳһ���Ӹձ�ɾ�����ұ�r��ָ�ڵ㣨������NULL�����档���¼���Ƿ�ʧ�⣬������Ҫ����
	if (!_hot) //���ձ�ɾ�����Ǹ��ڵ㣬�����úڣ������ºڸ߶�
		{ _root->color = RB_BLACK; updateHeight(_root); return true; }
// assert: ���£�ԭx����r���طǸ���_hot�طǿ�
	if (BlackHeightUpdated(*_hot)) //���������ȵĺ������Ȼƽ�⣬���������
		return true;
	if (IsRed(r))     //������rΪ�죬��ֻ������ת��
		{ r->color = RB_BLACK; r->height++; return true; }
// assert: ���£�ԭx����r����Ϊ��ɫ
	solveDoubleBlack(r); 
	return true;        //��˫�ڵ����󷵻�
}


/**************************************************************
* RedBlack˫�ڵ����㷨������ڵ�x�뱻������Ľڵ��Ϊ��ɫ������
* ��Ϊ�����๲���������
*    BB-1 ��2����ɫ��ת��2�κڸ߶ȸ��£�1~2����ת�����ٵݹ�
*    BB-2R��2����ɫ��ת��2�κڸ߶ȸ��£�0����ת�����ٵݹ�
*    BB-2B��1����ɫ��ת��1�κڸ߶ȸ��£�0����ת����Ҫ�ݹ�
*    BB-3 ��2����ɫ��ת��2�κڸ߶ȸ��£�1����ת��תΪBB-1��BB2R
*************************************************************/
template <class T>
void RBTree<T>::solveDoubleBlack(BinNodePosi(T) r)
{
	BinNodePosi(T) p = r ? r->parent : _hot;//r�ĸ���
	if (!p)
		return;
	BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc;//r���ֵ�
	if (IsBlack(s)) //�ֵ�sΪ��
	{
		BinNodePosi(T) t = nullptr;//s�ĺ캢�ӣ������Һ��ӽԺ죬�������ȣ��Ժ�ʱΪNULL��
		//ע�����������˳����Ϊ�����ȣ����Ժ��ж�������ɫ������������Һ��Ӷ�Ϊ�죬�����ո�ֵ�������ӣ���
		if (IsRed(s->rc)) t = s->rc; //����
		if (IsRed(s->lc)) t = s->lc; //����
		if (t)//��s�к캢�ӣ�BB-1
		{
			//printf("  case BB-1: Child ("); print(s->lc); printf(") of BLACK sibling ("); print(s); printf(") is RED\n");
			RBColor oldColor = p->color;//����ԭ�������ڵ�p��ɫ������t���丸�ס��游
	     // ���£�ͨ����ת��ƽ�⣬���������������Һ���Ⱦ��
			BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t);
			if (HasLChild(*b))  //����
				{b->lc->color=RB_BLACK; updateHeight(b->lc);}
			if (HasRChild(*b)) //����
				{ b->rc->color = RB_BLACK; updateHeight(b->rc); }
			b->color = oldColor; updateHeight(b);  //���������ڵ�̳�ԭ���ڵ����ɫ
		}
		else //��s�޺캢��
		{
			s->color = RB_RED; s->height--; //sת��
			if (IsRed(p))  //BB-2R
			{
				//printf("  case BB-2R: Both children ("); print(s->lc); printf(") and ("); print(s->rc); printf(") of BLACK sibling ("); print(s); printf(") are BLACK, and parent ("); print(p); printf(") is RED\n"); 
				//s���Ӿ��ڣ�p��
				p->color = RB_BLACK; //pת�ڣ����ڸ߶Ȳ���(��ΪǮ��sת����)
			}
			else
			{//BB-2B
				//printf("  case BB-2R: Both children ("); print(s->lc); printf(") and ("); print(s->rc); printf(") of BLACK sibling ("); print(s); printf(") are BLACK, and parent ("); print(p); printf(") is BLACK\n"); 
				//s���Ӿ��ڣ�p��
				p->height--; //p���ֺڣ����ڸ߶��½�
				solveDoubleBlack(p); //�ݹ�����
			}
		}
	}
	else //�ֵ�sΪ�죺BB-3
	{
		//printf("  case BB-3: sibling ("); print(s); printf(" is RED\n");
		//s�죨˫�Ӿ�ڣ�
		s->color = RB_BLACK; p->color = RB_RED; //sת�ڣ�pת��
		BinNodePosi(T) t = IsLChild(*s) ? s->lc : s->rc;//ȡt���丸sͬ��
		_hot = p;
		FromParentTo(*p) = rotateAt(t); //��t���丸�ס��游��ƽ�����(����������¸���ԭ�Ⱦɸ��ĸ�������)
		solveDoubleBlack(r);
	}
}