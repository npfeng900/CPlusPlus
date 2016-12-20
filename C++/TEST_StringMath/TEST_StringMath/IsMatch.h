//
//  IsMatch.h
//  TEST_StringMath
//
//  Created by Niu Panfeng on 15/4/26.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#ifndef TEST_StringMath_IsMatch_h
#define TEST_StringMath_IsMatch_h

#include <string>

bool IsMatch(const char *strOne, const char *strTwo)
{
    int lenofOne=(int)strlen(strOne);
    int lenofTwo=(int)strlen(strTwo);
    
    //字符串长度不一样则返回错误
    if (lenofOne != lenofTwo) {
        return false;
    }
    
    int hash[26]={0};
    
    //扫描字符串
    for ( int i=0; i<strlen(strOne) ; i++) {
        int index=strOne[i]-'A';
        hash[index]++;
        
    }
    
    for (int j=0; j<strlen(strTwo); j++) {
        int index=strTwo[j]-'A';
        
        if (hash[index]!=0) {
            hash[index]--;
        }
        else
            return false;
    }
    
    return true;
};

#endif
