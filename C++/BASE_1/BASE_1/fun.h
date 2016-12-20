//
//  fun.h
//  BASE_1
//
//  Created by Niu Panfeng on 15/5/17.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#ifndef __BASE_1__fun__
#define __BASE_1__fun__

#include <stdio.h>
#include <iostream>

void fun(int** a, int i_max, int j_max)
{
    for (int i=0; i<i_max; i++)
    {
        for (int j=0; j<j_max; j++)
        {
            std::cout<<a[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}

#endif /* defined(__BASE_1__fun__) */
