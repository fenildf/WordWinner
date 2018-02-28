//
// Created by kang on 18-1-24.
//

#ifndef WORDWINNER_SERVER_H
#define WORDWINNER_SERVER_H

#include <bits/unique_ptr.h>
#include <cstdio>
#include <cstdlib>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <sys/resource.h>
#include <string>
#include <cstring>
#include <iostream>
#include <csignal>
#include <vector>

#include "RedisWorker.h"
#include "MySQLWorker.h"
#include "JsonWorker.h"
typedef char Answer;

struct Question{
    std::string word;
    std::vector<std::string> choice;
    Answer right_answer;
};
enum ClientPacketType{
    SIGN_IN = 0,
    SIGN_UP,
    REFERSH,
    ADD_FRIEND,
    DEL_FRIEND,
    BATTLE_WITH_FRIEND,
    BATTLE_WITH_STARNGER,
    ANSWER
};
enum ServerPacketType{
    REPLY = 200,
    RESULT,

};
typedef struct {
    int length;
    int type;
    char load[0];
}__attribute__ ((__packed__))Packet;

struct TimeEvent{
    unsigned int battle_ID;
    int result;
    int player_fd;
    Answer right_answer;
};
struct Battle{
    unsigned int battle_ID;
    int playerA_ID;
    int playerB_ID;
    std::vector<std::unique_ptr<Question>> question;
    std::vector<Answer> A_answer;
    std::vector<Answer> B_answer;
    int now_question;
    int A_score;
    int B_score;
};

enum TaskType{
    SIGNIN = 1,
    SIGNUP,

    BATTLE,

};
typedef std::map<unsigned int, Battle> BattleMap;
typedef std::map<int, std::function<void(int, char*)>> FunctionMap;
typedef std::map<int, int> OnlineMap;
const int MAX_POLL_SIZE = 1000;
const int LISTEN_LEN = 1000;
class RedisWorker;
class Server
{
public:
    Server(short port):port_(port){packet = (Packet *)malloc(1000);}
    ~Server(){close(socket_fd_); free(packet);}
    Server(const Server &s) = delete;

    int Listen();
    void run();
private:
    bool setNonBlock(int);
    int addFdEpoll(int fd, uint32_t flag);
    int modFdEpoll(int fd, uint32_t flag);
    int delFdEpoll(int fd);
    int createFdEpoll();
    void dealEvent();
    void dealTask(int );
    void logError(const char []);
    short port_;
    int socket_fd_,conn_fd_;
    struct epoll_event ev_;
    struct epoll_event events_[MAX_POLL_SIZE];
    struct rlimit rt_;
    struct sockaddr_in server_addr_,client_addr_;
    int kdpfd_;
    int nfds_;
    int curfds_;
    int accept_count_;
    Packet *packet;
};
void signIn(int fd, char *buffer);
void signUp(int fd, char *buffer);
void refreshInfo(int fd, char *buffer);
void addFriend(int fd, char *buffer);
void delFriend(int fd, char *buffer);
void battleWithFriend(int fd, char *buffer);
void battleWithStranger(int fd, char *buffer);
void answer(int fd, char *buffer);

#endif //WORDWINNER_SERVER_H
