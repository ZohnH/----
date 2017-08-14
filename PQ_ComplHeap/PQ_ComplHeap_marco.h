//��غ궨��

#pragma  once 

template <typename T> static bool lt(T* a, T* b) { return lt(*a, *b); } //less than
template <typename T> static bool lt(T& a, T& b) { return a < b; } //less than


#define InHeap(n,i)  ( ( (-1) < (i) ) && ( (i) < (n) ) ) //�ж�PQ[i]�Ƿ�Ϸ�
#define Parent(i)  ( (i-1) >> 1 ) //PQ[i]�ĸ��ڵ㣨floor((i-1)/2)��i����������
#define LastInternal(n)  Parent(n-1) //���һ���ڲ��ڵ㣨��ĩ�ڵ�ĸ��ף�
#define LChild(i)  ( 1 + ( (i) << 1 ) )  //PQ[i]������
#define RChild(i)  ( ( 1 + (i) ) << 1 )  //PQ[i]���Һ���
#define ParentValid(i)  (0 < i) //�ж�PQ[i]�Ƿ��и���
#define LChildValid(n, i)  InHeap(n, LChild(i)) //�ж�PQ[i]�Ƿ���һ�����󣩺���
#define RChildValid(n, i)  InHeap(n, RChild(i)) //�ж�PQ[i]�Ƿ�����������(���Һ��Ӿ�һ��������)
#define Bigger(PQ, i, j) ( (PQ[i]) < (PQ[j]) ) ? j : i   //ȡ���ߣ�ע�⣺�����ʱǰ�����ȣ�

/*���ӣ����ࣩ�����еĴ���*/  //���ʱ���ڵ����ȣ���˿ɱ��ⲻ��Ҫ�Ľ���
#define ProperParent(PQ, n, i) \
	( RChildValid(n, i) ? Bigger( PQ, Bigger( PQ, i, LChild(i) ), RChild(i) ) : \
	 ( LChildValid(n, i) ? Bigger( PQ, i, LChild(i)) : i )  )
