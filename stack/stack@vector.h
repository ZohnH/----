//#pragma once 
//
////����Vector��ͷ�ļ�
//#include "../vector/vector.h" //������Ϊ���࣬������ջģ����
//
//template<typename T>
//class Stack : public Vector<T> //����������
//{
//public:
//	T& top() { return (*this)[size() - 1]; } //ȡ��
//	T pop() { return remove(size() - 1); } //��ջ
//	void push(T const& e) { insert(size(), e); } //��ջ
//}; //����������/ĩ��Ϊջ��/��
///*
// ����������/ĩ��Ϊջ��/��----��ʱ��ջ����ջ��ȡ��ʱ�临�ӶȽ�ΪO(1)
//    �������Ĳ����ɾ������ʱ�����������Ԫ�ص�������
// ����ߵ�����������ջ�ͳ�ջʱ�临�ӶȽ�������O(N)
//*/
//
//
