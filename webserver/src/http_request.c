//
// Created by 费渝航 on 2019/10/9.
//



#include <sys/socket.h>
#include <arpa/inet.h>
#include <printf.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/http_request.h"
#include "var.h"


int handler_http_request(int sockfd,char *rootdir){
    char buf[BUFSIZE];
    char method[HEADERSIZE], uri[BUFSIZE], protocol[HEADERSIZE];
    char *filename;
    struct stat sbuf;
    //
    read_line(sockfd,buf, sizeof(buf));
    // Parse header to three parts
    parse_header(buf, method, uri, protocol);

    printf("%s %s %s\n", method, uri, protocol);
    // Discard the rest request header
    while (1) { if (read_line(sockfd, buf, sizeof(buf)) == 0) break; };

    //Get request method
    if(!strcasecmp(method, "GET")) {
        //URL validation
        if (strlen(uri) > HEADERSIZE)
            response_400(sockfd);
        else if (strstr(uri, "..") != NULL)
            response_403(sockfd);
            // Response GET
        else
        {
            // Parse uri to filename
            filename = parse_uri(sockfd, uri, rootdir);
            stat(filename, &sbuf);
            if (S_ISDIR(sbuf.st_mode))
                strncat(filename, "/index.html", 12);
            if (access(filename, F_OK) < 0)
                response_404(sockfd);
            else
            {
                set_header(sockfd, 200, filename);
                serve_file(sockfd, filename);
            }
            free(filename);
        }
    }

        // Head request method
    else if(!strcasecmp(method, "HEAD")) {
        // URL validation
        if (strlen(uri) > HEADERSIZE)
            set_header(sockfd, 400, "./400.html");
        else if (strstr(uri, "..") != NULL)
            set_header(sockfd, 403, "./403.html");
            // Response HEAD
        else
        {
            //Parse uri to filename
            filename = parse_uri(sockfd, uri, rootdir);
            stat(filename, &sbuf);
            if (S_ISDIR(sbuf.st_mode))
                strncat(filename, "/index.html", 12);
            if (access(filename, F_OK) < 0)
                set_header(sockfd, 404, "./404.html");
            else
                set_header(sockfd, 200, filename);
            free(filename);
        }
    }

        //Other method - 501 - unimplemented
    else
        response_501(sockfd);

    memset(buf, 0, sizeof(buf));
    memset(method, 0, sizeof(method));
    memset(uri, 0, sizeof(uri));
    memset(protocol, 0, sizeof(protocol));

}



void parse_header(char *header, char *method, char *uri, char *protocol)
{
    size_t i = 0;
    size_t j = 0;

    while (i < HEADERSIZE && j < strlen(header)) {
        if (header[j] == ' ') {
            method[i] = '\0';
            i = 0;
            break;
        }
        method[i] = header[i];
        i++;
        j++;
    }

    while (header[j] == ' ' && j < strlen(header)) {
        j++;
    }

    while (j < strlen(header) && i < strlen(header)) {
        if (header[j] == ' ') {
            uri[i] = '\0';
            i = 0;
            break;
        }
        uri[i] = header[j];
        i++;
        j++;
    }

    while (header[j] == ' ' && j < strlen(header)) {
        j++;
    }

    while (j < strlen(header) && i < HEADERSIZE) {
        if (header[j] == '\n') {
            protocol[i] = '\0';
            break;
        }
        protocol[i] = header[j];
        i++;
        j++;
    }

}


char * parse_uri(int sockfd, char *uri, char *rootdir)
{
    if (uri[strlen(uri)-1] == '/') {
        char *index = "index.html";
        strncat(uri, index, sizeof(char)*strlen(index));
    }

    // Add root directory to file
    char *filedir;

    if ((filedir = malloc(sizeof(char)*(strlen(uri) + strlen(rootdir) + 20))) == NULL)
    {
        fprintf(stderr, "%s: %s\n", "malloc error:", strerror(errno));
        response_500(sockfd);
        exit(1);
    }

    snprintf(filedir, strlen(uri)+strlen(rootdir)+1, "%s%s", rootdir, uri);

    return filedir;
}


ssize_t read_line(int sockfd, char *buf, int size)
{
    ssize_t i = 0;
    char c = '\0';
    ssize_t n;

    while ((i < size - 1) && (c != '\n')) {
        n = recv(sockfd, &c, 1, 0);
        if (n > 0) {
            if (c == '\r') {
                n = recv(sockfd, &c, 1, MSG_PEEK);
                if ((n > 0) && (c == '\n'))
                    recv(sockfd, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';

    return(i-1);
}


