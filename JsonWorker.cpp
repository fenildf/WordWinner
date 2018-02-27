//
// Created by kang on 18-2-1.
//

#include <iostream>
#include "JsonWorker.h"
std::string JsonWorker::getString(std::string &key, const char *read_buffer)
{
    if(reader_.parse(read_buffer, value_))
        return value_[key].asString();
    else
        return key;

}
JsonWorker& JsonWorker::worker()
{
    static JsonWorker worker;
    return worker;
}
void JsonWorker::setKey(std::string &key, std::string &value)
{
    value_[key] = value;
}
std::string JsonWorker::getWriteBuffer()
{
//    std::cout << "value "<< value_.toStyledString();
//    std::cout << "write" << writer_.write(value_).c_str();
//    return value_.toStyledString().c_str();
    return writer_.write(value_);
}
size_t JsonWorker::getWriteBufferSize()
{
    return value_.toStyledString().size();
}