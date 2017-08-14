/* BST:Binary Search Tree二叉搜索树
	顺序性：任一节点均不小于其左后代，不大于其右后代（注意是后代，而不是孩子）
*/

#pragma  once

#include "..\BinTree\BinTree.h"

#define EQUAL(e,v) (!(v) || (e)==(v)->data)  //节点v（或假想的通配哨兵）的关键码等于e


template <class T>
class BST : public BinTree<T> //由BinTree派生BST模板类
{
public:  //基本接口：以virtual修饰，强制要求所有派生类（BST变种）根据各自的规则对其重写
	virtual BinNodePosi(T)& search(const T& e); //查找
	virtual BinNodePosi(T) insert(const T& e); //插入
	virtual bool remove(const T& e); //删除

protected:
	BinNodePosi(T) _hot; //“命中”节点的父亲###########################
	BinNodePosi(T) connect34( //按照“3 + 4”结构，联接3个节点及四棵子树
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
	BinNodePosi(T) rotateAt(BinNodePosi(T) x);  //对x及其父亲、祖父做统一旋转调整

};





//static函数的作用域是本源文件，把它想象为面向对象中的private函数就可以了。
//static函数可以很好地解决不同源文件中函数同名的问题，因为一个源文件对于其他源文件中的static函数是不可见的。
//下面的函数用了static关键字，本意是将迭代版和递归版的函数全都命名成了searchIn
template <class T>  //在以v为根的（AVL、SPLAY、rbTree等）BST子树中查找关键码e
static BinNodePosi(T)& searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot)
{
	if (EQUAL(e, v)) //退化情况：在子树根节点v处命中或者v不存在
		return v;  //这儿必须先执行return 
	hot = v;   //这样才能使得hot始终指向最后一个失败节点，即命中节点的父节点
	
	while (1) //一般地，反复不断地
	{
		BinNodePosi(T)& c = (e < hot->data) ? hot->lc : hot->rc;//确定深入方向
		if (EQUAL(e, c)) //命中返回，或者深入一层
			return c;
		hot = c;
	} //hot始终指向最后一个失败节点
} //返回时，返回值指向命中节点（或假想的通配哨兵），hot指向其父亲（退化时为初始值NULL）

template <class T>  //递归版本--典型的尾递归，减而治之
static BinNodePosi(T)& searchIn_R(BinNodePosi(T) & v, const T& e, BinNodePosi(T) & hot)
{
	if (!v || (e == v->data))
		return v;   //递归基：在节点v（或假想的通配节点）处命中
	hot = v;     //一般情况：先记下当前（非空）节点，然后再
	return searchIn_R(((e < v->data) ? v->lc : v->rc), e, hot);//深入一层，递归查找
}//返回时，返回值指向命中节点（或假想的通配哨兵），hot指向其父亲（退化时为初始值NULL）

template <class T>
BinNodePosi(T)& BST<T>::search(const T& e)  //在BST中查找关键码e
{//语义规定：//返回时，返回值指向命中节点（或假想的通配哨兵），hot指向其父亲
	return searchIn(_root, e, _hot = nullptr);
}


//插入：先借助search(e)确定插入位置及方向，再将新节点作为叶子插入(规定： 禁止雷同元素）
//若e尚不存在，则：_hot为新节点的父亲， v=search(e)是_hot对新孩子的引用！！！！！
template <class T>
BinNodePosi(T) BST<T>::insert(const T& e)
{
	BinNodePosi(T)& x = search(e); //确认目标不存在（留意对_hot的设置）
	if (x)
		return x;     
	x = new BinNode<T>(e, _hot); //创建新节点x：以e为关键码，以_hot为父
	_size++;  //更新全树规模
	updateHeightAbove(x);   //更新x及其历代祖先的高度
	return x; //新插入的节点，必为叶子
} //无论e是否存在于原树中，返回时总有x->data == e


/******************************************************************************************
* BST节点删除算法：删除位置x所指的节点（全局静态模板函数，适用于AVL、Splay、RedBlack等各种BST）
* 目标x在此前经查找定位，并确认非NULL，故必删除成功；与searchIn不同，调用之前不必将hot置空
* 返回值指向实际被删除节点的接替者，hot指向实际被删除节点的父亲----二者均有可能是NULL
******************************************************************************************/
template <class T>
static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot)
{
	BinNodePosi(T) w = x; //实际被摘除的节点，初值同x
	BinNodePosi(T) succ = nullptr;  //实际被删除节点的接替者
	if (!HasLChild(*x)) //若*x的左子树为空，则可
		succ = x = x->rc; //直接将*x替换为其右子树
	else if (!HasRChild(*x))   //若右子树为空，则可
		succ = x = x->lc; //对称地处理——注意：此时succ != NULL
	else  //若左右子树均存在，则选择x的直接后继作为实际被摘除节点，为此需要
	{ 
		w = w->succ();  //（在右子树中）找到*x的直接后继*w
		swap(x->data, w->data);  //交换*x和*w的数据元素
		BinNodePosi(T) u = w->parent;
		((u == x) ? u->rc : u->lc) = succ = w->rc; //隔离节点*w(w必没左孩子)
	}       //当x的右孩子没有左后代时u==x，当x的右孩子有左后代时u！=x。
	hot = w->parent;  //记录实际被删除节点的父亲(这儿应该用w，第三种情况中相当于已经互换w和x）
	if (succ)
		succ->parent = hot;   //并将被删除节点的接替者与hot相联
	delete w;
	return succ; //释放被摘除节点，返回接替者
}

template <class T>
bool BST<T>::remove(const T& e)  //从BST树中删除关键码e
{
	BinNodePosi(T)& x = search(e);
	if (!x)
		return false;   //确认目标存在（留意_hot的设置）
	removeAt(x, _hot);	_size--;  //实施删除
	updateHeightAbove(_hot); //更新_hot及其历代祖先的高度
	return true;
}  //删除成功与否，由返回值指示


/**********************************************************
* 按照“3 + 4”结构联接3个节点及其四棵子树，返回重组之后的局部子树根节点位置（即b）
* 子树根节点与上层节点之间的双向联接，均须由上层调用者完成
* 可用于AVL和RedBlack的局部平衡调整
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
	return b;      //该子树新的根节点
}


/**************************************************
* BST节点旋转变换统一算法（3节点 + 4子树），返回调整之后局部子树根节点的位置
* 注意：尽管子树根会正确指向上层节点（如果存在），但反向的联接须由上层函数完成
*********************************************************/
template <class T>   //画出连接图  根据connect34中的连接图找出对应的参数
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)  //v为非空孙辈节点
{
	if (!v)
	{
		printf("\a\nFail to rotate a null node\n");
		exit(-1);
	}
	BinNodePosi(T) p = v->parent; //父亲
	BinNodePosi(T) g = p->parent; //祖父   //视v、p和g的相对位置分四种情况
	if (IsLChild(*p)) //zig
	{
		if (IsLChild(*v)) //zig-zig
		{
			p->parent = g->parent;   //向上联接 （p变成最后的祖先节点）
			return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
		}
		else //zig-zag
		{
			v->parent = g->parent;   //向上联接 （v变成最后的祖先节点）
			return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
		}
	}
	else  //zag
	{
		if (IsRChild(*v)) //zag-zag
		{
			p->parent = g->parent; //向上联接
			return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
		}
		else  //zag-zig
		{
			v->parent = g->parent; //向上联接
			return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
		}
	}
}