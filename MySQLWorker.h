//
// Created by kang on 18-1-26.
//

#ifndef WORDWINNER_MYSQLWORKER_H
#define WORDWINNER_MYSQLWORKER_H

#include <mysql/mysql.h>
#include <string>
#include <memory>


class MySQLWorker {
public:
    MySQLWorker(std::string host,std::string user,std::string pwd,std::string dbname);
    int connectMySQLServer();
    int reConnectMySQLServer();
    ~MySQLWorker();
private:
    std::unique_ptr<MYSQL> sql_handler_;
    std::string user_;
    std::string host_;
    std::string dbname_;
    std::string passwd_

    void logError();
};


#endif //WORDWINNER_MYSQLWORKER_H
