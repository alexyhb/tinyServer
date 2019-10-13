//
// Created by 费渝航 on 2019/10/9.
//

#ifndef TINYSERVER_PROCESS_MANAGE_H
#define TINYSERVER_PROCESS_MANAGE_H
void daemon_start(const char *cmd);
int Open(const char *pathname, int flags, mode_t mode);
pid_t Fork();
FILE * Fopen(const char * path, const char * mode);
#endif //TINYSERVER_PROCESS_MANAGE_H
