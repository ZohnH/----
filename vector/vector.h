#pragma once 

#include "../fibonacci/fib.h"  //引入Fib数列类  用于Fibonacci查找算法

typedef int Rank;  //秩
#define DEFAULT_CAPACITY  3  //默认的初始容量（实际应用中可设置为更大）


template<class T>
class Vector  //向量模板类
{
protected:
	Rank _size;  int _capacity;  T* _elem; //规模、容量、数据区
	void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
	void expand(); //空间不足时扩容
	void shrink(); //装填因子过小时压缩

public:
	Rank bubble_fast(Rank lo, Rank hi); //扫描交换
	void bubbleSort_fast(Rank lo, Rank hi); //起泡排序算法
	Rank max(Rank lo, Rank hi); //选取最大元素
	void selectionSort(Rank lo, Rank hi); //选择排序算法


public:
//构造函数
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
		{ _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);} //s<=c
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间
// 析构函数
	~Vector(){ delete[] _elem; }  //释放内部空间
	
// 只读访问接口
	Rank size() const { return _size; } //规模
	bool empty() const { return !_size; } //判空


// 可写访问接口
	T& operator[] (Rank r) const; //重载下标操作符，可以类似于数组形式引用各元素
	Vector<T> & operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量
	Rank insert(Rank r, T const& e); //插入元素
	Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
	T remove(Rank r); //删除秩为r的元素
	int remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
	Rank search(T const& e) const //有序向量整体查找
	{ return (0 >= _size) ? -1 : search(e, 0, _size); }
	Rank search(T const& e, Rank lo, Rank hi) const;  //有序向量区间查找
};




template <class T>
void swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

template<class T>    //以数组区间A[lo, hi)为蓝本复制向量
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)
{
	_elem = new T[_capacity = 2 * (hi - lo)]; //分配空间
	_size = 0;   //规模清零
	while (lo < hi)
		_elem[_size++] = A[lo++];  //复制至_elem[0, hi - lo)
}

template<class T>
void Vector<T>::expand()   //向量空间不足时扩容
{
	if (_size < _capacity) //尚未满员时，不必扩容
		return;
	if (_capacity < DEFAULT_CAPACITY)
		_capacity = DEFAULT_CAPACITY;  //不低于最小容量
	T* oldElem = _elem;
	_elem = new T[_capacity <<= 1];  //容量加倍
	for (int i = 0; i < _size; ++i)
		_elem[i] = oldElem[i]; //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
	delete [] oldElem; //释放原空间
}

template<class T>
void Vector<T>::shrink()  //装填因子过小时压缩向量所占空间
{
	if (_capacity < DEFAULT_CAPACITY << 1)
		return; //不致收缩到DEFAULT_CAPACITY以下
	if (_size << 2 > _capacity) 
		return; //以25%为界
	T* oldElem = _elem; 
	_elem = new T[_capacity >>= 1]; //容量减半
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i]; //复制原向量内容
	delete[] oldElem; //释放原空间
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V)
{
	if (_elem) delete[] _elem; //释放原有内容
	copyFrom(V._elem, 0, V.size()); //整体复制
	return *this;  //返回当前对象的引用，以便链式赋值
}

template <class T>  //重载下标操作符，可以类似于数组形式引用各元素
T& Vector<T>::operator[](Rank r) const
{
	return _elem[r];
}

template <class T>    //将e作为秩为r元素插入
Rank Vector<T>::insert(Rank r, T const& e)
{
	expand();   //若有必要，扩容
	for (int i = _size; i > r; i--)
		_elem[i] = _elem[i - 1]; //自后向前，后继元素顺次后移一个单元
	_elem[r] = e;
	_size++;        //置入新元素并更新容量
	return r;   //返回秩
}

template <class T>
int Vector<T>::remove(Rank lo, Rank hi) //删除区间[lo, hi)
{
	if (lo == hi)
		return 0;  //出于效率考虑，单独处理退化情况，比如remove(0, 0)
	while (hi < _size)
		_elem[lo++] = _elem[hi++];  //[hi, _size)顺次前移hi - lo个单元
	_size = lo; //更新规模(lo已更新)，直接丢弃尾部[lo, _size = hi)区间
	shrink(); //若有必要，则缩容
	return hi - lo;
}

