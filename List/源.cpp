#define _CRT_SECURE_NO_WARNINGS

#include "List.h"
#include <time.h>
#include <iostream>
using namespace std;


int testID = 0; //���Ա��

/******************************************************************************************
* ������ɳ���Ϊn���б����п��ܰ����ظ��ڵ㣩
******************************************************************************************/
template <typename T> //Ԫ������
void randomList(List<T> & list, int n) { //��[0, 2n)��ѡ��n��ż������������б�
	ListNodePosi(T) p =
		(rand() % 2) ?
		list.insertAsLast(rand() % (T)n * 2) :
		list.insertAsFirst(rand() % (T)n * 2);
	for (int i = 1; i < n; i++)
		p = rand() % 2 ?
		list.insertBefore(p, rand() % (T)n * 2) :
		list.insertAfter(p, rand() % (T)n * 2);
}

/******************************************************************************************
* �����б�
******************************************************************************************/
template <typename T> //Ԫ������
void   testList(int testSize)
{
	printf("\n  ==== Test %2d. Generate two lists each of size %d by random insertions\n", testID++, testSize);
	List<T> La; randomList(La, testSize);
	List<T> Lb; randomList(Lb, testSize);
	for (int i = 0; i < La.size(); i++) cout<<La[i]<<'\t'; printf("\n");
	for (int i = 0; i < Lb.size(); i++) cout<<Lb[i]<<'\t'; printf("\n");

	//printf("\n  ==== Test %2d. Concatenation\n", testID++);
	//while (0 < Lb.size()) La.insertAsLast(Lb.remove(Lb.first()));
	//for (int i = 0; i < La.size(); i++) cout << La[i] << '\t'; printf("\n");
	//for (int i = 0; i < Lb.size(); i++) cout << Lb[i] << '\t'; printf("\n");

	//printf("\n  ==== Test %2d. Lowpass (with high complexity) on\n", testID++);
	//int i = La.size(); while (0 < --i) { La[i - 1] += La[i]; La[i - 1] >>= 1; }
	//for (int i = 0; i < La.size(); i++) cout << La[i] << '\t'; printf("\n");

	//printf("\n  ==== Test %2d. Copy\n", testID++);
	//List<T> Ld(La);
	//for (int i = 0; i < Ld.size(); i++) cout << Ld[i] << '\t'; printf("\n");

	//printf("\n  ==== Test %2d. FIND in\n", testID++); 
	//for (int i = 0; i <= testSize * 2; i++)
	//{ //��һ����[0, 2n]�е����п���
	//	ListNodePosi(T) p = La.find((T)i); printf("Looking for "); cout<<i; printf(": ");
	//	if (p) { printf(" found with"); cout<<p->data; }
	//	else printf(" not found");
	//	printf("\n");
	//} //��ȷ�ĽṹӦ���Ǵ��£�n+1�Σ�ʧ�ܡ���n�Σ��ɹ����

	//printf("\n  ==== Test %2d. Sort\n", testID++); 
	//La.sort();
	//for (int i = 0; i < La.size(); i++) cout << La[i] << '\t'; printf("\n");

	//printf("\n  ==== Test %2d. SEARCH in\n", testID++); 
	//for (int i = 0; i <= testSize * 2; i++) 
	//{ //��һ����[0, 2n]�е����п���
	//	ListNodePosi(T) p = La.search((T)i); printf("Looking for "); cout<<(T)i; printf(": ");
	//	printf(" stopped at"); cout<<(p->data);
	//	if ((T)i == p->data) printf(" and found");
	//	printf("\n");
	//} //��ȷ�ĽṹӦ���Ǵ��£�n+1�Σ�ʧ�ܡ���n�Σ��ɹ����

	//printf("\n  ==== Test %2d. Remove redundancy in\n", testID++); 
	//printf("%d node(s) removed\n", La.uniquify());   //�����б�ȥ��
	//for (int i = 0; i < La.size(); i++) cout << La[i] << '\t'; printf("\n");

	printf("\n  ==== Test %2d. Remove redundancy in\n", testID++);
	printf("%d node(s) removed\n", La.deduplicate()); //����ȥ��
	for (int i = 0; i < La.size(); i++) cout << La[i] << '\t'; printf("\n");


	return;
}


int main()
{
	int testSize;
	printf("Please input the list size: ");
	scanf("%d", &testSize);
	srand((unsigned int)time(NULL));
	testList<int> (testSize);

	return 0;
}