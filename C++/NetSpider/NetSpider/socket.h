//
//  socket.h
//  NetSpider
//
//  Created by Niu Panfeng on 15/5/23.
//  Copyright (c) 2015年 Niu Panfeng. All rights reserved.
//

#ifndef __NetSpider__socket__
#define __NetSpider__socket__

#include <stdio.h>


class Csocket
{
private:
    
public:
    Csocket();
    ~Csocket();
    static int Creat();
    static int Connect(int socket_client,const char* server_strIP,int server_port=80);
    static int Close(int socket);
    static int Send(int socket,const char* sendBuf,int len);
    static int Recv(int socket,char* &buffer,int &bytesRead);
};

#endif /* defined(__NetSpider__socket__) */
