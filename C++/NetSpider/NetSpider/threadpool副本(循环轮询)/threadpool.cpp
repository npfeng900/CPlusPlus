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
    //初始化互斥量
    if(pthread_mutex_init(&(pool->queue_mutex), NULL)!=0)
    {
        cout<<"Create queue_mutex Failed!\n";
        exit(-1);
    }
    //打开任务队列信号量（表示还能加入任务队列的数目）
    do
    {
        if (pool->queue_residue_sem)
        {
            sem_close(pool->queue_residue_sem);
            sem_unlink("threadpool_queue_residue_sem");
        }
        pool->queue_residue_sem=sem_open("threadpool_queue_residue_sem",O_CREAT|O_EXCL,0644, queue_max_num);
    }
    while (pool->queue_residue_sem==SEM_FAILED) ;
    //打开任务队列信号量（表示已经加入任务队列的数目）
    do
    {
        if (pool->queue_add_sem)
        {
            sem_close(pool->queue_add_sem);
            sem_unlink("threadpool_queue_add_sem");
        }
        pool->queue_add_sem=sem_open("threadpool_queue_add_sem",O_CREAT|O_EXCL,0644, 0);
    }
    while (pool->queue_add_sem==SEM_FAILED) ;
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
    
    if (pool->pool_alive==0)
    {
        return -1;
    }
    //申请一个job，并将任务传递进去
    struct job *pjob =(struct job*) malloc(sizeof(struct job));
    pjob->callback_function=callback_function;
    pjob->arg=arg;
    //----------------------------------------------
    //将job加入到任务队列中去。
    
    while(sem_trywait(pool->queue_residue_sem)!=0)
    {
        usleep(100);
        continue;
    }
    //----------------------------------------------
    pthread_mutex_lock(&(pool->queue_mutex));
    pool->queue_job.push(pjob);
    sem_post(pool->queue_add_sem);
    pthread_mutex_unlock(&(pool->queue_mutex));
    //---------------------------------------------


    return 0;
};

void* threadpool_function(void* arg)
{
    struct threadpool *pool = (struct threadpool*)arg;
    struct job *pjob = NULL;
    while (pool->pool_alive)
    {
        if(sem_trywait(pool->queue_add_sem)!=0)
        {
           usleep(100);
           continue;
        }
        //----------------------------------------------
        pthread_mutex_lock(&(pool->queue_mutex));
        pjob=pool->queue_job.front();
        pool->queue_job.pop();
        sem_post(pool->queue_residue_sem);//取出一个任务，信号量+1
        pthread_mutex_unlock(&(pool->queue_mutex));
        //----------------------------------------------
        (*(pjob->callback_function))(pjob->arg);
        free(pjob);
        pjob=NULL;
        
    }
    pthread_exit(NULL);
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
    
    for (int i=0; i<pool->thread_num; i++)
    {
        pthread_join(pool->pthreads[i], NULL); ;
    }
    free(pool->pthreads);
    pool->pthreads=NULL;
    pthread_mutex_destroy(&(pool->queue_mutex));
    sem_close(pool->queue_residue_sem);
    sem_unlink("threadpool_queue_residue_sem");
    sem_close(pool->queue_add_sem);
    sem_unlink("threadpool_queue_add_sem");
    
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