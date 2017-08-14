#pragma  once 

#include <stdio.h>
#include "PQ_ComplHeap_marco.h"
#include "PQ.h"  //�������ȼ�����ADTʵ�ֵ�
#include "../vector/vector.h" //�������ؼ̳л��ƣ���������



template<class T>
class PQ_ComplHeap : public PQ<T>, public Vector<T>//��ȫ�����
{
protected:
	Rank percolateDown(Rank n, Rank i); //����
	Rank percolateUp(Rank i);  //����
	void heapify(Rank n); //Floyd�����㷨
public: 
	PQ_ComplHeap() {}; //Ĭ�Ϲ���
	PQ_ComplHeap(T *A, Rank n){ copyFrom(A, 0, n); heapify(n); } //Ĭ�Ϲ���
	void insert(T); //���ձȽ���ȷ�������ȼ����򣬲������
	T getMax(); //��ȡ���ȼ���ߵĴ���
	T delMax(); //ɾ�����ȼ���ߵĴ���
	void heapSort(Rank lo, Rank hi); 
	void heapSort(){ heapSort(0, _size); }  //������
};



template <class T>
Rank PQ_ComplHeap<T>::percolateUp(Rank i)//�������еĵ�i������ʵʩ���˲�����i < _size
{
	while (ParentValid(i)) //ֻҪi�и��ף���δ�ִ�Ѷ�������
	{
		Rank j = Parent(i); //��i֮������j
		if (_elem[i] < _elem[j])
			break;   //һ����ǰ���Ӳ������������������
		swap(_elem[i], _elem[j]); //���򣬸��ӽ���λ�ã�������������һ��
		i = j;
	}
	return i; //�����������յִ��λ��
}

template <class T>
void PQ_ComplHeap<T>::insert(T e) //������������ȫ�������
{
	Vector<T>::insert(e);     //���Ƚ��´�����������ĩβ
	percolateUp(_size - 1);  //�ٶԸô���ʵʩ���˵���
}

///*�Ľ��Ĳ����㷨�������²���Ĵ��������ȱ��ݣ�ÿ������б�Ҫ����������ֻ���������ĸ��ڵ㡣
//  ����ڲ���swap()���������ڳ�ϵ���������������Ľ���һ��swap��ζ��3�θ�ֵ����3*logN�Ľ�ΪlogN+2 */
//template <class T>
//void PQ_ComplHeap<T>::insert(T e) //������������ȫ�������
//{
//	Vector<T>::insert(e);
//	Rank i = _size - 1;
//	T last = _elem[i];
//	while (ParentValid(i))
//	{
//		Rank j = Parent(i); //��i֮������j
//		if (_elem[i] < _elem[j])
//			break;
//		_elem[i] = _elem[j];   i = j;
//	}
//	_elem[i] = last;
//	return i;
//}


template <class T>
Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i)//������ǰn�������еĵ�i��ʵʩ���ˣ�i < n
{
	Rank j; //i���䣨���������������У�����Ϊ����
	while (i != ( j = ProperParent(_elem, n, i) ) ) //ֻҪi��j����
	{
		swap(_elem[i], _elem[j]); 
		i = j;                    //���߻�λ�������������½����i
	}
	return i; //�������˵ִ��λ�ã���i��j��
}

template <class T>
void PQ_ComplHeap<T>::heapify(Rank n) //Floyd�����㷨,O(n)ʱ��
{
	for (int i = LastInternal(n); InHeap(n, i); i--)//�Ե׶��ϣ�����
	{
		percolateDown(n, i);//���˸��ڲ��ڵ�
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
T PQ_ComplHeap<T>::getMax() //ȡ���ȼ���ߵĴ���
{
	return _elem[0];
}

template <class T>
T PQ_ComplHeap<T>::delMax()//ɾ���ǿ���ȫ����������ȼ���ߵĴ���
{
	T maxElem = _elem[0];
	_elem[0] = _elem[--_size]; //ժ���Ѷ����״���������֮��ĩ����
	percolateDown(_size, 0);  //���¶Ѷ�ʵʩ����
	return maxElem;  //���ش�ǰ���ݵ�������
}


//template <class T>
//void PQ_ComplHeap<T>::heapSort()
//{
//	while (!empty()) 
//		_elem[--_size] = delMax(); 
//	//����������delMax�������Ѿ�����_size���˸��ģ�����ٸ��ı�Ȼ��������������
//  //��ȷ�ķ�����������  ###############
//}

template <class T>
void PQ_ComplHeap<T>::heapSort(Rank lo, Rank hi)
{
	//while (lo < hi--)
	//	_elem[hi] = delMax(); 
	while (!empty())
		_elem[--hi] = delMax();
} 