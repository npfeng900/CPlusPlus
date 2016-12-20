//
//  main.cpp
//  TEST_FindMin
//
//  Created by Niu Panfeng on 15/4/27.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include <iostream>
#include "Header.h"

using namespace std;

#define MAXLEN 7000


int main(int argc, const char * argv[]) {
  
    //初始化数组，给数组赋值
    int A[MAXLEN];
    for (int i=0; i<MAXLEN; i++) {
        A[i]=MAXLEN-i;
    }
    
    //初始化数组为最小堆
    for (int i=MAXLEN/2-1; i>=0; i--) {
        InitMinHeap(A, i, MAXLEN);
    }
    
    //找出最小的k个数
    int k=6;
    int len=MAXLEN;
    for (int i=k; i>0; i--,len--) {
        int min=GetMin(A,len,i);
        
        cout<<min<<" ";
    }
    cout<<endl;
    
    
    
    return 0;
}
