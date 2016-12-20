//
//  socket.cpp
//  NetSpider
//
//  Created by Niu Panfeng on 15/5/23.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#include "socket.h"
#include <iostream>
#include <netinet/in.h> //socket
#include <arpa/inet.h>  //inet_pton
#include <unistd.h>     //close
#include <fstream>      //file

Csocket::Csocket(){};
Csocket::~Csocket(){};
//创建socket并返回
int Csocket::Creat()
{
    // 声明并初始化一个socket地址结构
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htons(INADDR_ANY);
    client_addr.sin_port = htons(0);
    
    // 创建socket
    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0)
    {
        std::cout<<"Create Socket Failed!\n";
        return -1;
    }
    
    // 绑定客户端的socket和客户端的socket地址结构 非必需
    if(bind(socket_fd, (struct sockaddr*)&client_addr, sizeof(client_addr))==-1)
    {
        std::cout<<"Socket Bind Failed!\n";
        return -1;
    }
    
    return socket_fd;
};
//连接服务器
int Csocket::Connect(int socket_client, const char *server_strIP, int server_port)
{
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    if(inet_pton(AF_INET, server_strIP, &server_addr.sin_addr) == 0)
    {
        std::cout<<"Server IP Address Error\n";
        return -1;
    }
    
    
    // 向服务器发起连接，连接成功后client_socket_fd代表了客户端和服务器的一个socket连接
    socklen_t server_addr_length = sizeof(server_addr);
    if(connect(socket_client, (struct sockaddr*)&server_addr, server_addr_length) < 0)
    {
        std::cout<<"Can Not Connect To Server IP\n";
        return -1;
    }
    
    return 0;
};
//关闭socket
int Csocket::Close(int socket)
{
    if(close(socket)!=0)
    {
        std::cout<<"Close socket Error\n";
         return -1;
    };
    return 0;
};
//发送信息
int Csocket::Send(int socket, const char *sendBuf, int len)
{
    int result=(int)send(socket,sendBuf,len,0);
    return result;
};
//接收信息,并存储子buffer中，buffer没有free
int Csocket::Recv(int socket,char* &buffer,int &bytesRead)
{
    using namespace std;
    // recv函数接收数据到缓冲区buffer中
    int bufSize=10240;
    buffer=(char*)malloc(bufSize);
    bzero(buffer, bufSize);
    int ret=0;
    bytesRead=0;
    
    do{
        ret=(int)recv(socket, buffer+bytesRead, bufSize-bytesRead, 0);
        if(ret > 0)
            bytesRead += ret;
        else
            break;
        
        if((bufSize-bytesRead)<100)
        {
            bufSize*=2;
            buffer=(char*)realloc(buffer, bufSize);
        }
        
    }while (ret>0);
    
    buffer=(char*)realloc(buffer, bytesRead+1);
    buffer[bytesRead]='\0';
    
    return bytesRead+1;
};