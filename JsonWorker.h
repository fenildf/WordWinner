//
// Created by kang on 18-2-1.
//

#ifndef WORDWINNER_JSONWORKER_H
#define WORDWINNER_JSONWORKER_H

#include <json/json.h>
class JsonWorker {
public:
    JsonWorker() = default;
    static JsonWorker& worker();
    std::string getString(std::string &key, const char *read_buffer);
    void setKey(std::string &key, std::string &value);
    std::string getWriteBuffer();
    size_t getWriteBufferSize();
private:
    Json::Value value_;
    Json::FastWriter writer_;
    Json::Reader reader_;
    char write_buffer_[1000];
};


#endif //WORDWINNER_JSONWORKER_H
