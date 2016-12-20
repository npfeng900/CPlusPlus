//
//  Header.h
//  TEST_FindMin
//
//  Created by Niu Panfeng on 15/4/28.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#ifndef TEST_FindMin_Header_h
#define TEST_FindMin_Header_h

//初始化数组为最小堆
void InitMinHeap(int array[],int i,int length)
{
    int child,temp;
    
    for(temp=array[i];2*i+1<length;i=child)
    {
        child=2*i+1;
        
        if(child<length-1 && array[child+1]<array[child])
            child++;
        
        if (temp>array[child])
            array[i]=array[child];
        else
            break;
        
        array[child]=temp;
        
    }
}

//交换
void Swap(int* a, int* b)
{
    //异或只能交换不相等的值
    //  if(*a==*b)
    //      return;
    
    *a=*a^*b;
    *b=*a^*b;
    *a=*a^*b;
    
}

//得到最小的k个值
int GetMin(int array[],int length,int k)
{
    int min=array[0];
    Swap(&array[0], &array[length-1]);
    
    length=length-1;
    //输出数组

    
    int child;
    
    for (int temp=array[0],i=0,j=k-1; j>0 && 2*i+1<length; j--,i=child)
    {
        child=2*i+1;
        if (child<length-1 && array[child]>array[child+1])
            child++;
        
        if (temp>array[child])
            array[i]=array[child];
        else
            break;
        
        array[child]=temp;
    }
    
    return min;
}

#endif
