//
// Created by 费渝航 on 2019/10/9.
//

#ifndef TINYSERVER_HTTP_RESPONSE_H
#define TINYSERVER_HTTP_RESPONSE_H
void response_400(int sockfd);
void response_403(int sockfd);
void response_404(int sockfd);
void response_500(int sockfd);
void response_501(int sockfd);
void set_header(int,int,const char *filename);
void serve_file(int sockfd, const char *filename);
void getDate(char *date);
void getPropertyOfFile(const char *filename, char *lastmod, char *length);
#endif //TINYSERVER_HTTP_RESPONSE_H
