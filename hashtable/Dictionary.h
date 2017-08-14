#pragma once
 
template <class K, class V>
struct Dictionary //词典Dictionary模板类
{
	virtual int size() const = 0; //当前词条总数--纯虚函数
	virtual bool put(K, V) = 0; //插入词条（禁止雷同词条时可能失败）
	virtual V* get(K k) = 0;  //读取词条
	virtual bool remove(K k) = 0; //删除词条
};

/*
抽象基类：必须至少包含一个纯虚函数
纯虚函数声明的结尾处为 =0， 在类中可以不定义该函数。
当类声明中包含纯虚函数时，则不能创建该类的对象。包含纯虚函数的类只能用作基类。
*/


template <class K,  class V>
struct Entry  //词条模板类
{
	K key; V value; //关键码、数值
	Entry(K k = K(), V v = V()) : key(k), value(v) {}; //默认构造函数
	Entry(Entry<K, V> const& e) : key(e.key), value(e.value) {};//基于克隆的构造函数

	bool operator< (Entry<K, V> const& e) { return key < e.key; } //比较器：小于
	bool operator> (Entry<K, V> const& e) { return key > e.key; } //比较器：大于
	bool operator== (Entry<K, V> const& e) { return key == e.key; } //比较器：等于
	bool operator!= (Entry<K, V> const& e) { return key != e.key; } //比较器：不等于
};//得益于比较器和判等器，从此往后，不必严格区分词条及其对应的关键码--通过比较词条就可