#include "sort.h"
void BBsort(uint32_t *arr, uint32_t size)
{
	uint32_t j,i,tem;
	for (i = 0; i < size-1;i ++)//size-1����Ϊ�������Լ��Ƚϣ����Աȵ�������һ��
	{
		int count = 0;
		for (j = 0; j < size-1 - i; j++)	//size-1-i����Ϊÿһ�˾ͻ���һ�����Ƚ�
		{
			if (arr[j] > arr[j+1])//���������ŷ���ǰһ�����ͺ�һ�����Ƚϣ����ǰ���������һ������λ��
			{
				tem = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tem;
				count = 1;
				
			}
		}
		if (count == 0)			//���ĳһ��û�н���λ�ã���˵���Ѿ��ź���ֱ���˳�ѭ��
				break;	
	}
 
}

void test(void){

};

