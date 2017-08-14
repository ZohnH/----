#pragma once
 
template <class K, class V>
struct Dictionary //�ʵ�Dictionaryģ����
{
	virtual int size() const = 0; //��ǰ��������--���麯��
	virtual bool put(K, V) = 0; //�����������ֹ��ͬ����ʱ����ʧ�ܣ�
	virtual V* get(K k) = 0;  //��ȡ����
	virtual bool remove(K k) = 0; //ɾ������
};

/*
������ࣺ�������ٰ���һ�����麯��
���麯�������Ľ�β��Ϊ =0�� �����п��Բ�����ú�����
���������а������麯��ʱ�����ܴ�������Ķ��󡣰������麯������ֻ���������ࡣ
*/


template <class K,  class V>
struct Entry  //����ģ����
{
	K key; V value; //�ؼ��롢��ֵ
	Entry(K k = K(), V v = V()) : key(k), value(v) {}; //Ĭ�Ϲ��캯��
	Entry(Entry<K, V> const& e) : key(e.key), value(e.value) {};//���ڿ�¡�Ĺ��캯��

	bool operator< (Entry<K, V> const& e) { return key < e.key; } //�Ƚ�����С��
	bool operator> (Entry<K, V> const& e) { return key > e.key; } //�Ƚ���������
	bool operator== (Entry<K, V> const& e) { return key == e.key; } //�Ƚ���������
	bool operator!= (Entry<K, V> const& e) { return key != e.key; } //�Ƚ�����������
};//�����ڱȽ������е������Ӵ����󣬲����ϸ����ִ��������Ӧ�Ĺؼ���--ͨ���Ƚϴ����Ϳ