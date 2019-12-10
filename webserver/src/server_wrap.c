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
int Socket(int domain, int type, int protocol)
{
    int n;
    if((n=socket(domain, type, protocol)) < 0){
        perror("Fail to create socket");
        exit(1);
    }
    return n;
}


int Bind(int sockfd, const struct sockaddr *serv_addr, socklen_t serv_addr_len)
{
    int n;
    if((n=bind(sockfd, serv_addr, serv_addr_len)) < 0){
        perror("Fail to bind");
        exit(1);
    }
    return n;
}


int Accept(int sockfd, struct sockaddr *cli_addr, socklen_t *cli_addr_len)
{
    int n;

    again:
    if((n=accept(sockfd, cli_addr, cli_addr_len)) < 0){
        if((errno == ECONNABORTED) || (errno == EINTR))
            goto again;
        else{
            perror("Fail to accept sockfd");
            exit(1);
        }
    }
    return n;
}

int Listen(int sockfd, int backlog)
{
    int n;
    if((n=listen(sockfd, backlog)) < 0){
        perror("Fail to listen");
        exit(1);
    }
    return n;
}
