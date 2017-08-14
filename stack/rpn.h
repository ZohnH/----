//RPN�����沨�����ʽ

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "../stack/stack@vector.h"
#include "priority.h"

//����
__int64 facI(int n);  //factorial�׳�
float calcu(float a, char op, float b); //ִ�ж�Ԫ����
float calcu(char op, float b); //ִ��һԪ����
void displayProgress(char*, char*, Stack<float>&, Stack<char>&, char*);
void readNumber(char*& p, Stack<float>& stk); //����ʼ��p���Ӵ�����Ϊ��ֵ�������������ջ
Operator optr2rank(char);  //�������ת������
char orderBetween(char, char); //�Ƚ����������֮������ȼ�
void append(char*&, float); //������������RPNĩβ
void append(char*&, char); //����  //�����������RPNĩβ
float evaluate(char* expr);  //��׺���ʽ����
float evaluate(char* S, char*& RPN); //�Ա��ʽS��ֵ����ת��Ϊ�沨��ʽRPN
char* removeSpace(char* s);  //�޳�s[]�еİ׿ո�  ��ʽ�����ʱ����ܻ��пո�



//����ʵ��
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
	case '/': if (0 == b) exit(-1); return a / b; //ע�⣺����и�����Ϊ����ܲ���ȫ
	case '^': return pow(a, b);
	default: exit(-1);
	}
}

float calcu(char op, float b)
{
	switch (op)
	{
	case '!': return (float)facI((int)b);//Ŀǰ���н׳ˣ����մ˷�ʽ���
	default: exit(-1);
	}
}

void readNumber(char*& p, Stack<float>& stk)
{ //����ʼ��p���Ӵ�����Ϊ��ֵ�������������ջ
	stk.push((float)(*p - '0')); //��ǰ��λ��Ӧ����ֵ��ջ
	while (isdigit(*(++p)))           //ֻҪ�������н��ڵ����֣�����λ���������������
		stk.push(stk.pop() * 10 + (*p - '0')); //����ԭ��������׷������λ������ֵ������ջ
	if ('.' != *p) return; //�˺��С���㣬����ζ�ŵ�ǰ�������������
	float fraction = 1;   //������ζ�Ż���С������
	while (isdigit(*(++p)))  //��λ����
		stk.push(stk.pop() + (*p - '0')*(fraction /= 10)); //С������
}

Operator optr2rank(char op)
{ //�������ת������
	switch (op)
	{
	case '+': return ADD; //��
	case '-': return SUB; //��
	case '*': return MUL; //��
	case '/': return DIV; //��
	case '^': return POW; //�˷�
	case '!': return FAC; //�׳�
	case '(': return L_P; //������
	case ')': return R_P; //������
	case '\0': return EOE; //��ʼ������ֹ��
	default: exit(-1); //δ֪�����
	}
}

char orderBetween(char op1, char op2)
{ //�Ƚ����������֮������ȼ�
	return pri[optr2rank(op1)][optr2rank(op2)];
}

void append(char*& rpn, float opnd) 
{ //������������RPNĩβ
	int n = strlen(rpn); //RPN��ǰ���ȣ���'\0'��β������n + 1��
	char buf[64];
	if (opnd != (float)(int)opnd) sprintf(buf, "%.2f \0", opnd); //�����ʽ����
	else                          sprintf(buf, "%d \0", (int)opnd); //������ʽ
	rpn = (char*)realloc(rpn, sizeof (char)* (n + strlen(buf) + 1)); //��չ�ռ�
	strcat(rpn, buf); //RPN�ӳ�
}

void append(char*& rpn, char optr)
{ //�����������RPNĩβ
	int n = strlen(rpn); //RPN��ǰ���ȣ���'\0'��β������n + 1��
	rpn = (char*)realloc(rpn, sizeof (char)* (n + 3)); //��չ�ռ�
	sprintf(rpn + n, "%c ", optr); rpn[n + 2] = '\0'; //����ָ���������
}

//char* removeSpace(char* s)
//{  //�޳�s[]�еİ׿ո�
//	
//
//}


