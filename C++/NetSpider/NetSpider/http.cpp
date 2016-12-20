//
//  http.cpp
//  NetSpider
//
//  Created by Niu Panfeng on 15/5/24.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include "http.h"
#include "socket.h"     //对socket的封装
#include "threadpool.h" //多线程
#include <netdb.h>      //gethostbyname()
#include <unistd.h>     //inet_ntoa
#include <sys/stat.h>   //mkdir()
#include <sys/types.h>  //mkdir()
#include <arpa/inet.h>  //inet_ntoa()
#include <iostream>
#include <fstream>
#include <string>
//#include <dirent.h>     //为了获取某文件夹目录内容，所使用的结构体。

#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <vector>

using namespace std;

string CurrentDirect="/Users/niupanfeng/Documents/Xcode/test/NetSpider/NetSpider/";
//-----------------------------------------------------------
//url转换为文件名
const char*  ToFileName(const char* url)
{
    const char* chSuffix="";
    char* fileName=(char*)malloc(strlen(url)+strlen(chSuffix));
    char ch;
    int k=0;
    for (int i=0; i<strlen(url); i++)
    {
        ch=*(url+i);
        if (ch!='\\'&&ch!='/'&&ch!=':'&&ch!='*'&&ch!='?'&&ch!='"'&&ch!='<'&&ch!='>'&&ch!='|')
            fileName[k++]=ch;
    }
    
    fileName=(char*)realloc(fileName, k+strlen(chSuffix)+1);
    memcpy(fileName+k, chSuffix, strlen(chSuffix));
    fileName[k+strlen(chSuffix)]='\0';
    
    return fileName;
}
//生成多级目录
int CreateDirectory(const char* path)
{
    if (path==NULL)
        return -1;
    
    char* p=strdup(path);
    int iLen=(int)strlen(p);
    int iRet;
    //判断文件夹是否存在
    if(p[iLen-1]=='/')
        p[iLen-1]='\0';
    if (access(p, 0)==0)
    {
        delete p;
        return 0;
    }
    
    for (int i=1; i<iLen; i++)
    {
        if(p[i]=='\\'||p[i]=='/')
        {
            p[i]='\0';
            if(access(p, 0)!=0)
            {
                iRet=mkdir(p, 0777);
                if (iRet!=0)
                    return -1;
            }
            p[i]='/';
        }
    }
    
    iRet=mkdir(p, 0777);
    free(p);
    return  iRet;
}
//-----------------------------------------------------------
//解析URL
int ParseURL(const char* url, char* &pHostName, char* &pResourcePath)
{
    if ( strlen(url) > 2000 )
        return -1;
    
    const char * pos;
    
    string tag[]={"https://","http://","//"};
    for (int i=0; tag[i].empty()==false; i++)
    {
        pos = strstr( url, tag[i].c_str() );
        if( pos )
        {
            pos+=strlen(tag[i].c_str());
            char hostName[100];
            char resourcePath[2000];
            sscanf( pos, "%[^/]%s", hostName, resourcePath );
            pHostName=strdup(hostName);
            pResourcePath=strdup(resourcePath);
            return 0;
            
        }
    }
    
    return -1;
};
//发送http请求报文，并接收html的返回数据，httpResponsebuf没有free,注意释放
int GetHttpRespose(const char* url, char* &httpResponsebuf, int &bytesRead)
{
    //分析URL
    char *pHostName,*pResourcePath;
    if(ParseURL(url, pHostName, pResourcePath)==-1)
    {
        cout<<"Can not parse the url"<<endl;
        return -1;
    }
    //获取服务器的ip
    struct hostent *host;;
    if((host=gethostbyname(pHostName))==NULL)
    {
        std::cout<<"Gethostbyname Error: "<<pHostName<<endl;
        return -1;
    }
    char* str_host_ip=inet_ntoa(*(struct in_addr*)host->h_addr);
    //创建套间字，并连接服务器
    int sock_fd;
    if((sock_fd=Csocket::Creat())==-1)
    {
        cout << "Can not create sock."<<endl;
        return -1;
    }
    if(Csocket::Connect(sock_fd, str_host_ip)==-1)
    {
        cout << "Can not connect <"<<pHostName<<">"<<endl;
        return -1;
    }
    //http请求报文
    string httpReq = "GET " + string(pResourcePath) + " HTTP/1.1\r\nHost:" + string(pHostName) + "\r\nConnection:Close\r\n\r\n";
    //发送请求报文
    if (Csocket::Send(sock_fd, httpReq.c_str(), (int)httpReq.size())==-1)
    {
        cout<<"send http requset failed!\n";
        return -1;
    }
    //接收数据
    Csocket::Recv(sock_fd, httpResponsebuf,bytesRead);
    //关闭套接字
    Csocket::Close(sock_fd);

    return 0;
}
//-----------------------------------------------------------
//解析html的返回数据,查找网页中的图片链接
int ParseHTML_IMG(const char* httpResponseBuf,std::vector<std::string> &imgURL)
{
    unordered_set<string> visitedIMG;
    //查找网页中的图片链接
    const char* tag="<img ";
    const char* pos=strstr(httpResponseBuf,tag);
    const char* att1= "src=\"";
    
    while (pos)
    {
        pos+=strlen(tag);
        const char* pos1 = strstr( pos, att1 );
        if( pos1 && (pos1<strstr(pos, ">")))
        {
            pos=pos1+strlen(att1);
        }
        
        const char * nextQ = strstr( pos, "\"");
        if( nextQ )
        {
            char * url = new char[ nextQ-pos+1 ];
            
            sscanf( pos, "%[^\"]", url);
            string img_url=string(url);
            
            if(visitedIMG.find(img_url)==visitedIMG.end())
            {
                visitedIMG.insert(img_url);
                imgURL.push_back(img_url);
                //cout<<img_url<<endl;
            }
            
            delete [] url;
            pos=strstr(pos, tag);
        }
    }
    return (int)imgURL.size();
}
//下载指定链接数组中的图片
int DownLoadImg(const char* url,std::vector<std::string> &imgURL)
{
    //生成保存该url下图片的文件夹
    string foldName=ToFileName(url);
    foldName=CurrentDirect+"img/"+foldName;
    if(CreateDirectory(foldName.c_str())!=0)
    {
       cout<<"Can not create directory:"<< foldName<<endl;
       return 0;
    }
    
    int imgURL_catched=0;
    
    for(int i=0;i<imgURL.size();i++)
    {
        string strImgUrl=imgURL[i];
        //判断是否为图片，bmp，jgp，jpeg，gif
        int pos=(int)strImgUrl.find_last_of(".");
        if (pos==string::npos)
        {
            continue;
        }
        else
        {
            string ext=strImgUrl.substr(pos+1,strImgUrl.size()-1-pos);
            if( ext!="bmp"&& ext!="jpg" && ext!="jpeg"&& ext!="gif"&&ext!="png")
            {
                continue;
            }
        }
        //下载其中的内容
        char* imageBuf;       
        int byteRead=0;
        if (GetHttpRespose(strImgUrl.c_str(), imageBuf, byteRead)==0)
        {
            if (byteRead<=0)
            {
                continue;
            }

            const char* p=imageBuf;
            const char * pos = strstr(p,"\r\n\r\n")+strlen("\r\n\r\n");
            int index = (int)strImgUrl.find_last_of("/");
            if(index!=string::npos)
            {
                string imgName=strImgUrl.substr(index,strImgUrl.size()-index);
                ofstream file_out;
                file_out.open(foldName+imgName,ios::binary);
                file_out.write(pos, byteRead-(pos-p));
                file_out.close();
                imgURL_catched++;
            }
            
        }
        free(imageBuf);
        
    }
    return imgURL_catched;
}
//下载指定链接中的图片
int DownLoadImg(const char* url)
{
    char *httpResponseBuf;
    int bytesRead;
    if(GetHttpRespose(url, httpResponseBuf, bytesRead)==-1)
    {
        cout << "GetHttpRespose Wrong!" << endl;
        return 0;
    }
    
    std::vector<std::string> imgURL;
    ParseHTML_IMG(httpResponseBuf, imgURL);
    int n=DownLoadImg(url, imgURL);
    
    free(httpResponseBuf);
    
    return n;
}
//-----------------------------------------------------------
//解析html的返回数据,查找网页中的url链接
int ParseHTML_URL(const char* httpResponseBuf,std::queue<std::string> &hrefURL)
{
    const char* tag="href=\"http://";
    const char* pos=strstr( httpResponseBuf, tag );
    int n=(int)hrefURL.size();
    //查找网页中的链接
    while (pos)
    {
        pos+=strlen(tag);
        
        const char* nextQ = strstr( pos, "\"" );
        if( nextQ )
        {
            char * url = new char[ nextQ-pos+1 ];
            sscanf( pos, "%[^\"]", url);
            string str_url="http://"+string(url);
            hrefURL.push(str_url);
            delete [] url;
            pos=strstr(pos, tag );
        }
        
    }
    
    return (int)hrefURL.size()-n;
    
}