template <class T>
T Vector<T>::remove(Rank r) //删除向量中秩为r的元素，0 <= r < size
{
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}


//// 二分查找算法（版本A）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
//template <typename T> 
//static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) 
//{
//	while (lo < hi)  //每步迭代可能要做两次比较判断，有三个分支
//	{
//		Rank mi = (lo + hi) >> 1; //以中点为轴点
//		if (e < A[mi]) 
//			hi = mi; //深入前半段[lo, mi)继续查找
//		else if (A[mi] < e) 
//			lo = mi + 1; //深入后半段(mi, hi)继续查找
//		else            
//			return mi; //在mi处命中
//	} //成功查找可以提前终止
//	return -1; //查找失败
//}//有多个命中元素时，不能保证返回秩最大者；查找失败时，简单地返回-1，而不能指示失败的位置

// 二分查找算法（版本C）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <class T>
static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)
{
	while (lo < hi)   //每步迭代仅需做一次比较判断，有两个分支
	{ 
		Rank mi = (lo + hi) >> 1;  //以中点为轴点
		// 注意这儿 e<A[mi]  以及判断后对lo的操作：lo=mi+1
		(e < A[mi]) ? hi = mi : lo = mi+1; //经比较后确定深入[lo, mi)或(mi, hi)
	}
	return --lo; //循环结束时，lo为大于e的元素的最小秩，故lo-1即不大于e的元素的最大秩
}  //有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置

// Fibonacci查找算法：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <class T> 
static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi)
{
	Fib fib(hi - lo);
	while (lo < hi)
	{
		while (hi - lo < fib.get())
			fib.prev();   //通过向前顺序查找（分摊O(1)）
		Rank mi = lo + fib.get() - 1; //确定形如Fib(k) - 1的轴点
		(e < A[mi]) ? hi = mi : lo = mi + 1;
	} //成功查找不能提前终止
	return --lo; //循环结束时，lo为大于e的元素的最小秩，故lo - 1即不大于e的元素的最大秩
} //有多个命中元素时，总能保证返回最秩最大者；查找失败时，能够返回失败的位置

template <class T> //在有序向量的区间[lo, hi)内，确定不大于e的最后一个节点的秩
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{  //按各50%的概率随机使用二分查找或Fibonacci查找
	return (rand() % 2) ? binSearch(_elem,e,lo,hi) : fibSearch(_elem,e,lo,hi);
}


//改进的冒泡排序：通过记录最右侧逆序对的位置，使得算法在常系数意义上有所改进
template <class T> 
Rank Vector<T>::bubble_fast(Rank lo, Rank hi)  //一趟扫描交换
{
	Rank last = lo;  //最右侧的逆序对初始化为[lo - 1, lo]
	while (++lo < hi) //自左向右，逐一检查各对相邻元素
	{
		if (_elem[lo - 1] > _elem[lo])
		{
			last = lo; //若逆序，则更新最右侧逆序对位置记录，并
			swap(_elem[lo - 1], _elem[lo]);  //通过交换使局部有序
		}	
	} 
	return last;//返回最右侧的逆序对位置
}

template <class T>
void Vector<T>::bubbleSort_fast(Rank lo, Rank hi)//向量的起泡排序
{
	while (lo < (hi = bubble_fast(lo, hi) ) ); //逐趟做扫描交换，直至全序
}

template <class T> 
Rank Vector<T>::max(Rank lo, Rank hi) //在[lo, hi]内找出最大者
{
	Rank mx = hi;
	while (lo < hi--)  //逆向扫描
	{
		if (_elem[hi] > _elem[mx]) //且严格比较
			mx = hi; //故能在max有多个时保证后者优先，进而保证selectionSort稳定
	}
	return mx;
}

template <class T>  //向量选择排序
void Vector<T>::selectionSort(Rank lo, Rank hi) //assert: 0 < lo <= hi <= size
{
	while (lo < --hi)
		swap(_elem[max(lo, hi)], _elem[hi]);//将[hi]与[lo, hi]中的最大者交换
}