float evaluate(char* S)
{ //�Ա��ʽS��ֵ
	Stack<float> opnd; Stack<char> optr; //������ջ�������ջ /*DSA*/�κ�ʱ�̣�����ÿ������Ԫ��֮�����Сһ��
	optr.push('\0'); //β�ڱ�'\0'Ҳ��Ϊͷ�ڱ�������ջ
	while (!optr.empty()) //�������ջ�ǿ�֮ǰ�����������ʽ�и��ַ�
	{
		if (isdigit(*S)) //����ǰ�ַ�Ϊ����������
		{ readNumber(S, opnd); } //���������
		else 
		{     //����ǰ�ַ�Ϊ���������
			switch (orderBetween(optr.top(), *S)) //������ջ�������֮�����ȼ��ߵͷֱ���
			{
			case '<':  //ջ����������ȼ�����ʱ
				optr.push(*S); S++; break; //�����Ƴ٣���ǰ�������ջ,��ȡ��һ���ַ�
			case '=':  //���ȼ���ȣ���ǰ�����Ϊ�����Ż���β���ڱ�'\0'��ʱ
				optr.pop(); S++; break; //�����Ų�������һ���ַ�
			case '>':  //ջ����������ȼ�����ʱ����ʵʩ��Ӧ�ļ��㣬�������������ջ
			{
					char op = optr.pop();
					if ('!' == op)  //������һԪ�����
					{
						float pOpnd = opnd.pop();
						opnd.push(calcu(op, pOpnd));
					}
					else  //������������Ԫ�������
					{
						float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //ȡ����ǰ������ ˳��Ҫ��
						opnd.push(calcu(pOpnd1, op, pOpnd2));
					}
					break;
			}
			default:  //���﷨������ʾ���˳�
				printf("WRONG!!\n");
				exit(-1);  /* �����������case'>'����䲻��{} �ᱨ��default��ǩ������op����ʼ������ */
			}
		}
	}
	return opnd.pop();
}


float evaluate(char* S, char*& RPN) { //�ԣ����޳��׿ո�ģ����ʽS��ֵ����ת��Ϊ�沨��ʽRPN
	Stack<float> opnd; Stack<char> optr; //������ջ�������ջ /*DSA*/�κ�ʱ�̣�����ÿ������Ԫ��֮�����Сһ��
	/*DSA*/ char* expr = S;
	optr.push('\0'); //β�ڱ�'\0'Ҳ��Ϊͷ�ڱ�������ջ
	while (!optr.empty()) { //�������ջ�ǿ�֮ǰ�����������ʽ�и��ַ�
		if (isdigit(*S)) { //����ǰ�ַ�Ϊ����������
			readNumber(S, opnd); append(RPN, opnd.top()); //��������������������RPNĩβ
		}
		else //����ǰ�ַ�Ϊ���������
			switch (orderBetween(optr.top(), *S)) { //������ջ�������֮�����ȼ��ߵͷֱ���
			case '<': //ջ����������ȼ�����ʱ
				optr.push(*S); S++; //�����Ƴ٣���ǰ�������ջ
				break;
			case '=': //���ȼ���ȣ���ǰ�����Ϊ�����Ż���β���ڱ�'\0'��ʱ
				optr.pop(); S++; //�����Ų�������һ���ַ�
				break;
			case '>': { //ջ����������ȼ�����ʱ����ʵʩ��Ӧ�ļ��㣬�������������ջ
						  char op = optr.pop(); append(RPN, op); //ջ���������ջ��������RPNĩβ
						  if ('!' == op) { //������һԪ�����
							  float pOpnd = opnd.pop(); //ֻ��ȡ��һ������������
							  opnd.push(calcu(op, pOpnd)); //ʵʩһԪ���㣬�����ջ
						  }
						  else { //������������Ԫ�������
							  float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //ȡ����ǰ������ /*DSA*/���ʣ��ɷ�ʡȥ������ʱ������
							  opnd.push(calcu(pOpnd1, op, pOpnd2)); //ʵʩ��Ԫ���㣬�����ջ
						  }
						  break;
			}
			default: exit(-1); //���﷨���󣬲�������ֱ���˳�
		}//switch
//		/*DSA*/displayProgress(expr, S, opnd, optr, RPN);
	}//while
	return opnd.pop(); //�������������ļ�����
}