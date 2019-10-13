//
// Created by 费渝航 on 2019/10/12.
//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/errno.h>
#include <arpa/inet.h>

#include "var.h"
#include "server_wrap.h"
int Creat_socket()         //to creat the socket and listen
{

    int listen_socket = socket(AF_INET, SOCK_STREAM, 0);      //creat a socket for listening
    if(listen_socket == -1)
    {
        perror("fail to  creat socket");
        return -1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(DEFAULT_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = bind(listen_socket, (struct sockaddr *)&addr, sizeof(addr));    //bind
    if(ret == -1)
    {
        perror("fail to bind");
        return -1;
    }

    ret = listen(listen_socket, 5);   //listen
    if(ret == -1)
    {
        perror("fail to listen");
        return -1;
    }


    return listen_socket;
}

int wait_client(int listen_socket)
{
    struct sockaddr_in cliaddr;
    socklen_t addrlen = sizeof(cliaddr);
    printf("waiting for the client \n");
    int client_socket = accept(listen_socket, (struct sockaddr *)&cliaddr, &addrlen);     //创建一个和客户端交流的套接字
    if(client_socket == -1)
    {
        perror("fail to accept");
        return -1;
    }

    printf("Success to connect a client：%s\n", inet_ntoa(cliaddr.sin_addr),"aaaa");

    return client_socket;
}


