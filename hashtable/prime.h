//�������
#pragma  once 

#include "Bitmap.h"

int primeNLT(int lo, int hi, char *file)//����file�ļ��еļ�¼����[low, hi)��ȡ��С������
{
	Bitmap B(file, hi);//file�Ѿ���λͼ��ʽ����¼��hi���ڵ��������������ֻҪ
	while (lo < hi) //��c��ʼ����λ��
	{
		if (B.test(lo)) //���ԣ�����
			lo++;
		else
			return lo;  //�����׸����ֵ�����
	} 
}