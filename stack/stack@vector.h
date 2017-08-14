//#pragma once 
//
////包含Vector的头文件
//#include "../vector/vector.h" //以向量为基类，派生出栈模板类
//
//template<typename T>
//class Stack : public Vector<T> //由向量派生
//{
//public:
//	T& top() { return (*this)[size() - 1]; } //取顶
//	T pop() { return remove(size() - 1); } //出栈
//	void push(T const& e) { insert(size(), e); } //入栈
//}; //以向量的首/末端为栈底/顶
///*
// 以向量的首/末端为栈底/顶----这时入栈、出栈、取顶时间复杂度仅为O(1)
//    （向量的插入和删除操作时间正比于其后方元素的数量）
// 如果颠倒过来：：入栈和出栈时间复杂度将上升到O(N)
//*/
//
//
