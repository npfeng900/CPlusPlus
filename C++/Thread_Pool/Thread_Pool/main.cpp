//
//  main.cpp
//  Thread_Pool
//
//  Created by Niu Panfeng on 15/5/17.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "threadPool.h"

using namespace std;

void* work(void* arg)
{
    char* p=(char*)arg;
    cout<<"threadpool callback fuction :"<<p<<endl;
    sleep(1);
    return NULL;
}

int main(void)
{
    struct threadpool *pool = threadpool_init(10, 20);
    string str[30]={"1","2","3","4","5","6","7","8","9","10",
        "11","12","13","14","15","16","17","18","19","20",
        "21","22","23","24","25","26","27","28","29","30"};
    
    for(int i=0;i<30;i++)
    {
        threadpool_add_job(pool, work, (void*)str[i].c_str());
        //cout<<str[i]<<endl;
    }
  
   // sleep(5);
    threadpool_destroy(pool);
};