### WordWinner
### 简介
单词王者，一个在线答题对战游戏。  
要在4个选项中选出正确的单词汉译，每个问题只有5秒钟，一旦选择便无更改，超时或者答错都算失败。  
### 服务端技术
服务端使用单线程 I/O 多路复用模型。  
通过时间轮管理定时事件。  
使用 JSON 作为数据交换格式。  
使用Redis 存储。  
利用单例模式封装对数据库的操作。  
使用 std::map 和 std::function 避免繁琐的 if-else。  
采用柔性数组减少内存空间的浪费。  
### 单例模式的使用
对Redis的访问使用单例模式封装为RedisWorker类。  
因为服务端采用单线程模型，所以不会出现多线程争用问题。  
```cpp
class RedisWorker {
public:
    // 通过C++11的delete关键字，删除了copy构造，移动构造，copy赋值运算符，移动赋值运算符  
    RedisWorker(const RedisWorker &) = delete;  
    RedisWorker(RedisWorker &&) = delete;   
    RedisWorker &operator=(const RedisWorker &) = delete;  
    RedisWorker &operator=(RedisWorker &&) = delete; 

    //通过局部static变量，保证在第一次调用时进行初始化，之后调用不会再初始化。  
    static RedisWorker& worker(){  
    	static RedisWorker redisWorker;  
    	return redisWorker;  
    }  

private:  
    RedisWorker() = default;  
};  

```
