//
// Created by kang on 18-1-26.
//

#include <iostream>
#include "MySQLWorker.h"
MySQLWorker::MySQLWorker(std::string host, std::string user, std::string pwd, std::string dbname):
                        host_(host), user_(user), passwd_(pwd), dbname_(dbname)
{

}
int MySQLWorker::connectMySQLServer()
{
    if(!mysql_real_connect(sql_handler_.get(), host_.c_str(), user_.c_str(), passwd_.c_str(), dbname_.c_str(),0,NULL, 0)){
        logError();
        return -1;
    }
    return 0;
}
void MySQLWorker::logError()
{
    std::cout << mysql_error(sql_handler_.get()) << std::endl;
}
MySQLWorker::~MySQLWorker()
{
    mysql_close(sql_handler_.get());
}