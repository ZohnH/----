#pragma  once

#include <stdio.h>
#include "ListNode.h"  ////�����б�ڵ���

template <typename T> 
class List   //�б�ģ����
{
private:
	int _size; ListNodePosi(T) header; ListNodePosi(T) trailer;  //��ģ��ͷ�ڱ���β�ڱ�

protected:
	void init(); //�б���ʱ�ĳ�ʼ��
	int clear(); //������нڵ�
	void copyNodes(ListNodePosi(T), int ); //�����б�����λ��p���n��
	void selectionSort(ListNodePosi(T), int); //�Դ�p��ʼ������n���ڵ�ѡ������
	void insertionSort(ListNodePosi(T), int); //�Դ�p��ʼ������n���ڵ��������
	void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int);//�鲢
	void mergeSort(ListNodePosi(T)&, int); //�Դ�p��ʼ������n���ڵ�鲢����

public:
// ���캯��
	List(){ init(); }   //Ĭ��
	List(List<T> const& L);  //���帴���б�L
	List(List<T> const& L, Rank r, int n);  //�����б�L���Ե�r�����n��
	List(ListNodePosi(T) p, int n);   //�����б�����λ��p���n��


// ��������
	~List(); //�ͷţ�����ͷ��β�ڱ����ڵģ����нڵ�
	

// ֻ�����ʽӿ�
	Rank size() const { return _size; } //��ģ
	bool empty() const { return _size <= 0; }
	T& operator[] (Rank r) const;  //���أ�֧��ѭ�ȷ��ʣ�Ч�ʵͣ�
	ListNodePosi(T) find(T const& e) const //�����б���ҳ�����e�Ľڵ�
	{ return find(e, _size, trailer); }
	ListNodePosi(T) find( T const& e, int n, ListNodePosi(T) p ) const; //�����������
	ListNodePosi(T) first() const { return header->succ; } //�׽ڵ�λ��
	ListNodePosi(T) last() const {return trailer->pred;} //ĩ�ڵ�λ��
	bool valid(ListNodePosi(T) p) //�ж�λ��p�Ƿ����Ϸ�
	{ return p && (trailer != p) && (header != p); }
	int disordered() const; //�ж��б��Ƿ������� //ͳ����������Ԫ�ضԵ�����
	ListNodePosi(T) search(T const& e) //�����б����
	{	return search(e, _size, trailer);	} //ǰ�����ҵ�������e�������
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;  //�����������
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);  //��p����n-1�������ѡ�������
	ListNodePosi(T) selectMax(){ return selectMax(header->succ, _size); }  //���������
	T remove(ListNodePosi(T) p); //ɾ���Ϸ�λ��p���Ľڵ�,���ر�ɾ���ڵ�
	

// ��д���ʽӿ�
	ListNodePosi(T) insertAsFirst(T const& e)   //��e�����׽ڵ����
	{ 
		_size++;
		return header->insertAsSucc(e); //ͷ�ڵ�ĺ�����׽ڵ�
	}

	ListNodePosi(T) insertAsLast(T const& e)   //e����ĩ�ڵ����
	{
		_size++;
		return trailer->insertAsPred(e);  //β�ڵ��ǰ����ĩ�ڵ�
	}

	ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e)  //��e����p��ǰ�����루Before��
	{  
		_size++; 
		return p->insertAsPred(e);
	}

	ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e)  //e����p�ĺ�̲���
	{
		_size++;
		return p->insertAsSucc(e);
	}


	int deduplicate(); //����ȥ��
	int uniquify(); //����ȥ��
	void sort(ListNodePosi(T), int);
	void sort(){ sort(first(), _size); }
};







template <typename T>
void List<T>::init()
{
	header = new ListNode<T>;   //����ͷ�ڱ��ڵ�
	trailer = new ListNode<T>;  //����β�ڱ��ڵ�
	header->succ = trailer;  header->pred = NULL;
	trailer->pred = header;  trailer->succ = NULL;
	_size = 0;
}


template <typename T>
int List<T>::clear()  //������нڵ�
{
	int oldSize = _size;
	while (0 < _size)   //����ɾ���׽ڵ㣬ֱ���б���
		remove(header->succ); //�׽ڵ�
	return oldSize;
}


