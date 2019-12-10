//
// Created by 费渝航 on 2019/10/12.
//

#ifndef TINYSERVER_SERVER_WRAP_H
#define TINYSERVER_SERVER_WRAP_H
int Socket(int domain, int type, int protocol);
int Bind(int sockfd, const struct sockaddr *serv_addr, socklen_t serv_addr_len);
int Accept(int sockfd, struct sockaddr *cli_addr, socklen_t *cli_addr_len);
int Listen(int sockfd, int backlog);

#endif //TINYSERVER_SERVER_WRAP_H
