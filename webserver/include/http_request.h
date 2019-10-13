//
// Created by 费渝航 on 2019/10/9.
//

#ifndef TINYSERVER_HTTP_REQUEST_H
#define TINYSERVER_HTTP_REQUEST_H
int handler_http_request(int sockfd,char *rootdir);
void parse_header(char *header, char *method, char *uri, char *protocol);
char * parse_uri(int sockfd, char *uri, char *rootdir);
ssize_t read_line(int sockfd, char *buf, int size);
#endif //TINYSERVER_HTTP_REQUEST_H
