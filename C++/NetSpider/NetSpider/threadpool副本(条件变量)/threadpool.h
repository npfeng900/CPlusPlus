//
//  threadpool.h
//  NetSpider
//
//  Created by Niu Panfeng on 15/6/1.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#ifndef __NetSpider__threadpool__
#define __NetSpider__threadpool__

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <queue>

struct job
{
    void* (*callback_function)(void *arg);//线程回调函数
    void *arg;                            //回调函数参数
};
struct threadpool
{
    int thread_num;                   //线程池中开启线程的个数
    pthread_t *pthreads;              //线程池中所有线程的pthread_t
    
    int queue_max_num;
    pthread_mutex_t queue_mutex;      //互斥信号量
    pthread_cond_t queue_not_full;    //任务队列信号量,表示还能加入任务队列的数目
    pthread_cond_t queue_not_empty;   //任务队列不为空的条件变量
    std::queue<job*> queue_job;       //job队列
    
    int pool_alive;                   //线程池是否已经关闭
};

//===========================================================================
//函数名：                   threadpool_init
//函数描述：                 初始化线程池
//输入：                    [in] thread_num     线程池开启的线程个数
//                         [in] queue_max_num  队列的最大job个数
//输出：                    无
//返回：                    成功：线程池地址 失败：NULL

struct threadpool* threadpool_init(int thread_num, int queue_max_num);
//===========================================================================
//函数名：                    threadpool_add_job
//函数描述：                  向线程池中添加任务
//输入：                     [in] pool                  线程池地址
//                          [in] callback_function     回调函数
//                          [in] arg                     回调函数参数
//输出：                     无
//返回：                     成功：0 失败：-1

int threadpool_add_job(struct threadpool *pool, void* (*callback_function)(void *arg), void *arg);

//===========================================================================
//函数名：                    threadpool_destroy
//函数描述：                   销毁线程池
//输入：                      [in] pool                  线程池地址
//输出：                      无
//返回：                      成功：0 失败：-1

int threadpool_destroy(struct threadpool *pool);

//===========================================================================
//函数名：                    threadpool_function
//函数描述：                  线程池中线程函数
//输入：                     [in] arg                  线程池地址
//输出：                     无
//返回：                     无

void* threadpool_function(void* arg);
#endif /* defined(__NetSpider__threadpool__) */
