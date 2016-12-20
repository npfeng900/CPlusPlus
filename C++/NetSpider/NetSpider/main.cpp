//
//  main.cpp
//  NetSpider
//
//  Created by Niu Panfeng on 15/5/20.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include "socket.h"
#include "http.h"
#include "threadpool.h"

using namespace std;


int main(int argc, const char * argv[])
{
    string urlStart="http://www.qq.com/";
    
    //cout<<DownLoadImg(urlStart.c_str())<<endl;
    BFS(urlStart.c_str());
    
    return 0;
}

/*
void* work(void* arg)
{
    char* p=(char*)arg;
    cout<<p<<flush;
    sleep(1);
    return NULL;
}
int main(int argc, const char * argv[])
{
    struct threadpool *pool = threadpool_init(9, 4);
    string str[52]={"a","b","c","d","e","f","g","h","i","j","k","l","m",
        "n","o","p","q","r","s","t","u","v","w","x","y","z",
        "A","B","C","D","E","F","G","H","I","J","K","L","M",
        "N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
    for(int i=0;i<52;i++)
    {
        threadpool_add_job(pool, work, (void*)str[i].c_str());
    }
    threadpool_destroy(pool);
}
 */
