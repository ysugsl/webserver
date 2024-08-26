// webbench.cpp
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <getopt.h>
#include <rpc/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <iostream>
using std::cout;
using std::endl;
//http请求⽅法
#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
#define WEBBENCH_VERSION "v1.5"
//请求⼤⼩
#define REQUEST_SIZE 2048
static int force = 0; //默认需要等待服务器响应

static int force_reload = 0; //默认不重新发送请求
static int clients_num = 1; //默认客户端数量
static int request_time = 30; //默认模拟请求时间
static int http_version = 2; //默认http协议版本 0:http/0.9, 1:http/1.0, 2:http/1.1
static char* proxy_host = NULL; //默认⽆代理服务器
static int port = 80; //默认访问80端⼝
static int is_keep_alive = 0; //默认不⽀持keep alive
static int request_method = METHOD_GET; //默认请求⽅法
static int pipeline[2]; //⽤于⽗⼦进程通信的管道
static char host[MAXHOSTNAMELEN]; //存放⽬标服务器的⽹络地址
static char request_buf[REQUEST_SIZE]; //存放请求报⽂的字节流
static int success_count = 0; //请求成功的次数
static int failed_count = 0; //失败的次数
static int total_bytes = 0; //服务器响应总字节数
volatile bool is_expired = false; //⼦进程访问服务器 是否超时
static void Usage() {
    fprintf(stderr,
    "Usage: webbench [option]... URL\n"
    " -f|--force 不等待服务器响应\n"
    " -r|--reload 重新请求加载(⽆缓存)\n"
    " -9|--http09 使⽤http0.9协议来构造请求\n"
    " -1|--http10 使⽤http1.0协议来构造请求\n"
    " -2|--http11 使⽤http1.1协议来构造请求\n"
    " -k|--keep_alive 客户端是否⽀持keep alive\n"
    " -t|--time <sec> 运⾏时间，单位：秒，默认为30秒\n"
    " -c|--clients_num <n> 创建多少个客户端，默认为1个\n"
    " -p|--proxy <server:port> 使⽤代理服务器发送请求\n"
    " --get 使⽤GET请求⽅法\n"
    " --head 使⽤HEAD请求⽅法\n"
    " --options 使⽤OPTIONS请求⽅法\n"
    " --trace 使⽤TRACE请求⽅法\n"
    " -?|-h|--help 显示帮助信息\n"
    " -V|--version 显示版本信息\n"
    );
}