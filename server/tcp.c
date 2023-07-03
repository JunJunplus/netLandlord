#include "tcp.h"
/*
 * 封装一个自定义事件，包括fd，这个fd的回调函数，还有一个额外的参数项
 * 注意：在封装这个事件的时候，为这个事件指明了回调函数，一般来说，一个fd只对一个特定的事件
 * 感兴趣，当这个事件发生的时候，就调用这个回调函数
 */

/* */
char stage[4][20] = {"登录", "叫地主", "正在游戏", "结束"};
// extern TableFrame *Table[MAX_TABLE]; // 定义桌子的全局变量，每个元素都是一桌，3人为一桌
int usr_id[MAX_TABLE][3];
int u_num = 0;

// 查看当前用户入桌情况
void __check_table()
{
    for (int i = 0; i < MAX_TABLE; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("[%d]桌[%d]号用户的全局变量下标是[%d]   ", i, j, usr_id[i][j]);
        }
        printf("\n");
    }
}

void eventset(struct myevent_s *ev, int fd, void (*call_back)(int fd, int events, void *arg), void *arg)
{
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    if (ev->len < 0)
    {
        memset(ev->buf, 0, sizeof(ev->buf));
        ev->len = 0;
    }
    ev->last_active = time(NULL); // 调用eventset函数的时间
    return;
}

/* 向 epoll监听的红黑树 添加一个文件描述符 */
void eventadd(int efd, int events, struct myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    int op = 0;
    epv.data.ptr = ev;                // ptr指向一个结构体（之前的epoll模型红黑树上挂载的是文件描述符cfd和lfd，现在是ptr指针）
    epv.events = ev->events = events; // EPOLLIN 或 EPOLLOUT
    if (ev->status == 0)              // status 说明文件描述符是否在红黑树上 0不在，1 在
    {
        op = EPOLL_CTL_ADD; // 将其加入红黑树 g_efd, 并将status置1
        ev->status = 1;
    }
    if (epoll_ctl(efd, op, ev->fd, &epv) < 0) // 添加一个节点
    {
        printf("event add failed [fd=%d],events[%d] op[%d]: \n", ev->fd, events, op);
        // perror("");
    }
    else
        // printf("event add OK [fd=%d],events[%0X]\n", ev->fd, events);
        return;
}

/* 从epoll 监听的 红黑树中删除一个文件描述符*/
void eventdel(int efd, struct myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    if (ev->status != 1) // 如果fd没有添加到监听树上，就不用删除，直接返回
        return;
    epv.data.ptr = NULL;
    ev->status = 0;
    // printf("摘下的fd ： %d\n", ev->fd);
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv);
    return;
}

