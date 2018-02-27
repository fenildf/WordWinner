//
// Created by kang on 18-1-24.
//

#ifndef WORDWINNER_REDISWORKER_H
#define WORDWINNER_REDISWORKER_H

#include <hiredis.h>
#include <string>

class RedisWorker {
public:
    //RedisWorker(std::string ip, short port);
    RedisWorker() = default;
    RedisWorker(const RedisWorker &) = delete;
    RedisWorker(RedisWorker &&) = delete;
    static RedisWorker& worker();
    int connectRedisServer();
    int reConnectRedisServer();
    int sendCommand(std::string command);
    std::string getReply();
    void closeConnect(){redisFree(context_);}
    ~RedisWorker(){ redisFree(context_); }

    static std::string redis_ip_;
    static short redis_port_;

private:
    void logError();
    redisContext *context_;
    redisReply *reply_;


};


#endif //WORDWINNER_REDISWORKER_H
