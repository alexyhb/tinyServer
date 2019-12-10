//
// Created by 费渝航 on 2019/10/9.
//

#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

#include "var.h"
#include "http_response.h"
#include "process_manage.h"

void response_400(int sockfd)
{
    set_header(sockfd, 400, "./400.html");

    serve_file(sockfd, "./400.html");
}
void response_403(int sockfd)
{
    set_header(sockfd, 403, "./403.html");

    serve_file(sockfd, "./403.html");
}
void response_404(int sockfd)
{
    set_header(sockfd, 404, "./404.html");

    serve_file(sockfd, "./404.html");
}
void response_500(int sockfd)
{
    set_header(sockfd, 500, "./500.html");

    serve_file(sockfd, "./500.html");
}
void response_501(int sockfd)
{
    set_header(sockfd, 501, "./501.html");

    serve_file(sockfd, "./501.html");
}
void set_header(int sockfd, int code, const char *filename)
{
    // Only adapted to text/html type
    char *type = "Content-Type: text/html; charset=UTF-8\r\n";

    // Get property of file(last-modified & content-length)
    char lastmod[HEADERSIZE];
    char length[HEADERSIZE];
    getPropertyOfFile(filename, lastmod, length);

    // Get current date
    char date[HEADERSIZE];
    getDate(date);

    char *ok_200 = "HTTP/1.0 200 Ok\r\n";

    char *no_implemented_501 = "HTTP/1.0 501 Not Implemented\r\n";

    char *not_found_404 = "HTTP/1.0 404 Not Found\r\n";

    char *bad_request_400 = "HTTP/1.0 400 Bad Request\r\n";

    char *forbidden_403 = "HTTP/1.0 403 Forbidden\r\n";

    char *internal_server_error_500 = "HTTP/1.0 500 Internal Server Error\r\n";

    char *endline = "\r\n";

    switch(code)
    {
        case 200:
            send(sockfd, ok_200, strlen(ok_200), 0);
            break;
        case 400:
            send(sockfd, bad_request_400, strlen(bad_request_400), 0);
            break;
        case 403:
            send(sockfd, forbidden_403, strlen(forbidden_403), 0);
            break;
        case 404:
            send(sockfd, not_found_404, strlen(not_found_404), 0);
            break;
        case 500:
            send(sockfd, internal_server_error_500, strlen(internal_server_error_500), 0);
            break;
        case 501:
            send(sockfd, no_implemented_501, strlen(no_implemented_501), 0);
            break;
        default:
            break;
    }

    send(sockfd, date, strlen(date), 0);
    send(sockfd, SERVER_STRING, strlen(SERVER_STRING), 0);
    send(sockfd, type, strlen(type), 0);
    send(sockfd, length, strlen(length), 0);
    send(sockfd, lastmod, strlen(lastmod), 0);
    send(sockfd, endline, strlen(endline), 0);

}

void serve_file(int sockfd, const char *filename)
{
    int fd;
    char *fdp;
    struct stat sbuf;

    fd = Open(filename, O_RDONLY, 0);

    fstat(fd, &sbuf);
    fdp = mmap(0, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (fdp == (void *)-1) {
        fprintf(stderr, "%s: %s\n", "mmap error:", strerror(errno));
        exit(1);
    }
    close(fd);

    send(sockfd, fdp, sbuf.st_size, 0);
    munmap(fdp, sbuf.st_size);
}

void getDate(char *date)
{
    time_t t;
    time(&t);
    strftime(date, HEADERSIZE, "Date: %a, %d %b %Y %H:%M:%S %Z\r\n", gmtime(&t));
}

void getPropertyOfFile(const char *filename, char *lastmod, char *length)
{
    struct stat property;
    const char *file = filename;

    stat(file, &property);

    // Get content length
    int size = property.st_size;
    snprintf(length, HEADERSIZE, "Content-Length: %d\r\n", size);

    // Get last modified date
    strftime(lastmod, HEADERSIZE, "Last-Modified: %a, %d %b %Y %H:%M:%S %Z\r\n", gmtime(&(property.st_mtime)));
}
