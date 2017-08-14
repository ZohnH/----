/*
		./   ��ǰĿ¼
		../  ����Ŀ¼
		/    ��Ŀ¼
*/

// ջ�ĵ���Ӧ�ó��ϣ�����������ݹ�Ƕ�ס� ���컺�塢ջʽ����
//      ��Ӧ��ʵ���� ����ת�� ������ƥ�� ջ��ϴ����׺���ʽ��ֵ��
// ջ��ϴ ������ĳ��Լ���Ĺ��򣬶�ջ�е�Ԫ�ؽ����������У���������������Ԫ�ص���Դ����ж��Ƿ�Ϊջ��ϴ��

#pragma once
#include "../List/List.h"

template<typename T>  //���б�Ϊ���࣬������ջģ����
class Stack : public List<T> //���б����/ĩ����Ϊջ��/��
{
public: //size()��empty()�Լ��������Žӿڣ�����ֱ������
	void push(T const& e){ insertAsLast(e); } //��ջ����Ч�ڽ���Ԫ����Ϊ�б��ĩԪ�ز���
	T pop(){ return remove(last()); } //��ջ����Ч��ɾ���б��ĩԪ��
	T& top(){ return last()->data; } //ȡ����ֱ�ӷ����б��ĩԪ��

};




void convert(Stack<char>& s, __int64 n, int base) //����ת��
{
	static char digit[] = //�½����µ���λ���ţ�����baseȡֵ��Χ�ʵ����
	{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	while (n > 0) //�ɵ͵�����һ������½����µĸ�λ��
	{
		s.push(digit[n%base]); //��������Ӧ����λ����ջ
		n /= base;  //n����Ϊ���base�ĳ���
	}
}


//����ƥ��
bool paren(const char exp[], int lo, int hi)
{  //exp[lo,hi)
	Stack<char> s;
	for (int i = lo; i < hi; ++i)
	{
		if (exp[i] == '(' || exp[i] == '[' || exp[i] == '{')
			s.push(exp[i]);
		else if (!s.empty() )  
		{
			if (exp[i] == ')' && s.top() == '(') s.pop();
			if (exp[i] == ']' && s.top() == '[') s.pop();
			if (exp[i] == '}' && s.top() == '{') s.pop();
		}
		else
			return false;
	}
	return s.empty();
}