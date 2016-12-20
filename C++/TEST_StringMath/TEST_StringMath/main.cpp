//
//  main.cpp
//  TEST_StringMath
//
//  Created by Niu Panfeng on 15/4/26.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//
//假设两个字符串中所含有的字符和个数都相同我们就叫这两个字符串匹配,比如:abcda 和 adabc,

#include <iostream>
#include "IsMatch.h"

using namespace std;


int main(int argc, const char * argv[]) {
    // insert code here...
    string strOne = "ABBAA";
    string strTwo = "BBAAA";
    
    bool flag = IsMatch(strOne.c_str(), strTwo.c_str());
    // 如果为 true 则匹配,否则不匹配
    if (flag == true)
        cout << "Match" << endl;
    else
        cout << "No Match" << endl;
    
    return 0;
}
