//
//  threadpool.cpp
//  NetSpider
//
//  Created by Niu Panfeng on 15/6/1.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include "threadpool.h"
#include <iostream>
#include <unistd.h>
#include <assert.h>

using namespace std;

struct threadpool* threadpool_init(int thread_num, int queue_max_num)
{
    struct threadpool* pool=(threadpool*)malloc(sizeof(struct threadpool));
    //--------------------------------------------------------
    //线程数目
    pool->thread_num=thread_num;
    //线程数组
    pool->pthreads =(pthread_t*)malloc(sizeof(pthread_t)*thread_num);
    //任务队列的最大数目
    pool->queue_max_num=queue_max_num;
    //初始化互斥量
    if(pthread_mutex_init(&(pool->queue_mutex), NULL)!=0)
    {
        cout<<"Create queue_mutex Failed!\n";
        exit(-1);
    }
    //
    if(pthread_cond_init(&(pool->queue_not_full), NULL)!=0)
    {
        cout<<"Create cond queue_not_full Failed!\n";
        exit(-1);
    }
    //初始化条件量
    if(pthread_cond_init(&(pool->queue_not_empty), NULL)!=0)
    {
        cout<<"Create cond queue_not_empty Failed!\n";
        exit(-1);
    }
    //线程池活动标识
    pool->pool_alive=1;
    //--------------------------------------------------------
    //启动所有的线程等待执行任务
    for(int i=0;i<pool->thread_num;i++)
    {
        if(pthread_create(&(pool->pthreads[i]), NULL, threadpool_function, (void *)pool)!=0)
        {
            cout<<"Create Thread "<<i<<" Failed!\n";
            exit(-1);
        }
    }
    
    return pool;
    
};

int threadpool_add_job(struct threadpool *pool, void* (*callback_function)(void *arg), void *arg)
{
    assert(pool != NULL);
    assert(callback_function != NULL);
    assert(arg != NULL);
    //申请一个job，并将任务传递进去
    struct job *pjob =(struct job*) malloc(sizeof(struct job));
    pjob->callback_function=callback_function;
    pjob->arg=arg;
    //----------------------------------------------
    //将job加入到任务队列中去。
    pthread_mutex_lock(&(pool->queue_mutex));
    //~~~~~~~~~~~~~~~
    while (pool->queue_job.size()==pool->queue_max_num && pool->pool_alive)//无空间加入任务，线程池处于运行状态，等待
    {
        pthread_cond_wait(&(pool->queue_not_full), &(pool->queue_mutex));
    }
    //~~~~~~~~~~~~~~~第一种情况，线程池要关闭了
    if(pool->pool_alive==0)
    {
        pthread_mutex_unlock(&(pool->queue_mutex));
        return 0;
    }
    //~~~~~~~~~~~~~~~第二种情况，线程池有空间加入任务了
    pool->queue_job.push(pjob);
    if (pool->queue_job.size()==1)
    {
        pthread_cond_broadcast(&(pool->queue_not_empty));
    };
    //~~~~~~~~~~~~~~~
    pthread_mutex_unlock(&(pool->queue_mutex));
    //----------------------------------------------
    return 0;
};

void* threadpool_function(void* arg)
{
    struct threadpool *pool = (struct threadpool*)arg;
    struct job *pjob = NULL;
    while (1)
    {
        //----------------------------------------------
        pthread_mutex_lock(&(pool->queue_mutex));
        //~~~~~~~~~~~~~~~
        while (pool->queue_job.empty() && pool->pool_alive)//任务队列为空，线程池处于运行状态，等待
        {
            pthread_cond_wait(&(pool->queue_not_empty), &(pool->queue_mutex));
        }
        //~~~~~~~~~~~~~~~第一种情况，线程池关闭了
        if(pool->pool_alive==0)
        {
            pthread_mutex_unlock(&(pool->queue_mutex));
            pthread_exit (NULL);
        }
        //~~~~~~~~~~~~~~~第二种情况，线程池有任务了
        pjob=pool->queue_job.front();
        pool->queue_job.pop();
        if (pool->queue_job.size()==(pool->queue_max_num-1))
        {
            pthread_cond_broadcast(&(pool->queue_not_full));
        }
        //~~~~~~~~~~~~~~~
        pthread_mutex_unlock(&(pool->queue_mutex));
        //----------------------------------------------
        (*(pjob->callback_function))(pjob->arg);
        free(pjob);
        pjob=NULL;
        
    }
};

int threadpool_destroy(struct threadpool *pool)
{
    assert(pool !=NULL);
   
    if (pool->pool_alive==0)
        return -1;
    
    pthread_mutex_lock(&(pool->queue_mutex));
    pool->pool_alive=0;
    pthread_mutex_unlock(&(pool->queue_mutex));
    
    //激活线程
    pthread_cond_broadcast(&(pool->queue_not_empty));
    pthread_cond_broadcast(&(pool->queue_not_full));
    
    for (int i=0; i<pool->thread_num; i++)
    {
        pthread_join(pool->pthreads[i], NULL); ;
    }
    free(pool->pthreads);
    pool->pthreads=NULL;
    pthread_mutex_destroy(&(pool->queue_mutex));
    pthread_cond_destroy(&(pool->queue_not_full));
    pthread_cond_destroy(&(pool->queue_not_empty));
    //删除任务队列
    struct job *pjob;
    while (pool->queue_job.empty()!=true)
    {
        //----------------------------------------------
        pjob=pool->queue_job.front();
        pool->queue_job.pop();
        free(pjob);
        pjob=NULL;
        //----------------------------------------------
    }
    //删除线程池
    free(pool);
    
    return 0;
};