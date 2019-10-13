//
// Created by 费渝航 on 2019/10/9.
//
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <signal.h>
#include <sys/resource.h>

#include "../include/process_manage.h"

pid_t Fork(){
    pid_t pid;
    if((pid=fork())<0){
        fprintf(stderr, "%s: %s\n", "fork error:", strerror(errno));
        exit(0);
    }
    return pid;
}

void daemon_start(const char *cmd){

    pid_t pid;
    int i,fd0,fd1,fd2;
    struct rlimit rl;
    struct  sigaction sa;
    //1. call umask()
    umask(0);

    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
        perror(cmd);
        exit(1);
    }
    //2. call the parent by fork and exit
    if ((pid = fork()) < 0) {
        perror(cmd);
        exit(1);
    }
    else if (pid != 0) /* parent */
        exit(0);
    setsid();

    //3. call the set id to creat new session
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        perror("Can't ignore SIGHUP");
        exit(1);
    }
    pid = Fork();
    if (pid != 0) /* parent */
        exit(0);

    pid_t daemon_pid = getpid();
    printf("Daemon pid: %d\n", daemon_pid);

    /*
     *      * Close all open file descriptors.
     *           */
    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for (i = 0; i < rl.rlim_max; i++)
        close(i);

    /*
     *      * Attach file descriptors 0, 1, and 2 to /dev/null.
     *           */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    signal(SIGCHLD, SIG_IGN);
}
int Open(const char *pathname, int flags, mode_t mode)
{
    int n;

    if ((n=open(pathname, flags, mode)) < 0) {
        fprintf(stderr, "%s: %s\n", "open error:", strerror(errno));
        exit(1);
    }

    return n;
}

FILE * Fopen(const char * path, const char * mode)
{
    FILE * fd;


    if ((fd = fopen(path, mode)) == NULL)
    {
        fprintf(stderr, "%s: %s\n", "fopen error:", strerror(errno));
        exit(1);
    }

    return fd;
}
