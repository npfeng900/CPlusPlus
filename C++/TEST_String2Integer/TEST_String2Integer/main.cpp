//
//  main.cpp
//  TEST_String2Integer
//
//  Created by Niu Panfeng on 15/4/27.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include <iostream>
#include <string>
#include <assert.h>
using namespace std;

int str_2_int(string str)
{
    assert(str.size()>0);
    
    int pos=0;
    int sym=1;
   
    if(str[pos]=='+'){
        pos++;
    }
    else if(str[pos]=='-')
    {
        pos++;
        sym=-1;
    }
    
    int num=0;
    while(pos<str.length())
    {
        assert(str[pos]>='0');
        assert(str[pos]<='9');
        num=num*10+str[pos]-'0';
        
        assert(num>=0);
        pos++;
    }
    num*=sym;
    
    return num;
}


int main(int argc, const char * argv[]) {
    string str = "10324";
    int num = str_2_int(str);
    cout << num << endl;
    return 0;
}
