//
// Created by kang on 18-1-24.
//

#include <iostream>
#include <random>
#include "Server.h"

int Server::Listen()
{
    int optrval = 1;

    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(socket_fd_, SOL_SOCKET, SO_REUSEADDR, (void *)&optrval,sizeof(int));

    memset(&server_addr_, 0, sizeof(struct sockaddr_in));
    server_addr_.sin_family = AF_INET;
    server_addr_.sin_port = htons(port_);
    server_addr_.sin_addr.s_addr = htonl(INADDR_ANY);

    setNonBlock(socket_fd_);

    bind(socket_fd_, (struct sockaddr *)&server_addr_, sizeof(struct sockaddr_in));
    listen(socket_fd_, LISTEN_LEN);
    return 0;
}
bool Server::setNonBlock(int fd) {
    if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)|O_NONBLOCK) == -1){
        logError("fcntl");
        return false;
    }else
        return true;
}
int Server::addFdEpoll(int fd, uint32_t flag)
{
    ev_.data.fd = fd;
    ev_.events = flag;
    if(!epoll_ctl(kdpfd_, EPOLL_CTL_ADD, fd, &ev_))
        return 0;
    else{
        logError("epoll_ctl");
        return -1;
    }
}
int Server::delFdEpoll(int fd)
{
    if(!epoll_ctl(kdpfd_, EPOLL_CTL_DEL, fd, NULL))
        return 0;
    else{
        logError("epoll_ctl");
        return -1;
    }

}
int Server::modFdEpoll(int fd, uint32_t flag)
{
    ev_.data.fd = fd;
    ev_.events = flag;
    if(!epoll_ctl(kdpfd_, EPOLL_CTL_MOD, fd, &ev_))
        return 0;
    else{
        logError("epoll_ctl");
        return -1;
    }
}
int Server::createFdEpoll()
{
    kdpfd_ = epoll_create(1024);
    if(kdpfd_ == -1){
        logError("epoll_create");
        return -1;
    } else
        return 0;
}
void Server::logError(const char function[])
{
    //std::cout << function  <<"Error:" << std::endl;
    perror(function);
}
void Server::dealEvent()
{
    for (int n = 0; n < nfds_; n++) {
        if (events_[n].data.fd == socket_fd_) {
            socklen_t sock_len = sizeof(struct sockaddr_in);
            conn_fd_ = accept(socket_fd_, (struct sockaddr *) &client_addr_, &sock_len);
            if (conn_fd_ < 0) {
                logError("accept");
                continue;
            }
            accept_count_++;

            if (curfds_ >= MAX_POLL_SIZE) {
                close(conn_fd_);
                continue;
            }
            setNonBlock(conn_fd_);
            addFdEpoll(conn_fd_, EPOLLIN | EPOLLRDHUP);
            curfds_++;
            continue;
        } else if (events_[n].events & EPOLLRDHUP) {
            delFdEpoll(events_[n].data.fd);
            continue;
        } else if (events_[n].events & EPOLLIN) {
            std::cout << "add task" << std::endl;
            dealTask(events_[n].data.fd);
        }

    }
}
void Server::run()
{
    createFdEpoll();
    addFdEpoll(socket_fd_,EPOLLIN);
    curfds_ = 1;
    accept_count_ = 0;
    while(1) {
        nfds_ = epoll_wait(kdpfd_, events_, curfds_, -1);
        if (nfds_ == -1) {
            logError("epoll_wait");
            continue;
        }
        dealEvent();
    }
}

static FunctionMap task = {{SIGN_IN,signIn},
                           {SIGN_UP,signUp},
                           {REFERSH, refreshInfo},
                           {ADD_FRIEND, addFriend},
                           {DEL_FRIEND,delFriend},
                           {BATTLE_WITH_FRIEND, battleWithFriend},
                           {BATTLE_WITH_STARNGER, battleWithStranger},
                           {ANSWER, answer}

        };

void Server::dealTask(int fd)
{
    std::cout << read(fd, packet, 8) << std::endl;
    std::cout << read(fd, packet->load, packet->length) << std::endl;
    task[packet->type](fd, packet->load);
}

void signIn(int fd, char *buffer)
{
    std::string key("name");
    std::cout << buffer << std::endl;
    std::string name = JsonWorker::worker().getString(key, buffer);
    key = "passwd";
    std::string passwd = JsonWorker::worker().getString(key, buffer);

    RedisWorker::worker().sendCommand("get " + name);
    if(RedisWorker::worker().getReply()  == passwd){

    }
}
void signUp(int fd, char *buffer)
{
    std::string key("name");
    std::string name = JsonWorker::worker().getString(key, buffer);
    key = "passwd";
    std::string passwd = JsonWorker::worker().getString(key, buffer);
    RedisWorker::worker().sendCommand("set " + name + " " + passwd);
    RedisWorker::worker().getReply();

}

void refreshInfo(int fd, char *buffer)
{

}

void addFriend(int fd, char *buffer)
{

}
void delFriend(int fd, char *buffer)
{

}
void prepareQuestion(std::unique_ptr<Battle> battle)
{
    battle->now_question = 0;

    for(int j = 0 ; j < 5; j++){
        std::unique_ptr<Question> question(new Question());
        std::string getRandomKey("RANDOMKEY");
        RedisWorker::worker().sendCommand(getRandomKey);
        question->word = RedisWorker::worker().getReply();
        RedisWorker::worker().sendCommand("get" + question->word);
        std::string right_answer = RedisWorker::worker().getReply();
        int right_index = std::random_device()() % 4;
        for(int i = 0; i < 4; i++){
            if(i == right_index){
                question->choice.push_back(right_answer);
                question->right_answer = right_index + 'A';
            }else{
                RedisWorker::worker().sendCommand(getRandomKey);
                question->choice.push_back(RedisWorker::worker().getReply());
            }
        }
        battle->question.push_back(std::move(question));
    }
}
void preparePlayer(std::unique_ptr<Battle> &&battle,int playerA_ID, int playerB_ID)
{
    battle->A_score = 0;
    battle->B_score = 0;
    battle->playerA_ID = playerA_ID;
    battle->playerB_ID = playerB_ID;
}

void battleWithFriend(int fd, char *buffer)
{
    std::unique_ptr<Battle> battle(new Battle());

    prepareQuestion(std::move(battle));
//    preparePlayer(battle);


}
void battleWithStranger(int fd, char *buffer)
{

}
void answer(int fd, char *buffer)
{

}

BattleMap battle;
OnlineMap online_map;// playerID to fd
OnlineMap fd_map; // fd to playerID
std::string ip("123.206.89.123");
short port = 6380;
std::string RedisWorker::redis_ip_ = ip;
short RedisWorker::redis_port_ = port;
int main()
{
//    test Redis

    RedisWorker::worker().connectRedisServer();

    Server server(5473);
    server.Listen();
    server.run();
    // test MySQL

//    std::string ip = "127.0.0.1";
//    std::string user_name = "root";
//    std::string passwd = "Kk19970610-";
//    std::string dbname = "WordWiner";
//    std::cout << "star" << std::endl;
//
//    MySQLWorker sqlWorker(ip, user_name, passwd,dbname);
//    std::cout << "start" << std::endl;
//
//    if(sqlWorker.connectMySQLServer() == 0)
//        std::cout << "OK" << std::endl;
    return 0;

}

