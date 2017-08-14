#define  _CRT_SECURE_NO_WARNINGS

#include "stack@list.h"
#include <stdio.h>
#include <iostream>
using namespace std;

#include "rpn.h"


int main()
{
	//int n;
	//printf("Input a decimal number:  ");
	//scanf("%d", &n);
	//Stack<char> s;
	//convert(s, n, 2);
	//while (!s.empty()) printf("%c", s.pop());

	//const char* c = "((({[]}[]))";
	//int size = strlen(c);
	//cout << paren(c, 0, size) << endl;
	//printf("%d\n", paren(c, 0, size));


	char* rpn = new char[1]; 
	rpn[0] = '\0';
	char* s =new char[256];
	cout << "Input formula to evalute: \n"; 
	cin >> s;
	float value = evaluate(s);
	cout << value << endl;

	return 0;
}
