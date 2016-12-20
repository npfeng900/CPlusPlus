//
//  http.h
//  NetSpider
//
//  Created by Niu Panfeng on 15/5/24.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#ifndef __NetSpider__http__
#define __NetSpider__http__

#include <stdio.h>
#include <string>
#include <unordered_set>
#include <queue>
#include <vector>
#include <fstream>

const char* ToFileName(const char* url);
int CreateDirectory(const char* path);

int ParseURL(const char* url, char* &pHostName, char* &pResourcePath);
int GetHttpRespose(const char* url, char* &httpResponsebuf, int &bytesRead);

int ParseHTML_IMG(const char* httpResponseBuf,std::vector<std::string> &imgURL);
int DownLoadImg(const char* url,std::vector<std::string> &imgURL);
int DownLoadImg(const char* url);

int ParseHTML_URL(const char* httpResponseBuf,std::queue<std::string> &hrefURL);
void BFS(const char*  url,int time_sec=2);

void* Spider_URL(void* arg);

class URL_Set
{
public:
    pthread_mutex_t   urlVisited_mutex;
    std::ofstream file_urlVisited;                     //用来存储所有vistedURL的文件
    pthread_mutex_t   urlToVisit_mutex;
    std::unordered_set <std::string> urlVisited;
    std::queue <std::string> urlToVisit;
    
public:
    URL_Set();
    ~URL_Set();
    int Add_urlVisited(std::string url);
    int Add_urlToVisit(std::string url);
    std::string Get_Front_urlToVisit();
};

#endif /* defined(__NetSpider__http__) */
