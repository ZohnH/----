#pragma once 

#include "../BST二叉搜索树/BST.h" //基于BST实现Splay

template <class T>
class Splay : public BST<T> //由BST派生的Splay树模板类
{
protected:
	BinNodePosi(T) splay(BinNodePosi(T) v); //将v伸展至根

public:
	BinNodePosi(T)& search(const T& e); //查找（重写） 基类定义了virtual函数
	BinNodePosi(T) insert(const T& e);   //插入（重写）
	bool remove(const T& e);  //删除（重写）

};


template <class NodePosi> inline  //在节点*p与*lc（可能为空）之间建立父（左）子关系
void attachAsLChild(NodePosi p, NodePosi lc){ p->lc = lc; if (lc) lc->parent = p; }

template <class NodePosi> inline //在节点*p与*rc（可能为空）之间建立父（右）子关系
void attachAsRChild(NodePosi p, NodePosi rc){ p->rc = rc; if (rc) rc->parent = p; }

template <class T> //Splay树伸展算法：从节点v出发逐层伸展
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v)  //v是因最近访问而需伸展的节点位置
{
	if (!v)
		return NULL;
	BinNodePosi(T) p; BinNodePosi(T) g;  //*v的父亲与祖父
	while ((p = v->parent) && (g = p->parent))  //自下而上，反复对*v做双层伸展
	{
		BinNodePosi(T) gg = g->parent; //每轮之后*v都以原曾祖父（great-grand parent）为父
		if (IsLChild(*v))
		{
			if (IsLChild(*p)) //zig-zig
			{
				attachAsLChild(g, p->rc);  //以g为中心zig操作结果
				attachAsLChild(p, v->rc); //下面三项是以p为中心zig操作
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
			v->parent = NULL; //若*v原先的曾祖父*gg不存在，则*v现在应为树根
		else  //否则，*gg此后应该以*v作为左或右孩子
			(g == gg->lc) ? attachAsLChild(gg, v) : attachAsRChild(gg, v);
		updateHeight(g);  updateHeight(p);  updateHeight(v);
	}
// 双层伸展结束时，必有g == NULL，但p可能非空
	if (p = v->parent) //若p果真非空，则额外再做一次单旋
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
} //调整之后新树根应为被伸展的节点，故返回该节点的位置以便上层函数更新树根


template <class T>  //在伸展树中查找e
BinNodePosi(T)& Splay<T>::search(const T& e)
{
	BinNodePosi(T) p = searchIn(_root, e, _hot = nullptr);
	_root = splay(p ? p : _hot);  //将最后一个被访问的节点伸展至根
	return _root;
}//与其它BST不同，无论查找成功与否，_root都指向最后被访问的节点


template <class T>  //将关键码e插入伸展树中
BinNodePosi(T) Splay<T>::insert(const T& e)
{
	if (!_root) //处理原树为空的退化情况
	{
		_size++;
		return _root = new BinNode<T>(e);
	}
	//接口Splay::search()已集成了splay()伸展功能，故查找返回后，树根节点要么等于查找目标（查找成功），
	//要么就是_hot，而且恰为拟插入节点的直接前驱或直接后继（查找失败）。 
	if (e == search(e)->data)  //确认目标节点不存在
		return _root;
	_size++;
	BinNodePosi(T) t = _root; //创建新节点。以下调整<=7个指针以完成局部重构
	if (_root->data < e) //插入新根，以t和t->rc为左、右孩子
	{
		t->parent = _root = new BinNode<T>(e, nullptr, t, t->rc); //2 + 3个
		if (HasRChild(*t))   //<= 2个
		{
			t->rc->parent = _root;
			t->rc = nullptr;
		}
	}
	else  //插入新根，以t->lc和t为左、右孩子
	{
		t->parent = _root = new BinNode<T>(e, nullptr, t->lc, t); //2 + 3个
		if (HasLChild(*t)) //<= 2个
		{
			t->lc->parent = _root;
			t->lc = nullptr;
		}
	}
	updateHeightAbove(t); //更新t及其祖先（实际上只有_root一个）的高度
	return _root;  //新节点必然置于树根，返回之
} //无论e是否存在于原树中，返回时总有_root->data == e


template <class T>  //从伸展树中删除关键码e
bool Splay<T>::remove(const T& e)
{
	if (!_root || e != search(e)->data)
		return false; //若树空或目标不存在，则无法删除
////assert: 经search()后节点e已被伸展至树根_root
	BinNodePosi(T) w = _root;
	if (!HasLChild(*w))  //若无左子树，则直接删除
	{
		_root = _root->rc;
		if (_root)
			_root->parent = nullptr;
	}
	else if (!HasRChild(*w)) //若无右子树，也直接删除
	{
		_root= _root->lc;
		if (_root)
			_root->parent = nullptr;
	}
	else //若左右子树同时存在，则
	{
		BinNodePosi(T) lTree = _root->lc;
		lTree->parent = nullptr;  _root->lc = nullptr;    //暂时将左子树切除
		_root = _root->rc;  _root->parent = nullptr; //只保留右子树
		search(w->data); //以原树根为目标，做一次（必定失败的）查找(查找实在原来的右子树中进行)
///// assert: 至此，右子树中最小节点必伸展至根，且（因无雷同节点）其左子树必空，于是
		_root->lc = lTree;  lTree->parent = _root; //只需将原左子树接回原位即可
	}
	delete w;  _size--; //释放节点，更新规模
	if (_root)
		updateHeight(_root);  //此后，若树非空，则树根的高度需要更新
	return true; 
}//若目标节点存在且被删除，返回true；否则返回false