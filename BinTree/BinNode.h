#pragma once 
#include "BinNode_macro_BASIC.h"
#include <stdio.h>
#include "../stack/stack@list.h"
#include "../queue/queue.h"

#define BinNodePosi(T) BinNode<T>*  //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）
#define max(a,b) (a>b?a:b)

typedef enum{RB_RED, RB_BLACK} RBColor; //节点颜色

template<class T> 
struct BinNode
{
	T data; 
	int height;  //数据 高度
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc; //父亲 孩子
	int npl; //Null Path Length（左式堆，也可直接用height代替）
	RBColor color; //颜色（红黑树）

// 构造函数
	BinNode() :
		parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) { }
	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL, int h = 0,
		 int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }

// 操作接口
	int size();   //统计当前节点后代总数，亦即以其为根的子树的规模(包括自己）
	BinNodePosi(T) insertAsLC(T const&); //作为当前节点的左孩子插入新节点
	BinNodePosi(T) insertAsRC(T const&); //作为当前节点的右孩子插入新节点
	BinNodePosi(T) succ(); //取当前节点的直接后继
	template <class VST> void travLevel(VST&); //子树层次遍历
	template <class VST> void travPre(VST&); //子树先序遍历 preorder
	template <class VST> void travIn(VST&);  //子树中序遍历 inorder
	template <class VST> void travPost(VST&);  //子树中序遍历 postorder

// 比较器、判等器
	bool operator< (BinNode const& bn){ return data < bn.data; }  //小于
	bool operator> (BinNode const& bn){ return data > bn.data; }  //大于
	bool operator== (BinNode const& bn){ return data == bn.data; } //等于

	BinNodePosi(T) zig();  //顺时针旋转
	BinNodePosi(T) zag();  //逆时针旋转
};






/*
 接口实现 
 当加入export关键字后可以将实现放在单独一个文件中
*/

template <class T>
int BinNode<T>::size() //统计当前节点后代总数，亦即以其为根的子树的规模(包括自己）
{
	int s = 1; //计入本身
	if (lc) s += lc->size(); //递归计入左子树规模
	if (rc) s += rc->size(); //递归计入右子树规模
	return s;
}  //O(n) = |size()|

template <class T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{ return lc = new BinNode(e, this); } //构造，data=e， parent=this

template <class T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)
{ return rc = new BinNode(e, this); } //构造，data=e， parent=this

template<class T>
BinNodePosi(T) BinNode<T>::succ()  //定位节点v的直接后继(后继按中序遍历规则）
{
	BinNodePosi(T) s = this; //记录后继的临时变量
	if (rc) //若有右孩子，则直接后继必在右子树中，具体地就是
	{
		s = rc;   //右子树中
		while (HasLChild(*s))
			s = s->lc;  //最靠左（最小）的节点
	}
	else  //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
	{
		while (IsRChild(*s)) 
			s = s->parent;  //逆向地沿右向分支，不断朝左上方移动
		s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
	}
	return s;
}


//二叉树先序遍历算法（迭代版#1）  使用栈结构实现
template <class T, class VST> //元素类型、操作器
void travPre_I1(BinNodePosi(T) x, VST& visit)
{   
	Stack<BinNodePosi(T)> S;  //辅助栈
	if (x)
		S.push(x); //根节点入栈(从根节点开始）
	while (!S.empty()) //在栈变空之前反复循环
	{
		x = S.pop();
		visit(x->data);  //弹出并访问当前节点，其非空孩子的入栈次序为先右后左
		if (HasRChild(*x))
			S.push(x->rc);
		if (HasLChild(*x))
			S.push(x->lc);
	}
}

