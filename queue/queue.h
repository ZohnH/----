#pragma once

#include "../List/List.h" //��ListΪ����


template<class T> 
class Queue : public List<T> //����ģ���ࣨ�̳�Listԭ�нӿڣ�
{
public: //size()��empty()�Լ��������Žӿھ���ֱ������
	void enqueue(T const& e){ insertAsLast(e); } //��ӣ�β������
	T dequeue(){ return remove(first()); } //���ӣ��ײ�ɾ��
	T& front(){ return first()->data; } //����
};