/*  当有文件描述符就绪, epoll返回, 调用该函数与客户端建立链接 */
void acceptconn(int lfd, int events, void *arg)
{
    // int usr_max_num = *((int *)arg);
    // printf("最大用户数量:%d\n", usr_max_num);
    struct sockaddr_in cin;
    socklen_t len = sizeof(cin);
    int cfd, i;
    if ((cfd = accept(lfd, (struct sockaddr *)&cin, &len)) == -1)
    {
        if (errno != EAGAIN && errno != EINTR)
        {
            sleep(1);
        }
        printf("%s:accept,%s\n", __func__, strerror(errno));
        return;
    }
    do
    {
        for (i = 0; i < MAX_EVENTS; i++) // 从全局数组g_events中找一个空闲元素，类似于select中找值为-1的元素
        {
            if (g_events[i].status == 0)
                break;
        }
        if (i == MAX_EVENTS) // 超出连接数上限
        {
            printf("%s: 超出了最大连接上限[%d]\n", __func__, MAX_EVENTS);
            break;
        }
        int flag = 0;
        if ((flag = fcntl(cfd, F_SETFL, O_NONBLOCK)) < 0) // 将cfd也设置为非阻塞
        {
            printf("%s: fcntl nonblocking failed, %s\n", __func__, strerror(errno));
            break;
        }

        //  每登录一位新用户都设置用户的状态
        printf("新的连接<ip:port>:[%s:%d],[time:%ld],g_events[%d]\n", inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), g_events[i].last_active, i);
        int t_id = check_table2play();
        if (t_id == -1)
        {
            printf("当前没有空闲的桌子，请稍等..\n");
        }
        setUsr_ready(i, t_id, (Usr_t *)(g_events[i].buf));
        usr_id[t_id][((Usr_t *)(g_events[i].buf))->usr_id] = i;
        __check_table();
        printf("--->usr_id[%d][%d]: %d\n", t_id, ((Usr_t *)(g_events[i].buf))->usr_id, usr_id[t_id][((Usr_t *)(g_events[i].buf))->usr_id]);
        g_events[i].len = sizeof(Usr_t);                     // 为了让发送时候数据不被清零
        eventset(&g_events[i], cfd, senddata, &g_events[i]); // 找到合适的节点之后，先告诉用户，将其置于准备状态
        g_events[i].fd = cfd;
        eventadd(g_efd, EPOLLOUT, &g_events[i]);
    } while (0);

    // 每当有一位用户登录之后，检查一下是否可以开一桌游戏
    int table_id = check_tablegameStart(); //  发桌子阶段
    if (table_id)
    {
        TableFrame *tmp = init_table(table_id);
        for (int i = 0; i < 3; i++)
        {
            int only_id = usr_id[table_id][i];
            ((Usr_t *)(g_events[usr_id[table_id][i]].buf))->usr_turn = false; //  复位三位用户的回合标志
        }
        ((Usr_t *)(g_events[usr_id[table_id][tmp->m_wFirstUser]].buf))->usr_turn = true;
        for (int i = 0; i < 3; i++)
        { //  i就是usr_id
            int only_id = usr_id[table_id][i];
            memcpy(&((Usr_t *)(g_events[usr_id[table_id][i]].buf))->Table, tmp, sizeof(TableFrame)); //  将初始化完成的桌子复制到用户手上
            ((Usr_t *)(g_events[only_id].buf))->Stage = CALL_LANDLORD;
            g_events[only_id].len = sizeof(Usr_t);
            printf("现在给[%d]号桌的[%d]号用户发去桌子,该用户对应的全局变量的下标是:[%d]\n", table_id, only_id, usr_id[table_id][i]);
            eventdel(g_efd, &g_events[only_id]);                                                //  先把从原来的监听读事件的节点摘下来，再重新把写事件放上红黑树
            eventset(&g_events[only_id], g_events[only_id].fd, senddata, &g_events[only_id]);
            eventadd(g_efd, EPOLLOUT, &g_events[only_id]);
        }
    }
    return;
}

/*读取客户端发过来的数据的函数*/
void recvdata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = recv(fd, ev->buf, sizeof(ev->buf), 0); // 读取客户端发过来的数据
    // int t = ((Usr_t *)(ev->buf))->Table_id - 1;
    int t = ((Usr_t *)(ev->buf))->Table_id;
    int u = ((Usr_t *)(ev->buf))->usr_id;
    int o = usr_id[t][u];
    printf("g_events[%d] 发来了数据, 桌子号是[%d]\n", o, t);
    analyze_data(&g_events[o]);

    eventdel(g_efd, ev); // 将该节点从红黑树上摘除

    if (len > 0)
    {
        ev->len = len;
        // ev->buf[len] = '\0'; // 手动添加字符串结束标记
        // printf("接受数据 描述符:[%d]\n", fd);

        eventset(ev, fd, senddata, ev); // 设置该fd对应的回调函数为senddata
        eventadd(g_efd, EPOLLOUT, ev);  // 将fd加入红黑树g_efd中,监听其写事件
    }
    else if (len == 0)
    {
        close(ev->fd);
        check_usr_exit(ev);
        /* ev-g_events 地址相减得到偏移元素位置 */
        printf("[fd=%d] pos[%ld], closed\n", fd, ev - g_events);
    }
    else
    {
        close(ev->fd);
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
    }
    return;
}

