#include "Organize_game.h"

extern int usr_id[MAX_TABLE][3];

char stage_name[4][20] = {"登录", "叫地主", "正在游戏", "结束"};

TableFrame *Table[MAX_TABLE]; // 定义桌子的全局变量，每个元素都是一桌，3人为一桌

int call_cnt = 0;
int finish_cnt = 0;

// 初始化桌子管理数组
void tableArr_Init()
{
    for (int i = 0; i < MAX_TABLE; i++)
    {
        Table[i] = calloc(1, sizeof(TableFrame)); //  初始化每一张桌子，每一张桌子都有独立的堆内存
        memset(Table[i], 0, sizeof(TableFrame));
    }
}

// 初始化桌子
TableFrame *init_table(int table_id)
{
    Table[table_id] = tableInit();
    OnEventGameStart(Table[table_id]);
    return Table[table_id];
}

// 设置用户为准备状态
void setUsr_ready(int usr_num, int t_id, Usr_t *usr)
{
    usr->Stage = LOGIN;                           //    初始登录上来的用户标志为登录状态，等待人数够了之后就改变状态
    usr->Table_id = t_id; //    表示每三个人一桌，按顺序入座
    usr->usr_id = Table[t_id]->m_wPlayerCount; //    每一位用户都用桌子id以及用户id唯一标识自己
    Table[usr->Table_id]->m_wPlayerCount++;
}

// 查看哪张桌子空闲可做
int check_table2play()
{
    for (int t_id = 1; t_id < MAX_TABLE; t_id++)
    {
        if (Table[t_id]->m_wPlayerCount != 3)
        {
            printf("目前空闲的桌子是[%d]:\n", t_id);
            return t_id;
        }
        else
        {
            continue;
        }
    }
    return -1;
}

// 每当有一位用户登录之后，检查一下是否可以开一桌游戏
int check_tablegameStart()
{
    for (int table_id = 1; table_id < MAX_TABLE; table_id++)
    {
        // printf("[%d]号桌子的人数是:[%d]\n", table_id, Table[table_id]->m_wPlayerCount);

        if (Table[table_id]->m_wPlayerCount == 3 && Table[table_id]->m_wBombTime == 0) //  用炸弹倍数来标识该桌已经开始了游戏
        {
            return table_id;
        }
        else
        {
            continue;
            // return 0;
        }
    }
    return 0;
}

// 检测用户断线退出
void check_usr_exit(struct myevent_s *usr)
{
    Usr_t *exit_usr = (Usr_t *)usr->buf;
    int t_id = exit_usr->Table_id;
    int u_id = exit_usr->usr_id;
    call_cnt = 0;
    finish_cnt = 0;
    Table[t_id]->m_wPlayerCount--;
    printf("当前退出的是[%d]号桌的[%d]用户,当前桌子还剩下%d名用户\n", t_id, u_id, Table[t_id]->m_wPlayerCount);
    //  如果t_id的桌子的人离线了，则清空桌子内容
    if (Table[t_id]->m_wPlayerCount == 0) //  如果该桌子人数已经清零之后，则直接清空全局桌子的数据
    {
        memset(Table[t_id], 0, sizeof(TableFrame)); //  此处不能直接清掉桌子的内容，因为会导致桌子的剩余人数异常
    }

    //  先预留一个接口，等客户端可以正常退出的时候，那么应该清掉的是用户的桌子内容
    // （因为从一开始将全局桌子拷贝到用户桌子之后，全局的桌子与用户的桌子的数据就开始不对等）
    memset(&exit_usr->Table, 0, sizeof(TableFrame));
    usr_id[t_id][u_id] = 0;
    for (int i = 0; i < 0; i++)
    {
        int only_id = usr_id[t_id][i];
        ((Usr_t *)(g_events[only_id].buf))->usr_turn = false;
    }
    // memset(&g_events[usr_id[t_id][u_id]], 0, sizeof(struct myevent_s));     //  将全局变量里面的内容清掉

    //  然后需要告诉其他用户，有人退出了,然后将其他用户的状态设置为LOGIN
    for (int i = 0; i < Table[t_id]->m_wPlayerCount; i++)
    {
        // int only_id = (t_id - 1) * 3 + i;
        int only_id = usr_id[t_id][i];
        // if (only_id == u_id)
        // {
        //     continue; //  跳过那位已经退出的用户
        // }
        memset(&((Usr_t *)(g_events[only_id].buf))->Table, 0, sizeof(TableFrame));
        memset(&((Usr_t *)(g_events[only_id].buf))->Stage, LOGIN, sizeof(usr_stage));
        g_events[only_id].len = sizeof(Usr_t);
        eventdel(g_efd, &g_events[only_id]);
        eventset(&g_events[only_id], g_events[only_id].fd, senddata, &g_events[only_id]);
        eventadd(g_efd, EPOLLOUT, &g_events[only_id]);
    }
}

