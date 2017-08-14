#define _CRT_SECURE_NO_WARNINGS

#include "List.h"
#include <time.h>
#include <iostream>
using namespace std;


int testID = 0; //测试编号

/******************************************************************************************
* 随机生成长度为n的列表（其中可能包含重复节点）
******************************************************************************************/
template <typename T> //元素类型
void randomList(List<T> & list, int n) { //在[0, 2n)中选择n个偶数，随机插入列表
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
* 测试列表
******************************************************************************************/
template <typename T> //元素类型
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
	//{ //逐一测试[0, 2n]中的所有可能
	//	ListNodePosi(T) p = La.find((T)i); printf("Looking for "); cout<<i; printf(": ");
	//	if (p) { printf(" found with"); cout<<p->data; }
	//	else printf(" not found");
	//	printf("\n");
	//} //正确的结构应该是大致（n+1次）失败、（n次）成功相间

	//printf("\n  ==== Test %2d. Sort\n", testID++); 
	//La.sort();
	//for (int i = 0; i < La.size(); i++) cout << La[i] << '\t'; printf("\n");

	//printf("\n  ==== Test %2d. SEARCH in\n", testID++); 
	//for (int i = 0; i <= testSize * 2; i++) 
	//{ //逐一测试[0, 2n]中的所有可能
	//	ListNodePosi(T) p = La.search((T)i); printf("Looking for "); cout<<(T)i; printf(": ");
	//	printf(" stopped at"); cout<<(p->data);
	//	if ((T)i == p->data) printf(" and found");
	//	printf("\n");
	//} //正确的结构应该是大致（n+1次）失败、（n次）成功相间

	//printf("\n  ==== Test %2d. Remove redundancy in\n", testID++); 
	//printf("%d node(s) removed\n", La.uniquify());   //有序列表去重
	//for (int i = 0; i < La.size(); i++) cout << La[i] << '\t'; printf("\n");

	printf("\n  ==== Test %2d. Remove redundancy in\n", testID++);
	printf("%d node(s) removed\n", La.deduplicate()); //无序去重
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