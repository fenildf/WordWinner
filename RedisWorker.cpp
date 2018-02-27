//
// Created by kang on 18-1-24.
//

#include <iostream>
#include "RedisWorker.h"
//RedisWorker::RedisWorker(std::string ip, short port):redis_ip_(ip),
//                                                redis_port_(port)
//{
//
//}
RedisWorker& RedisWorker::worker()
{
    static RedisWorker redisWorker;
    return redisWorker;
}
int RedisWorker::connectRedisServer()
{
    context_ = redisConnect(RedisWorker::redis_ip_.c_str(),RedisWorker::redis_port_);
    if(context_ == NULL && context_->err){
        logError();
        return -1;
    }else
        return 0;
}
int RedisWorker::reConnectRedisServer()
{
    return  connectRedisServer();
}
int RedisWorker::sendCommand(std::string command)
{
    const char * cmd = command.c_str();
    reply_ = (redisReply *)redisCommand(context_,cmd);
    std::cout << "reply1:" << reply_->str << std::endl;

    if(reply_ == NULL && context_->err){
        logError();
        return -1;
    }else
        return 0;
}
std::string RedisWorker::getReply()
{
    std::string rep(reply_->str);
    freeReplyObject(reply_);
    return rep;
}
void RedisWorker::logError()
{
    std::cout << context_->err ;
}