//RPN：：逆波兰表达式

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "../stack/stack@vector.h"
#include "priority.h"

//声明
__int64 facI(int n);  //factorial阶乘
float calcu(float a, char op, float b); //执行二元运算
float calcu(char op, float b); //执行一元运算
void displayProgress(char*, char*, Stack<float>&, Stack<char>&, char*);
void readNumber(char*& p, Stack<float>& stk); //将起始于p的子串解析为数值，并存入操作数栈
Operator optr2rank(char);  //由运算符转译出编号
char orderBetween(char, char); //比较两个运算符之间的优先级
void append(char*&, float); //将操作数接至RPN末尾
void append(char*&, char); //重载  //将运算符接至RPN末尾
float evaluate(char* expr);  //中缀表达式计算
float evaluate(char* S, char*& RPN); //对表达式S求值，并转换为逆波兰式RPN
char* removeSpace(char* s);  //剔除s[]中的白空格  公式输入的时候可能会有空格



//方法实现
__int64 facI(int n)
{
	__int64 f = 1;
	while (n > 1)	f *= n--;
	return f;	
}

float calcu(float a, char op, float b)
{
	switch (op)
	{
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a * b;
	case '/': if (0 == b) exit(-1); return a / b; //注意：如此判浮点数为零可能不安全
	case '^': return pow(a, b);
	default: exit(-1);
	}
}

float calcu(char op, float b)
{
	switch (op)
	{
	case '!': return (float)facI((int)b);//目前仅有阶乘，可照此方式添加
	default: exit(-1);
	}
}

void readNumber(char*& p, Stack<float>& stk)
{ //将起始于p的子串解析为数值，并存入操作数栈
	stk.push((float)(*p - '0')); //当前数位对应的数值进栈
	while (isdigit(*(++p)))           //只要后续还有紧邻的数字（即多位整数的情况），则
		stk.push(stk.pop() * 10 + (*p - '0')); //弹出原操作数并追加新数位后，新数值重新入栈
	if ('.' != *p) return; //此后非小数点，则意味着当前操作数解析完成
	float fraction = 1;   //否则，意味着还有小数部分
	while (isdigit(*(++p)))  //逐位加入
		stk.push(stk.pop() + (*p - '0')*(fraction /= 10)); //小数部分
}

Operator optr2rank(char op)
{ //由运算符转译出编号
	switch (op)
	{
	case '+': return ADD; //加
	case '-': return SUB; //减
	case '*': return MUL; //乘
	case '/': return DIV; //除
	case '^': return POW; //乘方
	case '!': return FAC; //阶乘
	case '(': return L_P; //左括号
	case ')': return R_P; //右括号
	case '\0': return EOE; //起始符与终止符
	default: exit(-1); //未知运算符
	}
}

char orderBetween(char op1, char op2)
{ //比较两个运算符之间的优先级
	return pri[optr2rank(op1)][optr2rank(op2)];
}

void append(char*& rpn, float opnd) 
{ //将操作数接至RPN末尾
	int n = strlen(rpn); //RPN当前长度（以'\0'结尾，长度n + 1）
	char buf[64];
	if (opnd != (float)(int)opnd) sprintf(buf, "%.2f \0", opnd); //浮点格式，或
	else                          sprintf(buf, "%d \0", (int)opnd); //整数格式
	rpn = (char*)realloc(rpn, sizeof (char)* (n + strlen(buf) + 1)); //扩展空间
	strcat(rpn, buf); //RPN加长
}

void append(char*& rpn, char optr)
{ //将运算符接至RPN末尾
	int n = strlen(rpn); //RPN当前长度（以'\0'结尾，长度n + 1）
	rpn = (char*)realloc(rpn, sizeof (char)* (n + 3)); //扩展空间
	sprintf(rpn + n, "%c ", optr); rpn[n + 2] = '\0'; //接入指定的运算符
}

//char* removeSpace(char* s)
//{  //剔除s[]中的白空格
//	
//
//}


float evaluate(char* S)
{ //对表达式S求值
	Stack<float> opnd; Stack<char> optr; //运算数栈、运算符栈 /*DSA*/任何时刻，其中每对相邻元素之间均大小一致
	optr.push('\0'); //尾哨兵'\0'也作为头哨兵首先入栈
	while (!optr.empty()) //在运算符栈非空之前，逐个处理表达式中各字符
	{
		if (isdigit(*S)) //若当前字符为操作数，则
		{ readNumber(S, opnd); } //读入操作数
		else 
		{     //若当前字符为运算符，则
			switch (orderBetween(optr.top(), *S)) //视其与栈顶运算符之间优先级高低分别处理
			{
			case '<':  //栈顶运算符优先级更低时
				optr.push(*S); S++; break; //计算推迟，当前运算符进栈,读取下一个字符
			case '=':  //优先级相等（当前运算符为右括号或者尾部哨兵'\0'）时
				optr.pop(); S++; break; //脱括号并接收下一个字符
			case '>':  //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
			{
					char op = optr.pop();
					if ('!' == op)  //若属于一元运算符
					{
						float pOpnd = opnd.pop();
						opnd.push(calcu(op, pOpnd));
					}
					else  //对于其它（二元）运算符
					{
						float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //取出后、前操作数 顺序要对
						opnd.push(calcu(pOpnd1, op, pOpnd2));
					}
					break;
			}
			default:  //逢语法错误，提示并退出
				printf("WRONG!!\n");
				exit(-1);  /* 这儿如果上面的case'>'的语句不加{} 会报错：default标签跳过“op”初始化操作 */
			}
		}
	}
	return opnd.pop();
}


float evaluate(char* S, char*& RPN) { //对（已剔除白空格的）表达式S求值，并转换为逆波兰式RPN
	Stack<float> opnd; Stack<char> optr; //运算数栈、运算符栈 /*DSA*/任何时刻，其中每对相邻元素之间均大小一致
	/*DSA*/ char* expr = S;
	optr.push('\0'); //尾哨兵'\0'也作为头哨兵首先入栈
	while (!optr.empty()) { //在运算符栈非空之前，逐个处理表达式中各字符
		if (isdigit(*S)) { //若当前字符为操作数，则
			readNumber(S, opnd); append(RPN, opnd.top()); //读入操作数，并将其接至RPN末尾
		}
		else //若当前字符为运算符，则
			switch (orderBetween(optr.top(), *S)) { //视其与栈顶运算符之间优先级高低分别处理
			case '<': //栈顶运算符优先级更低时
				optr.push(*S); S++; //计算推迟，当前运算符进栈
				break;
			case '=': //优先级相等（当前运算符为右括号或者尾部哨兵'\0'）时
				optr.pop(); S++; //脱括号并接收下一个字符
				break;
			case '>': { //栈顶运算符优先级更高时，可实施相应的计算，并将结果重新入栈
						  char op = optr.pop(); append(RPN, op); //栈顶运算符出栈并续接至RPN末尾
						  if ('!' == op) { //若属于一元运算符
							  float pOpnd = opnd.pop(); //只需取出一个操作数，并
							  opnd.push(calcu(op, pOpnd)); //实施一元计算，结果入栈
						  }
						  else { //对于其它（二元）运算符
							  float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //取出后、前操作数 /*DSA*/提问：可否省去两个临时变量？
							  opnd.push(calcu(pOpnd1, op, pOpnd2)); //实施二元计算，结果入栈
						  }
						  break;
			}
			default: exit(-1); //逢语法错误，不做处理直接退出
		}//switch
//		/*DSA*/displayProgress(expr, S, opnd, optr, RPN);
	}//while
	return opnd.pop(); //弹出并返回最后的计算结果
}