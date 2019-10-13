//
// Created by 费渝航 on 2019/10/10.
//

#ifndef TINYSERVER_SERVER_CONFIG_H
#define TINYSERVER_SERVER_CONFIG_H
int strkv(char *src, char *key, char *value);
void get_config(char *configFilePath, struct configItem * configVar, int configNum);
#endif //TINYSERVER_SERVER_CONFIG_H
