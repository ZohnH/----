#pragma  once

#include <stdio.h>
#include "ListNode.h"  ////引入列表节点类

template <typename T> 
class List   //列表模板类
{
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;  //规模、头哨兵、尾哨兵

protected:
	void init(); //列表创建时的初始化
	int clear(); //清除所有节点
	void copyNodes(ListNodePosi(T), int ); //复制列表中自位置p起的n项
	void selectionSort(ListNodePosi(T), int); //对从p开始连续的n个节点选择排序
	void insertionSort(ListNodePosi(T), int); //对从p开始连续的n个节点插入排序
	void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int);//归并
	void mergeSort(ListNodePosi(T)&, int); //对从p开始连续的n个节点归并排序

public:
// 构造函数
	List(){ init(); }   //默认
	List(List<T> const& L);  //整体复制列表L
	List(List<T> const& L, Rank r, int n);  //复制列表L中自第r项起的n项
	List(ListNodePosi(T) p, int n);   //复制列表中自位置p起的n项


// 析构函数
	~List(); //释放（包含头、尾哨兵在内的）所有节点
	

// 只读访问接口
	Rank size() const { return _size; } //规模
	bool empty() const { return _size <= 0; }
	T& operator[] (Rank r) const;  //重载，支持循秩访问（效率低）
	ListNodePosi(T) find(T const& e) const //无序列表查找出等于e的节点
	{ return find(e, _size, trailer); }
	ListNodePosi(T) find( T const& e, int n, ListNodePosi(T) p ) const; //无序区间查找
	ListNodePosi(T) first() const { return header->succ; } //首节点位置
	ListNodePosi(T) last() const {return trailer->pred;} //末节点位置
	bool valid(ListNodePosi(T) p) //判断位置p是否对外合法
	{ return p && (trailer != p) && (header != p); }
	int disordered() const; //判断列表是否已排序 //统计逆序相邻元素对的总数
	ListNodePosi(T) search(T const& e) //有序列表查找
	{	return search(e, _size, trailer);	} //前驱中找到不大于e的最后者
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;  //有序区间查找
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);  //在p及其n-1个后继中选出最大者
	ListNodePosi(T) selectMax(){ return selectMax(header->succ, _size); }  //整体最大者
	T remove(ListNodePosi(T) p); //删除合法位置p处的节点,返回被删除节点
	

// 可写访问接口
	ListNodePosi(T) insertAsFirst(T const& e)   //将e当作首节点插入
	{ 
		_size++;
		return header->insertAsSucc(e); //头节点的后继是首节点
	}

	ListNodePosi(T) insertAsLast(T const& e)   //e当作末节点插入
	{
		_size++;
		return trailer->insertAsPred(e);  //尾节点的前驱是末节点
	}

	ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e)  //将e当作p的前驱插入（Before）
	{  
		_size++; 
		return p->insertAsPred(e);
	}

	ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e)  //e当作p的后继插入
	{
		_size++;
		return p->insertAsSucc(e);
	}


	int deduplicate(); //无序去重
	int uniquify(); //有序去重
	void sort(ListNodePosi(T), int);
	void sort(){ sort(first(), _size); }
};







template <typename T>
void List<T>::init()
{
	header = new ListNode<T>;   //创建头哨兵节点
	trailer = new ListNode<T>;  //创建尾哨兵节点
	header->succ = trailer;  header->pred = NULL;
	trailer->pred = header;  trailer->succ = NULL;
	_size = 0;
}


template <typename T>
int List<T>::clear()  //清除所有节点
{
	int oldSize = _size;
	while (0 < _size)   //反复删除首节点，直至列表变空
		remove(header->succ); //首节点
	return oldSize;
}


template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)  //复制列表中自位置p起的n项
{
	init();  //创建头、尾哨兵节点并做初始化
	while (n--)
	{  //将起自p的n项依次作为末节点插入
		insertAsLast(p->data);
		p = p->succ;
	}
}


template <typename T> //复制列表中自位置p起的n项（assert: p为合法位置，且至少有n-1个后继节点）
List<T>::List(ListNodePosi(T) p, int n)
{
	copyNodes(p, n);
}

template <typename T> //整体复制列表L
List<T>::List(List<T> const& L)
{
	copyNodes(L.first(), L._size);
}

template <typename T>//复制L中自第r项起的n项（assert: r+n <= L._size）
List<T>::List(List<T> const& L, Rank r, int n)
{
	copyNodes(L[r], n);
}

template <typename T> List<T>::~List() //列表析构器
{
	clear(); delete header; delete trailer;
} //清空列表，释放头、尾哨兵节点

template <typename T>//重载下标操作符，以通过秩直接访问列表节点（虽方便，效率低，需慎用）
T& List<T>::operator[](Rank r) const
{
	ListNodePosi(T) p = first(); //从首节点出发
	while (0 < r--){ p = p->succ; } //顺数第r个节点即是
	return p->data;  //目标节点，返回其中所存元素
}

template <typename T>  //统计逆序相邻元素对的总数
int List<T>::disordered() const
{
	int n = 0;
	ListNodePosi(T) p = first();
	for (int i = 0; i < _size - 1; ++i, p = p->succ)
	{
		if (p->data > p->succ->data)
			n++;
	}
	return n;
}


// 在节点p（可能是trailer）的n个（真）前驱中，找到等于e的最后者
template <typename T>
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const
{
	while (0 < n--) //（0 <= n <= rank(p) < _size）对于p的最近的n个前驱，从右向左
	{
		if (e == (p = p->pred)->data) //逐个比对，直至命中或范围越界
			return p;
	}
	return NULL; //p越出左边界意味着区间内不含e，查找失败
} //header的存在使得处理更为简洁