// 同步三家用户数据并且发送数据
void copy_and_sendData(Usr_t *cur_usr)
{

    int t_id = cur_usr->Table_id;
    int n_id = cur_usr->usr_id + 1; //  标识下一位用户
    if (n_id == 3)
        n_id = 0;
    for (int i = 0; i < 3; i++)
    { //  复位三位用户的回合标志
        int only_id = usr_id[t_id][i];
        ((Usr_t *)(g_events[only_id].buf))->usr_turn = false;
    }
    ((Usr_t *)(g_events[usr_id[t_id][n_id]].buf))->usr_turn = true;
    ((Usr_t *)(g_events[usr_id[t_id][n_id]].buf))->Table.m_wCurrentUser = n_id;
    printf("当前玩家是[%d]号\n", n_id);
    for (int i = 0; i < 3; i++)
    { //  同步三家数据
        int only_id = usr_id[t_id][i];
        memcpy(&((Usr_t *)(g_events[only_id].buf))->Table, &cur_usr->Table, sizeof(TableFrame));
        memcpy(&((Usr_t *)(g_events[only_id].buf))->Stage, &cur_usr->Stage, sizeof(usr_stage));
    }
    for (int i = 0; i < 3; i++)
    {
        int only_id = usr_id[t_id][i];
        printf("现在给[%d]号桌的[%d]用户发数据,全局变量下标为[%d]\n", t_id, i, usr_id[t_id][i]);
        g_events[only_id].len = sizeof(Usr_t);
        eventdel(g_efd, &g_events[only_id]);
        eventset(&g_events[only_id], g_events[only_id].fd, senddata, &g_events[only_id]);
        eventadd(g_efd, EPOLLOUT, &g_events[only_id]);
    }
}