//二叉树先序遍历算法（迭代版#2）
//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template <class T, class VST> //元素类型、操作器
static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S)
{
	while (x)
	{
		visit(x->data); //访问当前节点
		S.push(x->rc);  //右孩子入栈暂存（可优化：通过判断，避免空的右孩子入栈）
		x = x->lc;  //沿左分支深入一层
	} 
}
template <class T, class VST> //元素类型、操作器
void travPre_I2(BinNodePosi(T) x, VST& visit)
{
	Stack<BinNodePosi(T)> S;  //辅助栈
	while (true)
	{
		visitAlongLeftBranch(x, visit, S);  //从当前节点出发，逐批访问
		if (S.empty())
			break;       //直到栈空
		x = S.pop();   //弹出下一批的起点
	}
}

//二叉树先序遍历算法（递归版）
template <class T, class VST> //元素类型、操作器
void travPre_R(BinNodePosi(T) x, VST& visit)
{
	if (!x)
		return;
	visit(x->data);
	travPre_R(x->lc, visit);
	travPre_R(x->rc, visit);
}


//二叉树先序遍历算法统一入口  注意这儿template用俩次！！！！！！！！！！
template <class T>   template <class VST> //元素类型、操作器
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


template<class T>  template<class VST>  //元素类型、操作器
void BinNode<T>::travLevel(VST& visit) //二叉树层次遍历算法
{
	Queue<BinNodePosi(T)> Q; //辅助队列
	Q.enqueue(this);  //根节点入队
	while (!Q.empty()) //在队列再次变空之前，反复迭代
	{
		BinNodePosi(T) x = Q.dequeue(); 
		visit(x->data); //取出队首节点并访问之
		if (HasLChild(*x))
			Q.enqueue(x->lc); //左孩子入队
		if (HasRChild(*x))
			Q.enqueue(x->rc); //右孩子入队
	}
}


template<class T>  //从当前节点出发，沿左分支不断深入，直至没有左分支的节点
static void goAlongLeftBranch(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S)
{
	while (x)
	{  //当前节点入栈后随即向左侧分支深入，迭代直到无左孩子
		S.push(x);
		x = x->lc;
	}
}

template<class T, class VST> //二叉树中序遍历算法（迭代版#1）
void travIn_I1(BinNodePosi(T) x, VST& visit)
{
	Stack<BinNodePosi(T)> S;
	while (true)
	{
		goAlongLeftBranch(x, S); //从当前节点出发，逐批入栈
		if (S.empty()) 
			break;      //直至所有节点处理完毕
		x = S.pop();
		visit(x->data); //弹出栈顶节点并访问之
		x = x->rc;      //转向右子树
	} 
}

template<class T, class VST>  //二叉树中序遍历算法（迭代版#2）
void travIn_I2(BinNodePosi(T) x, VST& visit)
{
	Stack<BinNodePosi(T)> S; //辅助栈
	while(true)
	{
		if (x)
		{
			S.push(x); //根节点进栈
			x = x->lc; //深入遍历左子树
		}
		else if (!S.empty())
		{
			x = S.pop();    //尚未访问的最低祖先节点退栈
			visit(x->data);  //访问该祖先节点
			x = x->rc;    //遍历祖先的右子树
		}
		else
			break;  //遍历完成
	}
}

template<class T, class VST>  //二叉树中序遍历算法（迭代版#3）
void travIn_I3(BinNodePosi(T) x, VST& visit)
{
	bool backtrack = false; //前一步是否刚从右子树回溯—---省去栈，仅O(1)辅助空间
	while (true)
	{
		if (!backtrack && HasLChild(*x)) //若有左子树且不是刚刚回溯，则
			x=x->lc;                     //深入遍历左子树
		else   //否则——---无左子树或刚刚回溯（相当于无左子树）
		{
			visit(x->data);   //访问该节点
			if (HasRChild(*x))   //若其右子树非空，则
			{
				x = x->rc;  //深入右子树继续遍历
				backtrack = false; //并关闭回溯标志
			}
			else   //若右子树空，则
			{
				if (!(x = x->succ()))
					break;    //回溯（含抵达末节点时的退出返回）
				backtrack = true;  //并设置回溯标志
			}
		}
	}
}