//线程池需要添加的任务回调函数,解析url，添加记录urlVisited，并添加记录urlToVisit
void* Spider_URL(void* arg)
{
    URL_Set* url_set=(URL_Set*)arg;
    string url=url_set->Get_Front_urlToVisit();
    if (url.length()==0)
    {
        return NULL;
    }
    //获取url的回应
    char *httpResponseBuf; //读取http的缓存区指针，缓冲区空间在GetHttpRespose()函数中分配，需要手动释放
    int bytesRead;
    if(GetHttpRespose(url.c_str(), httpResponseBuf, bytesRead)==-1)
    {
        cout << "GetHttpRespose Wrong!" << endl;
        return NULL;
    }
    url_set->Add_urlVisited(url);
    //解析httpResponseBuf中的内容，并进行处理
    pthread_mutex_lock(&(url_set->urlToVisit_mutex));
    ParseHTML_URL(httpResponseBuf,url_set->urlToVisit);
    pthread_mutex_unlock(&(url_set->urlToVisit_mutex));
    //善后处理
    free(httpResponseBuf);
    return NULL;
}

//广度遍历搜索url
void BFS(const char* url,int time_sec)
{
    URL_Set *url_set=new URL_Set();
    url_set->Add_urlToVisit(string(url));
  
    struct threadpool* pool=threadpool_init(10, 50);
    while (1)
    {
        threadpool_add_job(pool, Spider_URL, (void*)url_set);
        sleep(time_sec);
    }
    
   
   // threadpool_destroy(pool);
    
}
//-----------------------------------------------------------
//URL信息类包含，所有的对URL的操作，想声明为单件类
URL_Set::URL_Set()
{
    pthread_mutex_init(&urlVisited_mutex, NULL);
    file_urlVisited.open(CurrentDirect+"url_Visited.txt",ios::trunc);
    pthread_mutex_init(&urlToVisit_mutex, NULL);
};
URL_Set::~URL_Set()
{
    pthread_mutex_destroy(&urlVisited_mutex);
    file_urlVisited.close();
    pthread_mutex_destroy(&urlToVisit_mutex);
};
int URL_Set::Add_urlVisited(string url)
{
    pthread_mutex_lock(&urlVisited_mutex);
    if (urlVisited.find(url)==urlVisited.end())
    {
        urlVisited.insert(url);
        file_urlVisited<<url<<endl<<flush;
    }
    pthread_mutex_unlock(&urlVisited_mutex);
    return 0;
};
int  URL_Set::Add_urlToVisit(string url)
{
    pthread_mutex_lock(&urlToVisit_mutex);
    urlToVisit.push(url);
    pthread_mutex_unlock(&urlToVisit_mutex);
    return 0;
}
string URL_Set::Get_Front_urlToVisit()
{
    string url;
    pthread_mutex_lock(&urlToVisit_mutex);
    if (urlToVisit.empty()==false)
    {
        url=urlToVisit.front();
        urlToVisit.pop();
    }
    pthread_mutex_unlock(&urlToVisit_mutex);
    return url;
};
//-----------------------------------------------------------