#include "server.h"
// #include "tcp.h"

void main(){
    tableArr_Init();
    epoll_loop(NULL);

    return ;
} 