template <typename T>
T List<T>::remove(ListNodePosi(T) p)  //删除合法位置p处的节点,返回被删除节点.  o(1)
{
	T e = p->data;  //备份待删除节点的数值（假定T类型可直接赋值）
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	delete p;
	_size--;
	return e; //返回备份的数值
}

template <typename T>
int List<T>::deduplicate() //剔除无序列表中的重复节点
{
	if (_size < 2) return 0; //平凡列表自然无重复
	int oldSize = _size; //记录原规模
	ListNodePosi(T) p = header;  Rank r = 0;
	while (trailer != (p = p->succ))
	{
		ListNodePosi(T) q = find(p->data, r, p); //在p的r个（真）前驱中查找雷同者
		q ? remove(q) : r++; //若的确存在，则删除之；否则秩加一  ————应该删除q而不是p，循环条件中要求p的后继
	}                         //删除的是在当前元素前面找到的与当前元素相等的节点
	return oldSize - _size;  //列表规模变化量，即被删除元素总数
}


template <typename T>
int List<T>::uniquify()  //成批剔除重复元素，效率更高  o(n)
{
	if (_size < 2) return 0;
	int oldSize = _size;
	ListNodePosi(T) p = first(); ListNodePosi(T) q; //p为各区段起点，q为其后继
	while (trailer != (q = p->succ)) //反复考查紧邻的节点对(p, q)
	{
		if (p->data != q->data)
			p = q;  //若互异，则转向下一区段
		else
			remove(q);  //否则（雷同），删除后者
	}
	return oldSize - _size;
}


template <typename T>  //在有序列表内节点p（可能是trailer）的n个（真）前驱中，找到不大于e的最后者
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const
{
	while (0 <= n--)   //0 <= n-- 要找到头节点为止
	{
		if (((p = p->pred)->data) <= e) //从右向左 逐个比较 直至命中或越界
			break;
	}
	return p;
}


template <typename T>  //从起始于位置p的n个元素中选出最大者
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) max = p;  //最大者暂定为首节点p
	for (ListNodePosi(T) cur = p; 1 < n; n--)
	{
		if (((cur = cur->succ)->data) >= max->data) //这儿去≥与查找命令的语义对等 
			max = cur;           //选出不小于p的最靠后的元素
	}
	return max;
}

template <typename T> //列表的选择排序算法：对起始于位置p的n个元素排序
void List<T>::selectionSort(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) head = p->pred;
	ListNodePosi(T) tail = p;
	for (int i = 0; i < n; i++)
		tail = tail->succ;   //待排序区间为(head, tail)
	while (1 < n)  //在至少还剩两个节点之前，在待排序区间内
	{
		ListNodePosi(T) max = selectMax(head->succ, n);
		insertBefore(tail, remove(max));
		tail = tail->pred;
		n--;
	}
}

template<typename T>  //列表的插入排序算法：对起始于位置p的n个元素排序
void List<T>::insertionSort(ListNodePosi(T) p, int n)
{
	for (int r = 0; r < n; r++) //逐一为各节点
	{
		insertAfter(search(p->data, r, p), p->data); //查找适当的位置并插入
		p = p->succ;   //转向下一节点(因为有尾节点哨兵，所以这步安全）
		remove(p->pred); //删除p本身
	}
}


template<typename T>//有序列表的归并：当前列表中自p起的n个元素，与列表L中自q起的m个元素归并
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m)
{ //因为是在p上修改 所以输入为p的引用
	// assert:  this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
	//          L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
	// 注意：在归并排序之类的场合，有可能 this == L && rank(p) + n = rank(q)
	ListNodePosi(T) pp = p->pred;  //借助前驱（可能是header），以便返回前 ...
	while (0 < m) //在q尚未移出区间之前
	{
		if ((0 < n) && (p->data <= q->data)) //若p仍在区间内且v(p) <= v(q)，则
		{
			if (q == (p = p->succ)) //p归入合并的列表，并替换为其直接后继
				break; n--;
		}
		else //若p已超出右界或v(q) < v(p)，则
		{
			insertBefore(p, L.remove((q = q->succ)->pred));
			m--; //将q转移至p之前
		}
	}
	p = pp->succ; //确定归并后区间的（新）起点
}


template<typename T> //列表的归并排序算法：对起始于位置p的n个元素排序
void List<T>::mergeSort(ListNodePosi(T)& p, int n)
{   //valid(p) && rank(p) + n <= size
	if (n < 2) return; //若待排序范围已足够小，则直接返回；否则...
	int m = n >> 1; //以中点为界 //均分列表
	ListNodePosi(T) q = p;
	for (int i = 0; i < m; i++)
		q = q->succ;
	mergeSort(p, m);      //对前、后子列表分别排序
	mergeSort(q, n - m);
	merge(p, m, *this, q, n - m);  //归并
}

template <typename T> 
void List<T>::sort(ListNodePosi(T) p, int n) 
{ //列表区间排序
	switch (rand() % 3) 
	{ //随机选取排序算法。可根据具体问题的特点灵活选取或扩充
	case 1:  insertionSort(p, n); break; //插入排序
	case 2:  selectionSort(p, n); break; //选择排序
	default: mergeSort(p, n); break; //归并排序
	}

//	insertionSort(p, n);
}

