#include "tcp.h"

void *send_message(Usr_t *usr)
{
    int len = 0;

    while (1)
    {
        len = send(cliFd, usr, sizeof(Usr_t), 0);
        if (len == -1)
        {
            perror("send error");
            return NULL;
        }
        if (len > 0)
        {
            printf("向服务器发送len : %d\n", len);
            return NULL;
        }
    }
}

int init_tcp()
{
    cliFd = socket(AF_INET, SOCK_STREAM, 0);
    if (cliFd == -1)
    {
        perror("socket error");
        return -1;
    }

    struct sockaddr_in sAddr;
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = inet_addr("122.152.226.122");
    sAddr.sin_port = htons(6666);

    if (connect(cliFd, (struct sockaddr *)&sAddr, sizeof(sAddr)) == -1)
    {
        perror("connect error");
        return -1;
    }
    return 1;
}

void start_backstage_secv()
{
    pthread_t tid2;

    pthread_create(&tid2, NULL, recv_message, g_usr);
    pthread_detach(tid2);
}

void *recv_message(void *arg)
{
    int recv_cnt = 0;

    Usr_t *usr = malloc(sizeof(Usr_t));

    while (1)
    {
        recv_cnt = recv(cliFd, usr, sizeof(Usr_t), 0);
        if (recv_cnt == -1)
        {
            perror("recv error");
            return NULL;
        }
        if (recv_cnt == 0)
        {
            printf("对方已经退出连接\n");
            sleep(1);

            // break;
        }
        if (usr->Stage == LOGIN)
        {
            printf("等待服务器发牌\n");
        }
        if (recv_cnt > 0)
        {
            memcpy(g_usr, usr, sizeof(Usr_t));
            if (g_usr->Stage == LOGIN)
            {
                // g_usr->Stage = CALL_LANDLORD;
                // // g_usr->usr_turn = true;
                // show_table(g_usr);
            }
            printf("我是玩家:[%d] 当前玩家:[%d]\n", g_usr->usr_id, g_usr->Table.m_wCurrentUser );
            ////////////show_other
            show_table(g_usr);
            // {
            //     if (g_usr->Table.m_wCurrentUser == g_usr->usr_id)
            //     {
            //         bmp_pox_to_palette("71.bmp", 250, 200);
            //         bmp_pox_to_palette("70.bmp", 550, 200);
            //     }
            //     out_card_to_palette(usr->Table.m_bTurnCardData, usr->Table.m_bTurnCardCount);

            //     other_card_to_palette(usr->Table.m_bCardCount[(usr->usr_id + 1) % 3], 1);
            //     other_card_to_palette(usr->Table.m_bCardCount[(usr->usr_id + 2) % 3], 0);

            //     palette_to_screen();
            // }
            printf("收到服务器发来的信息 状态为 ： %d\n", g_usr->Stage);
            if (g_usr->Stage == FINISH)
            {
                OnEventGameEnd(&g_usr->Table, g_usr->Table_id, 0);
            }

            if(g_usr->Stage == GAMING )
            {
                timer_count_down();
            }
        }
    }

    close(cliFd);

    return NULL;
}