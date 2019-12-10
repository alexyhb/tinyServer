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
#include "http_request.h"
#include "server_wrap.h"
#include "process_manage.h"
#include "server_config.h"
#include "http_response.h"

struct configItem configList[] = {
        {"ROOTDIR", 0},
        {"PORT", 0},
        {"METHOD", 0}
};


 
int main(int argc,char* argv[])
{
    int port_number = DEFAULT_PORT;
    char *root_dir = ROOTDIR;
    char *method = CONCURRENT;
    int serv_fd,cli_fd;
    char serv_ip[BUFSIZE], cli_ip[BUFSIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int reuse_on = 1, daemon_on = 0;// if use the daemon process
    int config_num = sizeof(configList)/sizeof(struct configItem);
    int option, i;



    // Get server configuration from config file
    get_config("./.lab2-config", configList, config_num);

    for(i = 0; i < config_num; i++)
    {
        if (!strcmp(configList[i].key, "PORT"))
            port_number = atoi(configList[i].value);
        else if (!strcmp(configList[i].key, "ROOTDIR"))
            root_dir = configList[i].value;
        else if (!strcmp(configList[i].key, "METHOD"))
            method = configList[i].value;
    }

    printf("[Server configuration]  Port:%d  Dir:%s  Method:%s\n", port_number, root_dir, method);
    // Handle Command Line Options
    while((option=getopt(argc,argv,"hp:ds:")) != -1)
    {
        switch(option)
        {
            case 'h':
                printf("Usage: %s -p <port> -h(help) -d(daemon process) -s threads\n", argv[0]);
                exit(0);
            case 'p':
                port_number = atoi(optarg);
                break;
            case 'd':
                daemon_on = 1;
                break;
            case 's':
                if (strcmp(optarg, "fork") == 0 ){

                }else if(strcmp(optarg, "thread") != 0){

                }else{
                    printf("Usage: %s -p <port> -h(help) -d(daemon process) -s threads\n", argv[0]);
                    exit(3);
                }

                break;
            default:
                printf("Usage: %s -p <port> -h(help) -d(daemon process) -s threads\n", argv[0]);
                exit(3);
        }
    }
    // Start daemon process
    if (daemon_on)
        daemon_start("tinyServer");

    // Declare the server socket

    serv_fd = Socket(AF_INET, SOCK_STREAM, 0);
// Reuse same port
    setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_on, sizeof(int));

    socklen_t serv_len = sizeof(serv_addr);
    socklen_t cli_len = sizeof(cli_addr);

    memset(&serv_addr, 0, serv_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port_number);

    //Bind address family
    Bind(serv_fd, (struct sockaddr *) &serv_addr, serv_len);

    //Listen to the socket
    Listen(serv_fd, 10);

    // Print the server information
    inet_ntop(AF_INET, &serv_addr.sin_addr.s_addr, serv_ip, sizeof(serv_ip));
    printf("TinyServer %s:%d is working!!!\n",
           inet_ntop(AF_INET, &serv_addr.sin_addr.s_addr, cli_ip, sizeof(cli_ip)),
           ntohs(serv_addr.sin_port));

    printf("==========waiting for client's request=========\n");

//    pid_t parent_pid = getpid();
    pid_t pid;
    while (1) {

        cli_fd = Accept(serv_fd, (struct sockaddr *) &cli_addr, &cli_len);

        pid = Fork();

        if (pid == 0) {
            close(serv_fd);

            // Print the client connection information
            inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, cli_ip, sizeof(cli_ip));
            printf("client IP:%s\tport:%d\t%d\n",
                   inet_ntop(AF_INET, &cli_addr.sin_addr.s_addr, cli_ip, sizeof(cli_ip)),
                   ntohs(cli_addr.sin_port), cli_fd);

            // Handle the http request.
            handle_http_request(cli_fd, root_dir);

            close(cli_fd);

            exit(0);
        }

        close(cli_fd);
    }

    close(serv_fd);
    return 0;
}


