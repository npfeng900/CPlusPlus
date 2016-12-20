//
//  main.cpp
//  TEST_StringFind
//
//  Created by Niu Panfeng on 15/4/26.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//
#include <iostream>
#include <string>

using namespace std;

//给定一个字符串 A,要求在 A 中查找一个子串 B。如 A="ABCDF",要你在 A 中查找子串 B=“CD”的位置。

int StrFind(const char *string,const char *subString)
{
    int len1=(int)strlen(string);
    int len2=(int)strlen(subString);
    int j;
    
    for (int i=0; i<len1-len2; i++) {
        for (j=0; j<len2; j++) {
            if (string[i+j]!=subString[j]) {
                break;
            }
         }
        if (j==len2) {
            return i+1;
        }
    }
    return -1;
}

//在一个字符串中找到第一个只出 现一次的字符。如输入 abaccdeff,则输出 b
char FindFirstUniqueChar(const char *str)
{
    if (str==NULL) {
        return '\0';
    }
    
    int data[256]={0};
    const char *p=str;
    while (*p!='\0') {
        data[(unsigned char)*p++]++;
    }
    
    while(*str != '\0'){
        if (data[(unsigned char)*str]==1) {
            return *str;
        }
        str++;

    }
    
    return '\0';
}

//Main函数
int main(int argc, const char * argv[]) {
    // insert code here...
    string str1 = "ABBBDDFFFE";
    string str2 = "D";
    
    cout<<StrFind(str1.c_str(), str2.c_str())<<endl;
    

    cout<<FindFirstUniqueChar(str1.c_str())<<endl;
    
    return 0;
}
