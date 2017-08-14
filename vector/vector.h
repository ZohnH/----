#pragma once 

#include "../fibonacci/fib.h"  //����Fib������  ����Fibonacci�����㷨

typedef int Rank;  //��
#define DEFAULT_CAPACITY  3  //Ĭ�ϵĳ�ʼ������ʵ��Ӧ���п�����Ϊ����


template<class T>
class Vector  //����ģ����
{
protected:
	Rank _size;  int _capacity;  T* _elem; //��ģ��������������
	void copyFrom(T const* A, Rank lo, Rank hi); //������������A[lo, hi)
	void expand(); //�ռ䲻��ʱ����
	void shrink(); //װ�����ӹ�Сʱѹ��

public:
	Rank bubble_fast(Rank lo, Rank hi); //ɨ�轻��
	void bubbleSort_fast(Rank lo, Rank hi); //���������㷨
	Rank max(Rank lo, Rank hi); //ѡȡ���Ԫ��
	void selectionSort(Rank lo, Rank hi); //ѡ�������㷨


public:
//���캯��
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //����Ϊc����ģΪs������Ԫ�س�ʼΪv
		{ _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);} //s<=c
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //�������帴��
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //����
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //�������帴��
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //����
// ��������
	~Vector(){ delete[] _elem; }  //�ͷ��ڲ��ռ�
	
// ֻ�����ʽӿ�
	Rank size() const { return _size; } //��ģ
	bool empty() const { return !_size; } //�п�


// ��д���ʽӿ�
	T& operator[] (Rank r) const; //�����±������������������������ʽ���ø�Ԫ��
	Vector<T> & operator= (Vector<T> const&); //���ظ�ֵ���������Ա�ֱ�ӿ�¡����
	Rank insert(Rank r, T const& e); //����Ԫ��
	Rank insert(T const& e) { return insert(_size, e); } //Ĭ����ΪĩԪ�ز���
	T remove(Rank r); //ɾ����Ϊr��Ԫ��
	int remove(Rank lo, Rank hi); //ɾ����������[lo, hi)֮�ڵ�Ԫ��
	Rank search(T const& e) const //���������������
	{ return (0 >= _size) ? -1 : search(e, 0, _size); }
	Rank search(T const& e, Rank lo, Rank hi) const;  //���������������
};




template <class T>
void swap(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

template<class T>    //����������A[lo, hi)Ϊ������������
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)
{
	_elem = new T[_capacity = 2 * (hi - lo)]; //����ռ�
	_size = 0;   //��ģ����
	while (lo < hi)
		_elem[_size++] = A[lo++];  //������_elem[0, hi - lo)
}

template<class T>
void Vector<T>::expand()   //�����ռ䲻��ʱ����
{
	if (_size < _capacity) //��δ��Աʱ����������
		return;
	if (_capacity < DEFAULT_CAPACITY)
		_capacity = DEFAULT_CAPACITY;  //��������С����
	T* oldElem = _elem;
	_elem = new T[_capacity <<= 1];  //�����ӱ�
	for (int i = 0; i < _size; ++i)
		_elem[i] = oldElem[i]; //����ԭ�������ݣ�TΪ�������ͣ��������ظ�ֵ������'='��
	delete [] oldElem; //�ͷ�ԭ�ռ�
}

template<class T>
void Vector<T>::shrink()  //װ�����ӹ�Сʱѹ��������ռ�ռ�
{
	if (_capacity < DEFAULT_CAPACITY << 1)
		return; //����������DEFAULT_CAPACITY����
	if (_size << 2 > _capacity) 
		return; //��25%Ϊ��
	T* oldElem = _elem; 
	_elem = new T[_capacity >>= 1]; //��������
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i]; //����ԭ��������
	delete[] oldElem; //�ͷ�ԭ�ռ�
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V)
{
	if (_elem) delete[] _elem; //�ͷ�ԭ������
	copyFrom(V._elem, 0, V.size()); //���帴��
	return *this;  //���ص�ǰ��������ã��Ա���ʽ��ֵ
}

template <class T>  //�����±������������������������ʽ���ø�Ԫ��
T& Vector<T>::operator[](Rank r) const
{
	return _elem[r];
}

template <class T>    //��e��Ϊ��ΪrԪ�ز���
Rank Vector<T>::insert(Rank r, T const& e)
{
	expand();   //���б�Ҫ������
	for (int i = _size; i > r; i--)
		_elem[i] = _elem[i - 1]; //�Ժ���ǰ�����Ԫ��˳�κ���һ����Ԫ
	_elem[r] = e;
	_size++;        //������Ԫ�ز���������
	return r;   //������
}

template <class T>
int Vector<T>::remove(Rank lo, Rank hi) //ɾ������[lo, hi)
{
	if (lo == hi)
		return 0;  //����Ч�ʿ��ǣ����������˻����������remove(0, 0)
	while (hi < _size)
		_elem[lo++] = _elem[hi++];  //[hi, _size)˳��ǰ��hi - lo����Ԫ
	_size = lo; //���¹�ģ(lo�Ѹ���)��ֱ�Ӷ���β��[lo, _size = hi)����
	shrink(); //���б�Ҫ��������
	return hi - lo;
}