template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)  //�����б�����λ��p���n��
{
	init();  //����ͷ��β�ڱ��ڵ㲢����ʼ��
	while (n--)
	{  //������p��n��������Ϊĩ�ڵ����
		insertAsLast(p->data);
		p = p->succ;
	}
}


template <typename T> //�����б�����λ��p���n�assert: pΪ�Ϸ�λ�ã���������n-1����̽ڵ㣩
List<T>::List(ListNodePosi(T) p, int n)
{
	copyNodes(p, n);
}

template <typename T> //���帴���б�L
List<T>::List(List<T> const& L)
{
	copyNodes(L.first(), L._size);
}

template <typename T>//����L���Ե�r�����n�assert: r+n <= L._size��
List<T>::List(List<T> const& L, Rank r, int n)
{
	copyNodes(L[r], n);
}

template <typename T> List<T>::~List() //�б�������
{
	clear(); delete header; delete trailer;
} //����б��ͷ�ͷ��β�ڱ��ڵ�

template <typename T>//�����±����������ͨ����ֱ�ӷ����б�ڵ㣨�䷽�㣬Ч�ʵͣ������ã�
T& List<T>::operator[](Rank r) const
{
	ListNodePosi(T) p = first(); //���׽ڵ����
	while (0 < r--){ p = p->succ; } //˳����r���ڵ㼴��
	return p->data;  //Ŀ��ڵ㣬������������Ԫ��
}

template <typename T>  //ͳ����������Ԫ�ضԵ�����
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


// �ڽڵ�p��������trailer����n�����棩ǰ���У��ҵ�����e�������
template <typename T>
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const
{
	while (0 < n--) //��0 <= n <= rank(p) < _size������p�������n��ǰ������������
	{
		if (e == (p = p->pred)->data) //����ȶԣ�ֱ�����л�ΧԽ��
			return p;
	}
	return NULL; //pԽ����߽���ζ�������ڲ���e������ʧ��
} //header�Ĵ���ʹ�ô����Ϊ���


template <typename T>
T List<T>::remove(ListNodePosi(T) p)  //ɾ���Ϸ�λ��p���Ľڵ�,���ر�ɾ���ڵ�.  o(1)
{
	T e = p->data;  //���ݴ�ɾ���ڵ����ֵ���ٶ�T���Ϳ�ֱ�Ӹ�ֵ��
	p->pred->succ = p->succ;
	p->succ->pred = p->pred;
	delete p;
	_size--;
	return e; //���ر��ݵ���ֵ
}

template <typename T>
int List<T>::deduplicate() //�޳������б��е��ظ��ڵ�
{
	if (_size < 2) return 0; //ƽ���б���Ȼ���ظ�
	int oldSize = _size; //��¼ԭ��ģ
	ListNodePosi(T) p = header;  Rank r = 0;
	while (trailer != (p = p->succ))
	{
		ListNodePosi(T) q = find(p->data, r, p); //��p��r�����棩ǰ���в�����ͬ��
		q ? remove(q) : r++; //����ȷ���ڣ���ɾ��֮�������ȼ�һ  ��������Ӧ��ɾ��q������p��ѭ��������Ҫ��p�ĺ��
	}                         //ɾ�������ڵ�ǰԪ��ǰ���ҵ����뵱ǰԪ����ȵĽڵ�
	return oldSize - _size;  //�б��ģ�仯��������ɾ��Ԫ������
}


template <typename T>
int List<T>::uniquify()  //�����޳��ظ�Ԫ�أ�Ч�ʸ���  o(n)
{
	if (_size < 2) return 0;
	int oldSize = _size;
	ListNodePosi(T) p = first(); ListNodePosi(T) q; //pΪ��������㣬qΪ����
	while (trailer != (q = p->succ)) //����������ڵĽڵ��(p, q)
	{
		if (p->data != q->data)
			p = q;  //�����죬��ת����һ����
		else
			remove(q);  //������ͬ����ɾ������
	}
	return oldSize - _size;
}