template<class T, class VST>  //二叉树中序遍历（迭代版#4，无需栈或标志位）
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
void travIn_R(BinNodePosi(T) x, VST& visit) //二叉树中序遍历算法（递归版）
{
	if (!x)
		return;
	travIn_R(x->lc, visit);
	visit(x->data);
	travIn_R(x->rc, visit);
}

template<class T>  template<class VST>  //元素类型、操作器
void BinNode<T>::travIn(VST& visit) //二叉树中序遍历算法统一入口
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
void travPost_R(BinNodePosi(T) x, VST& visit) //二叉树后序遍历算法（递归版）
{
	if (!x)	return;
	travPost_R(x->lc, visit);
	travPost_R(x->rc, visit);
	visit(x->data);
}

template<class T> //在以S栈顶节点为根的子树中，找到最高左侧可见叶节点
static void gotoHLVFL(Stack <BinNodePosi(T)>& S) //highest leaf visible from left
{   //沿途所遇节点依次入栈
	while (BinNodePosi(T) x = S.top()) //自顶而下，反复检查当前节点（即栈顶）
	{
		if (HasLChild(*x))  //尽可能向左
		{
			if (HasRChild(*x))
				S.push(x->rc);  //若有右孩子，优先入栈
			S.push(x->lc); //然后才转至左孩子
		}
		else   //实不得已
			S.push(x->rc); //才向右
	}
	S.pop(); //返回之前，弹出栈顶的空节点
}

template<class T, class VST>
void travPost_I(BinNodePosi(T) x, VST& visit) //二叉树的后序遍历（迭代版）
{
	Stack<BinNodePosi(T)> S;  //辅助栈
	if (x)
		S.push(x); //根节点入栈
	while (!S.empty())
	{
		if (S.top() != x->parent) //若栈顶非当前节点之父（则必为其右兄），此时需
			gotoHLVFL< T>(S);  //在以其右兄为根之子树中，找到HLVFL（相当于递归深入其中）
		          //模板函数标准调用：gotoHLVFL< T >(S);  根据指定模板类型生成对应的函数。
		          //模板函数非标准调用：gotoHLVFL(S);  根据参数类型自动识别生成一个新的类型的函数。
		x = S.pop();
		visit(x->data);   //弹出栈顶（即前一节点之后继），并访问之
	}
}

template<class T>  template<class VST>  //元素类型、操作器
void BinNode<T>::travPost(VST& visit) //二叉树后序遍历算法统一入口
{
	switch (rand() % 2)
	{
	case 1:travPost_I(this, visit); break;
	default:travPost_R(this, visit); break;
	}
}

template<class T>
BinNodePosi(T) BinNode<T>::zig() //顺时针旋转
{
	BinNodePosi(T) lChild = lc;  //新的当前节点为原先当前节点的左孩子
	lChild->parent = this->parent;
	if (lChild->parent)  //确定新的当前节点与父节点的关系
		((this == lChild->parent->rc) ? lChild->parent->rc : lChild->parent->lc) = lChild;
	lc = lChild->rc;  //新当前节点的右孩子变为原先当前节点的左孩子！！！！
	if (lc)
		lc->parent = this;
	lChild->rc = this;     //原先当前节点为新当前节点的右孩子
	this->parent = lChild;
	return lChild;
}

template<class T>
BinNodePosi(T) BinNode<T>::zag() //逆时针旋转
{
	BinNodePosi(T) rChild = rc;     //新的当前节点为原先当前节点的右孩子
	rChild->parent = this->parent;
	if (rChild->parent)    //确定新的当前节点与父节点的关系
		((this == rChild->parent->rc) ? rChild->parent->rc : rChild->parent->lc) = rChild;
	rc = rChild->lc;   //原先当前节点的右孩子是新当前节点的左孩子
	if (rc)
		rc->parent = this;
	rChild->lc = this;    //原先当前节点为新当前节点的左孩子
	this->parent = rChild;
	return rChild; 
}