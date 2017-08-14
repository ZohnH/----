#pragma  once 

#include <stdio.h>
#include "PQ_ComplHeap_marco.h"
#include "PQ.h"  //按照优先级队列ADT实现的
#include "../vector/vector.h" //借助多重继承机制，基于向量



template<class T>
class PQ_ComplHeap : public PQ<T>, public Vector<T>//完全二叉堆
{
protected:
	Rank percolateDown(Rank n, Rank i); //下滤
	Rank percolateUp(Rank i);  //上滤
	void heapify(Rank n); //Floyd建堆算法
public: 
	PQ_ComplHeap() {}; //默认构造
	PQ_ComplHeap(T *A, Rank n){ copyFrom(A, 0, n); heapify(n); } //默认构造
	void insert(T); //按照比较器确定的优先级次序，插入词条
	T getMax(); //读取优先级最高的词条
	T delMax(); //删除优先级最高的词条
	void heapSort(Rank lo, Rank hi); 
	void heapSort(){ heapSort(0, _size); }  //堆排序
};



template <class T>
Rank PQ_ComplHeap<T>::percolateUp(Rank i)//对向量中的第i个词条实施上滤操作，i < _size
{
	while (ParentValid(i)) //只要i有父亲（尚未抵达堆顶），则
	{
		Rank j = Parent(i); //将i之父记作j
		if (_elem[i] < _elem[j])
			break;   //一旦当前父子不再逆序，上滤旋即完成
		swap(_elem[i], _elem[j]); //否则，父子交换位置，并继续考查上一层
		i = j;
	}
	return i; //返回上滤最终抵达的位置
}

template <class T>
void PQ_ComplHeap<T>::insert(T e) //将词条插入完全二叉堆中
{
	Vector<T>::insert(e);     //首先将新词条接至向量末尾
	percolateUp(_size - 1);  //再对该词条实施上滤调整
}

///*改进的插入算法：对于新插入的词条，首先备份，每次如果有必要交换，我们只是下移它的父节点。
//  相比于采用swap()函数，这在常系数的意义上有所改进。一次swap意味着3次赋值。从3*logN改进为logN+2 */
//template <class T>
//void PQ_ComplHeap<T>::insert(T e) //将词条插入完全二叉堆中
//{
//	Vector<T>::insert(e);
//	Rank i = _size - 1;
//	T last = _elem[i];
//	while (ParentValid(i))
//	{
//		Rank j = Parent(i); //将i之父记作j
//		if (_elem[i] < _elem[j])
//			break;
//		_elem[i] = _elem[j];   i = j;
//	}
//	_elem[i] = last;
//	return i;
//}


template <class T>
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i)//对向量前n个词条中的第i个实施下滤，i < n
{
	Rank j; //i及其（至多两个）孩子中，最大的为父者
	while (i != ( j = ProperParent(_elem, n, i) ) ) //只要i非j，则
	{
		swap(_elem[i], _elem[j]); 
		i = j;                    //二者换位，并继续考查下降后的i
	}
	return i; //返回下滤抵达的位置（亦i亦j）
}

template <class T>
void PQ_ComplHeap<T>::heapify(Rank n) //Floyd建堆算法,O(n)时间
{
	for (int i = LastInternal(n); InHeap(n, i); i--)//自底而上，依次
	{
		percolateDown(n, i);//下滤各内部节点
/*DSA*/ for (int k = 0; k < n; k++)
		{
			int kk = k;
			while (i < kk)
				kk = (kk - 1) >> 1;
			i == kk ? printf("%d  ", _elem[k]) : printf("   ");
		}
		printf("\n");
	}
}

template <class T>
T PQ_ComplHeap<T>::getMax() //取优先级最高的词条
{
	return _elem[0];
}

template <class T>
T PQ_ComplHeap<T>::delMax()//删除非空完全二叉堆中优先级最高的词条
{
	T maxElem = _elem[0];
	_elem[0] = _elem[--_size]; //摘除堆顶（首词条），代之以末词条
	percolateDown(_size, 0);  //对新堆顶实施下滤
	return maxElem;  //返回此前备份的最大词条
}


//template <class T>
//void PQ_ComplHeap<T>::heapSort()
//{
//	while (!empty()) 
//		_elem[--_size] = delMax(); 
//	//上面语句错误：delMax函数中已经对与_size有了更改，这儿再更改必然出错！！！！！！
//  //正确的方法是运用秩  ###############
//}

template <class T>
void PQ_ComplHeap<T>::heapSort(Rank lo, Rank hi)
{
	//while (lo < hi--)
	//	_elem[hi] = delMax(); 
	while (!empty())
		_elem[--hi] = delMax();
} 