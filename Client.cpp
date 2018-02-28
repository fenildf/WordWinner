//
// Created by kang on 18-1-21.
//
#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include <iostream>

#include "JsonWorker.h"
int fd;
struct sockaddr_in serverAddr;
int key;
const int MAXSIZE = 5000;
const int PORT = 5473;                  // Your server bind port
const char *IP_ADDR = "127.0.0.1"; //Your server Ip address
struct Packet{
    int length;
    int type;
    char load[0];
}__attribute__ ((__packed__));

int connectServer()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd > 0);

    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    key = inet_aton(IP_ADDR, &serverAddr.sin_addr);
    key = connect(fd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr_in ));
    assert(!key);
    return 0;
}
int main(void)
{
    if(connectServer())
        return -1;

    struct Packet *packet;
    packet =  (struct Packet *)malloc(1000);
    packet->type = 0;
    std::string key("name"),value("kangkang");
    JsonWorker::worker().setKey(key, value);
    key = "passwd";
    value = "kk1997";
    JsonWorker::worker().setKey(key, value);

    packet->length = JsonWorker::worker().getWriteBufferSize() ;
    std::cout << JsonWorker::worker().getWriteBuffer() << std::endl;
    sprintf(packet->load, "%s", JsonWorker::worker().getWriteBuffer().c_str());
    write(fd, packet, packet->length + 8);
    close(fd);
    printf("close\n");

}