template <class T>
T Vector<T>::remove(Rank r) //ɾ����������Ϊr��Ԫ�أ�0 <= r < size
{
	T e = _elem[r];
	remove(r, r + 1);
	return e;
}


//// ���ֲ����㷨���汾A��������������������[lo, hi)�ڲ���Ԫ��e��0 <= lo <= hi <= _size
//template <typename T> 
//static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) 
//{
//	while (lo < hi)  //ÿ����������Ҫ�����αȽ��жϣ���������֧
//	{
//		Rank mi = (lo + hi) >> 1; //���е�Ϊ���
//		if (e < A[mi]) 
//			hi = mi; //����ǰ���[lo, mi)��������
//		else if (A[mi] < e) 
//			lo = mi + 1; //�������(mi, hi)��������
//		else            
//			return mi; //��mi������
//	} //�ɹ����ҿ�����ǰ��ֹ
//	return -1; //����ʧ��
//}//�ж������Ԫ��ʱ�����ܱ�֤����������ߣ�����ʧ��ʱ���򵥵ط���-1��������ָʾʧ�ܵ�λ��

// ���ֲ����㷨���汾C��������������������[lo, hi)�ڲ���Ԫ��e��0 <= lo <= hi <= _size
template <class T>
static Rank binSearch(T* A, T const& e, Rank lo, Rank hi)
{
	while (lo < hi)   //ÿ������������һ�αȽ��жϣ���������֧
	{ 
		Rank mi = (lo + hi) >> 1;  //���е�Ϊ���
		// ע����� e<A[mi]  �Լ��жϺ��lo�Ĳ�����lo=mi+1
		(e < A[mi]) ? hi = mi : lo = mi+1; //���ȽϺ�ȷ������[lo, mi)��(mi, hi)
	}
	return --lo; //ѭ������ʱ��loΪ����e��Ԫ�ص���С�ȣ���lo-1��������e��Ԫ�ص������
}  //�ж������Ԫ��ʱ�����ܱ�֤����������ߣ�����ʧ��ʱ���ܹ�����ʧ�ܵ�λ��

// Fibonacci�����㷨������������������[lo, hi)�ڲ���Ԫ��e��0 <= lo <= hi <= _size
template <class T> 
static Rank fibSearch(T* A, T const& e, Rank lo, Rank hi)
{
	Fib fib(hi - lo);
	while (lo < hi)
	{
		while (hi - lo < fib.get())
			fib.prev();   //ͨ����ǰ˳����ң���̯O(1)��
		Rank mi = lo + fib.get() - 1; //ȷ������Fib(k) - 1�����
		(e < A[mi]) ? hi = mi : lo = mi + 1;
	} //�ɹ����Ҳ�����ǰ��ֹ
	return --lo; //ѭ������ʱ��loΪ����e��Ԫ�ص���С�ȣ���lo - 1��������e��Ԫ�ص������
} //�ж������Ԫ��ʱ�����ܱ�֤������������ߣ�����ʧ��ʱ���ܹ�����ʧ�ܵ�λ��

template <class T> //����������������[lo, hi)�ڣ�ȷ��������e�����һ���ڵ����
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{  //����50%�ĸ������ʹ�ö��ֲ��һ�Fibonacci����
	return (rand() % 2) ? binSearch(_elem,e,lo,hi) : fibSearch(_elem,e,lo,hi);
}


//�Ľ���ð������ͨ����¼���Ҳ�����Ե�λ�ã�ʹ���㷨�ڳ�ϵ�������������Ľ�
template <class T> 
Rank Vector<T>::bubble_fast(Rank lo, Rank hi)  //һ��ɨ�轻��
{
	Rank last = lo;  //���Ҳ������Գ�ʼ��Ϊ[lo - 1, lo]
	while (++lo < hi) //�������ң���һ����������Ԫ��
	{
		if (_elem[lo - 1] > _elem[lo])
		{
			last = lo; //��������������Ҳ������λ�ü�¼����
			swap(_elem[lo - 1], _elem[lo]);  //ͨ������ʹ�ֲ�����
		}	
	} 
	return last;//�������Ҳ�������λ��
}

template <class T>
void Vector<T>::bubbleSort_fast(Rank lo, Rank hi)//��������������
{
	while (lo < (hi = bubble_fast(lo, hi) ) ); //������ɨ�轻����ֱ��ȫ��
}

template <class T> 
Rank Vector<T>::max(Rank lo, Rank hi) //��[lo, hi]���ҳ������
{
	Rank mx = hi;
	while (lo < hi--)  //����ɨ��
	{
		if (_elem[hi] > _elem[mx]) //���ϸ�Ƚ�
			mx = hi; //������max�ж��ʱ��֤�������ȣ�������֤selectionSort�ȶ�
	}
	return mx;
}

template <class T>  //����ѡ������
void Vector<T>::selectionSort(Rank lo, Rank hi) //assert: 0 < lo <= hi <= size
{
	while (lo < --hi)
		swap(_elem[max(lo, hi)], _elem[hi]);//��[hi]��[lo, hi]�е�����߽���
}