template <typename T>  //�������б��ڽڵ�p��������trailer����n�����棩ǰ���У��ҵ�������e�������
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const
{
	while (0 <= n--)   //0 <= n-- Ҫ�ҵ�ͷ�ڵ�Ϊֹ
	{
		if (((p = p->pred)->data) <= e) //�������� ����Ƚ� ֱ�����л�Խ��
			break;
	}
	return p;
}


template <typename T>  //����ʼ��λ��p��n��Ԫ����ѡ�������
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) max = p;  //������ݶ�Ϊ�׽ڵ�p
	for (ListNodePosi(T) cur = p; 1 < n; n--)
	{
		if (((cur = cur->succ)->data) >= max->data) //���ȥ����������������Ե� 
			max = cur;           //ѡ����С��p������Ԫ��
	}
	return max;
}

template <typename T> //�б��ѡ�������㷨������ʼ��λ��p��n��Ԫ������
void List<T>::selectionSort(ListNodePosi(T) p, int n)
{
	ListNodePosi(T) head = p->pred;
	ListNodePosi(T) tail = p;
	for (int i = 0; i < n; i++)
		tail = tail->succ;   //����������Ϊ(head, tail)
	while (1 < n)  //�����ٻ�ʣ�����ڵ�֮ǰ���ڴ�����������
	{
		ListNodePosi(T) max = selectMax(head->succ, n);
		insertBefore(tail, remove(max));
		tail = tail->pred;
		n--;
	}
}

template<typename T>  //�б�Ĳ��������㷨������ʼ��λ��p��n��Ԫ������
void List<T>::insertionSort(ListNodePosi(T) p, int n)
{
	for (int r = 0; r < n; r++) //��һΪ���ڵ�
	{
		insertAfter(search(p->data, r, p), p->data); //�����ʵ���λ�ò�����
		p = p->succ;   //ת����һ�ڵ�(��Ϊ��β�ڵ��ڱ��������ⲽ��ȫ��
		remove(p->pred); //ɾ��p����
	}
}


template<typename T>//�����б�Ĺ鲢����ǰ�б�����p���n��Ԫ�أ����б�L����q���m��Ԫ�ع鲢
void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m)
{ //��Ϊ����p���޸� ��������Ϊp������
	// assert:  this.valid(p) && rank(p) + n <= size && this.sorted(p, n)
	//          L.valid(q) && rank(q) + m <= L._size && L.sorted(q, m)
	// ע�⣺�ڹ鲢����֮��ĳ��ϣ��п��� this == L && rank(p) + n = rank(q)
	ListNodePosi(T) pp = p->pred;  //����ǰ����������header�����Ա㷵��ǰ ...
	while (0 < m) //��q��δ�Ƴ�����֮ǰ
	{
		if ((0 < n) && (p->data <= q->data)) //��p������������v(p) <= v(q)����
		{
			if (q == (p = p->succ)) //p����ϲ����б����滻Ϊ��ֱ�Ӻ��
				break; n--;
		}
		else //��p�ѳ����ҽ��v(q) < v(p)����
		{
			insertBefore(p, L.remove((q = q->succ)->pred));
			m--; //��qת����p֮ǰ
		}
	}
	p = pp->succ; //ȷ���鲢������ģ��£����
}


template<typename T> //�б�Ĺ鲢�����㷨������ʼ��λ��p��n��Ԫ������
void List<T>::mergeSort(ListNodePosi(T)& p, int n)
{   //valid(p) && rank(p) + n <= size
	if (n < 2) return; //��������Χ���㹻С����ֱ�ӷ��أ�����...
	int m = n >> 1; //���е�Ϊ�� //�����б�
	ListNodePosi(T) q = p;
	for (int i = 0; i < m; i++)
		q = q->succ;
	mergeSort(p, m);      //��ǰ�������б�ֱ�����
	mergeSort(q, n - m);
	merge(p, m, *this, q, n - m);  //�鲢
}

template <typename T> 
void List<T>::sort(ListNodePosi(T) p, int n) 
{ //�б���������
	switch (rand() % 3) 
	{ //���ѡȡ�����㷨���ɸ��ݾ���������ص����ѡȡ������
	case 1:  insertionSort(p, n); break; //��������
	case 2:  selectionSort(p, n); break; //ѡ������
	default: mergeSort(p, n); break; //�鲢����
	}

//	insertionSort(p, n);
}

