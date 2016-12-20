//
//  main.cpp
//  BASE_1
//
//  Created by Niu Panfeng on 15/5/17.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include <iostream>
#include "fun.h"

using namespace std;
int main(int argc, const char * argv[])
{

    //int *a1=new int[4]{1,3,4,3};
    //int *a2=new int[4]();
    // int* a[]={a1,a2};
    
    int** a;
    int m,n;
    m=2;
    n=5;
    a=new int*[m];
    for (int i=0; i<m; i++)
    {
        *(a+i)=new int[n]{3,3,3,2,4};
    }
    
    fun(a,m,n);
    return 0;
}
