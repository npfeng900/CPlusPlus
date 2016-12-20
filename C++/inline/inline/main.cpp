//
//  main.cpp
//  inline
//
//  Created by Niu Panfeng on 15/5/17.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include <iostream>
//#include "fun.h"
using namespace std;

extern inline void fun1();

int main(int argc, const char * argv[]) {
    fun1();
    return 0;
}
