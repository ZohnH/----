//素数相关
#pragma  once 

#include "Bitmap.h"

int primeNLT(int lo, int hi, char *file)//根据file文件中的记录，在[low, hi)内取最小的素数
{
	Bitmap B(file, hi);//file已经按位图格式，记录了hi以内的所有素数，因此只要
	while (lo < hi) //从c开始，逐位地
	{
		if (B.test(lo)) //测试，即可
			lo++;
		else
			return lo;  //返回首个发现的素数
	} 
}