// 分析用户发回来的数据
void analyze_data(struct myevent_s *data)
{
    eventdel(g_efd, data);
    Usr_t *usr = (Usr_t *)data->buf;
    int t_id = usr->Table_id; //  桌子id
    int u_id = usr->usr_id;   //  用户id
    // for(int i = 0; i < 3; i++){
    //     printf("g_events[%d]号用户的分是[%d]分 \n", t_id+i, ((Usr_t *)(g_events[t_id+i].buf))->Table.m_bScoreInfo[i]);
    // }
    int n_id = 0;
    n_id = usr->usr_id + 1; //  标识下一位的用户
    if (n_id == 3)
        n_id = 0;
    // printf("[%d]号用户发送过来的状态是:[%s]%d\n", u_id, stage_name[usr->Stage], usr->Stage);
    switch (usr->Stage)
    {
    case CALL_LANDLORD:
        call_cnt++;

        if (usr->Table.m_bScoreInfo[0] == 255 && usr->Table.m_bScoreInfo[1] == 255 && usr->Table.m_bScoreInfo[2] == 255)
        {
            printf("没人叫地主，重启游戏\n");
            // RepositTableFrameSink(&Table[t_id]);
            memset(Table[usr->Table_id], 0, sizeof(TableFrame));
            Table[usr->Table_id] = init_table(usr->Table_id);
            usr->Stage = CALL_LANDLORD;
            call_cnt = 0;
            memcpy(&usr->Table, Table[usr->Table_id], sizeof(TableFrame));
            for (int i = 0; i < 3; i++)
            {
                usr->Table.m_bScoreInfo[i] == 0;
            }
            for (int i = 0; i < 3; i++)
            {
                // usr->usr_turn = false;
                ((Usr_t *)(g_events[usr_id[t_id][i]].buf))->usr_turn = false;
            }
            ((Usr_t *)(g_events[usr_id[t_id][Table[usr->Table_id]->m_wFirstUser]].buf))->usr_turn = true;
            ((Usr_t *)(g_events[usr_id[t_id][u_id]].buf))->Table.m_wCurrentUser = u_id;
            for (int i = 0; i < 3; i++)
            { //  同步三家数据
                int only_id = usr_id[t_id][i];
                memcpy(&((Usr_t *)(g_events[only_id].buf))->Table, Table[usr->Table_id], sizeof(TableFrame));
            }
            for (int i = 0; i < 3; i++)
            {
                int only_id = usr_id[t_id][i];
                g_events[only_id].len = sizeof(Usr_t);
                eventdel(g_efd, &g_events[only_id]);
                eventset(&g_events[only_id], g_events[only_id].fd, senddata, &g_events[only_id]);
                eventadd(g_efd, EPOLLOUT, &g_events[only_id]);
            }

            // copy_and_sendData(usr);
        }
        if (/* usr->Table.m_wBankerUser != -1 ||  */ call_cnt == 3 || (usr->Table.m_bLandScore == 3))
        {
            printf("出现地主，进入游戏阶段\n");
            usr->Stage += 1; // 叫的次数变成3的时候进入下一个阶段
        }
        copy_and_sendData(usr);
        break;

    case GAMING:
        // printf("现在到[%d]号用户的回合\n", t_id * 3 + n_id);

        if (usr->Table.m_bCardCount[u_id] == 0)
        {
            printf("游戏结束\n");
            usr->Stage += 1; // 叫的次数变成3的时候进入下一个阶段
        }
        copy_and_sendData(usr);
        break;

    case FINISH: //  结束游戏之后暂定重开
        if (((Usr_t *)(g_events[usr_id[t_id][0]].buf))->Table.m_bHandCardData[0][0] == 255 || ((Usr_t *)(g_events[usr_id[t_id][1]].buf))->Table.m_bHandCardData[1][0] == 255 || ((Usr_t *)(g_events[usr_id[t_id][2]].buf))->Table.m_bHandCardData[2][0] == 255)
        {
            if (finish_cnt == 0) //  判断第一位发送数据上来的用户(即赢家)，将分数拷贝到另外三家
            {
                for (int i = 0; i < 3; i++)
                { //  三个用户的数据同步
                    memcpy(&((Usr_t *)(g_events[usr_id[t_id][i]].buf))->Table, &usr->Table, sizeof(Usr_t));
                }
                for (int i = 0; i < 3; i++)
                {
                    // int only_id = (t_id)*3 + i;
                    int only_id = usr_id[t_id][i];
                    g_events[only_id].len = sizeof(Usr_t);
                    eventdel(g_efd, &g_events[only_id]);
                    eventset(&g_events[only_id], g_events[only_id].fd, senddata, &g_events[only_id]);
                    eventadd(g_efd, EPOLLOUT, &g_events[only_id]);
                }
            }
            finish_cnt++;
        }
        if (finish_cnt == 3)
        {
            printf("重启游戏\n");
            call_cnt = 0; //  标志位复位
            finish_cnt = 0;
            // free(Table[t_id]);
            memset(Table[t_id], 0, sizeof(TableFrame));
            Table[t_id] = init_table(t_id); //  复位桌子
            // OnEventGameStart(Table[t_id]); //  重启游戏
            memcpy(&usr->Table, Table[t_id], sizeof(TableFrame));

            usr->Stage = CALL_LANDLORD; //  复位三名用户到叫地主阶段
            for (int i = 0; i < 3; i++)
            { //  三个用户的数据同步
                int only_id = usr_id[t_id][i];
                memcpy(&((Usr_t *)(g_events[only_id].buf))->Table, &usr->Table, sizeof(Usr_t));
                memcpy(&((Usr_t *)(g_events[only_id].buf))->Stage, &usr->Stage, sizeof(usr_stage));
            }

            for (int i = 0; i < 3; i++)
            {
                int only_id = usr_id[t_id][i];
                ((Usr_t *)(g_events[only_id].buf))->usr_turn = false; //  复位三位用户的回合标志
            }
            int First_usr = Table[t_id]->m_wFirstUser;
            ((Usr_t *)(g_events[First_usr].buf))->usr_turn = true;
            ((Usr_t *)(g_events[usr_id[t_id][n_id]].buf))->Table.m_wCurrentUser = n_id;
            for (int i = 0; i < 3; i++)
            {
                // int only_id = (t_id)*3 + i;
                int only_id = usr_id[t_id][i];
                g_events[only_id].len = sizeof(Usr_t);
                eventdel(g_efd, &g_events[only_id]);
                eventset(&g_events[only_id], g_events[only_id].fd, senddata, &g_events[only_id]);
                eventadd(g_efd, EPOLLOUT, &g_events[only_id]);
            }
        }

        break;

    default:
        break;
    }
}
