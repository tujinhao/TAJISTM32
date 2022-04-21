#include "sort.h"
void BBsort(uint32_t *arr, uint32_t size)
{
	uint32_t j,i,tem;
	for (i = 0; i < size-1;i ++)//size-1是因为不用与自己比较，所以比的数就少一个
	{
		int count = 0;
		for (j = 0; j < size-1 - i; j++)	//size-1-i是因为每一趟就会少一个数比较
		{
			if (arr[j] > arr[j+1])//这是升序排法，前一个数和后一个数比较，如果前数大则与后一个数换位置
			{
				tem = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = tem;
				count = 1;
				
			}
		}
		if (count == 0)			//如果某一趟没有交换位置，则说明已经排好序，直接退出循环
				break;	
	}
 
}

void test(void){

};

