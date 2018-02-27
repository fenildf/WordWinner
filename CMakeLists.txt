cmake_minimum_required(VERSION 3.8)
project(WordWinner)

set(CMAKE_CXX_STANDARD 11)
LINK_LIBRARIES(/usr/lib/libhiredis.a)
LINK_LIBRARIES(/usr/lib/libjsoncpp.a)
LINK_LIBRARIES(/usr/lib/libpthread.so)
LINK_LIBRARIES(/usr/lib64/libmysqlcppconn.so )
LINK_LIBRARIES(/usr/lib/libjsoncpp.a)
INCLUDE_DIRECTORIES(/usr/local/include/redis)

#
#set(SOURCE_FILES Player.cpp User.cpp Client.cpp JsonWorker.cpp)
#add_executable(Client ${SOURCE_FILES})

set(SOURCE_FILES Server.cpp RedisWorker.cpp JsonWorker.cpp MySQLWorker.cpp)
add_executable(Server ${SOURCE_FILES})