/*发送给客户端数据*/
void senddata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    Usr_t *tmp = (Usr_t *)ev->buf;
    len = send(fd, ev->buf, sizeof(Usr_t), 0); // 直接将数据回射给客户端
    // printf("现在用户进入的阶段是:[%s]\n", stage[tmp->Stage]);
    // for(int i = 0; i < 3; i++){
    //     printf("ev->buf[%d]  发送过去的叫分是[%d]\n", i, ((Usr_t *)ev->buf)->Table.m_bScoreInfo[i]);
    // }
    eventdel(g_efd, ev); // 从红黑树g_efd中移除

    if (len > 0)
    {
        // printf("发送数据的描述符: [fd=%d], 数据长度 [%d]%s\n", fd, len, ev->buf);
        eventset(ev, fd, recvdata, ev); // 将该fd的回调函数改为recvdata
        eventadd(g_efd, EPOLLIN, ev);   // 重新添加到红黑树上，设为监听读事件
    }
    else
    {
        close(ev->fd); // 关闭链接
        printf("发送数据的描述符: [fd=%d] error %s\n", fd, strerror(errno));
    }
    return;
}

/*创建 socket, 初始化lfd */

void initlistensocket(int efd, short port)
{
    struct sockaddr_in sin;

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(lfd, F_SETFL, O_NONBLOCK); // 将socket设为非阻塞

    int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&sin, 0, sizeof(sin)); // bzero(&sin, sizeof(sin))
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    bind(lfd, (struct sockaddr *)&sin, sizeof(sin));

    listen(lfd, 20);

    /* void eventset(struct myevent_s *ev, int fd, void (*call_back)(int, int, void *), void *arg);  */
    eventset(&g_events[MAX_EVENTS], lfd, acceptconn, &g_events[MAX_EVENTS]);

    /* void eventadd(int efd, int events, struct myevent_s *ev) */
    eventadd(efd, EPOLLIN, &g_events[MAX_EVENTS]); // 将efd添加到监听树上，监听读事件

    return;
}

void *epoll_loop(void *arg)
{
    int port = SERV_PORT;

    g_efd = epoll_create(MAX_EVENTS + 1); // 创建红黑树,返回给全局 g_efd,实际上在linux 2.6.8中函数的参数已经被忽略，可以填任意值
    if (g_efd <= 0)
        printf("create efd in %s err %s\n", __func__, strerror(errno));

    initlistensocket(g_efd, port); // 初始化监听socket

    struct epoll_event events[MAX_EVENTS + 1]; // 定义这个结构体数组，用来接收epoll_wait传出的满足监听事件的fd结构体
    printf("server running:port[%d]\n", port);

    int checkpos = 0;
    int i;
    while (1)
    {
        // 调用eppoll_wait等待接入的客户端事件,epoll_wait传出的是满足监听条件的那些fd的struct epoll_event类型
        int nfd = epoll_wait(g_efd, events, MAX_EVENTS + 1, 1000); //  1000是等待的超时时间（毫秒）
        // printf("nfd:%d\n", nfd);
        if (nfd < 0)
        {
            printf("epoll_wait et\n");
            exit(-1);
        }
        for (i = 0; i < nfd; i++)
        {
            // evtAdd()函数中，添加到监听树中监听事件的时候将myevents_t结构体类型给了ptr指针
            // 这里epoll_wait返回的时候，同样会返回对应fd的myevents_t类型的指针
            struct myevent_s *ev = (struct myevent_s *)events[i].data.ptr;
            // 如果监听的是读事件，并返回的是读事件 -->EPOLLIN:0x01
            if ((events[i].events & EPOLLIN) && (ev->events & EPOLLIN))
            {
                // printf("[%d]用户触发了读事件\n", ((Usr_t *)(ev->buf))->usr_id);
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
            // 如果监听的是写事件，并返回的是写事件 -->EPOLLOUT:0x04
            if ((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT))
            {
                // printf("对[%d]用户触发了写事件\n", ((Usr_t *)(ev->buf))->usr_id);
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }
    }
    return NULL;
}
