/*
		./   当前目录
		../  父级目录
		/    根目录
*/

// 栈的典型应用场合：逆序输出、递归嵌套、 延伸缓冲、栈式计算
//      相应的实例： 进制转换 、括号匹配 栈混洗、中缀表达式求值、
// 栈混洗 ：按照某种约定的规则，对栈中的元素进行重新排列（可以用任意三个元素的相对次序判断是否为栈混洗）

#pragma once
#include "../List/List.h"

template<typename T>  //以列表为基类，派生出栈模板类
class Stack : public List<T> //将列表的首/末端作为栈底/顶
{
public: //size()、empty()以及其它开放接口，均可直接沿用
	void push(T const& e){ insertAsLast(e); } //入栈：等效于将新元素作为列表的末元素插入
	T pop(){ return remove(last()); } //出栈：等效于删除列表的末元素
	T& top(){ return last()->data; } //取顶：直接返回列表的末元素

};




void convert(Stack<char>& s, __int64 n, int base) //进制转换
{
	static char digit[] = //新进制下的数位符号，可视base取值范围适当添加
	{ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	while (n > 0) //由低到高逐一计算出新进制下的各位数
	{
		s.push(digit[n%base]); //余数（对应的数位）入栈
		n /= base;  //n更新为其对base的除商
	}
}


//括号匹配
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