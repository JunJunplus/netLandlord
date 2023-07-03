#include "lcd.h"
#include "bmp.h"
#include "card.h"
#include "table.h"
#include "touch.h"
#include "tcp.h"
#include <unistd.h>
#include <time.h>

void user_card(TableFrame *table, int userid)
{
    printf("用户%d: ", userid);

    for (int i = 0; i < table->m_bCardCount[userid]; i++)
    {
        printf("%d\t", GetCardValue(table->m_bHandCardData[userid][i]));
    }
    printf("\n");
    printf("用户%d: ", userid);

    for (int i = 0; i < table->m_bCardCount[userid]; i++)
    {
        printf("%d\t", table->m_bHandCardData[userid][i]);
    }
    printf("\n");
}

void up_arr_to_hand_card(uint8_t *upCard, int wChairID, uint8_t m_bHandCardData[3][20], uint8_t *bCard, uint8_t *pcount)
{
    uint8_t count = 0;
    for (int i = 0; i < 20; i++)
    {
        if (upCard[i])
        {
            bCard[count] = m_bHandCardData[wChairID][i];
            count++;
        }
    }
    *pcount = count;
}

int main()
{
    g_usr = malloc(sizeof(Usr_t));
    g_usr->Stage = LOGIN;
    lcd_init();
    init_bmp();
    init_palette();

    touch_init();
    srand((unsigned int)time(NULL));

    show_table(g_usr);
LOGIN:
    //////
    touch_client_server(g_usr);

//     // 等待服务器发牌
LABLE:
    while (g_usr->Stage != CALL_LANDLORD)
    {
        printf("--Stage : %d\n", g_usr->Stage);
        usleep(2000 * 100);
    }

    // 叫分
    while (1)
    {
        show_table(g_usr);
        printf("叫分中\n");
        if (g_usr->usr_turn)
        {
            usleep(2000 * 100);
            show_table(g_usr);
            touch_land_score(g_usr);
            break;
        }
        usleep(2000 * 200);
    }

    usleep(2000 * 1000);

    if (g_usr->Stage == CALL_LANDLORD)
    {
        printf("重新叫分\n");
        goto LABLE;
    }

    g_usr->usr_turn = false;
    show_table(g_usr);

    // 等待服务器回复开始游戏
    while (g_usr->Stage != GAMING)
    {
        printf("等待服务器开始游戏--：%d\n", g_usr->Stage);
        usleep(2000 * 100);
    }

    /////////////////////////////////////////////////////////////////////
    // TableFrame *table = tableInit();
    // OnEventGameStart(table);

    // memcpy(&g_usr->Table, table, sizeof(TableFrame));
    // g_usr->usr_id = table->m_wFirstUser;
    /////////////////////////////////////////////////////////////////////

    show_table(g_usr);

    printf("end\n");
OUTCARD:

    while (1)
    {
        int *card = touch_select_card(g_usr);
        uint8_t bCard[20];
        uint8_t count = 0;
        if (g_usr->Stage == FINISH)
        {
            break;
        }
        // if (*card >= 0 && *card < g_usr->Table.m_bCardCount[g_usr->usr_id])
        if (ptouch_arr->count_card > 0 && ptouch_arr->count_card <= g_usr->Table.m_bCardCount[g_usr->usr_id])
        {
            // card_on_click(g_usr->usr_id, g_usr->Table.m_bHandCardData, g_usr->Table.m_bCardCount[g_usr->usr_id], *card % g_usr->Table.m_bCardCount[g_usr->usr_id]);
            card_on_click_slide(g_usr->usr_id, g_usr->Table.m_bHandCardData, g_usr->Table.m_bCardCount[g_usr->usr_id], 0);
            printf("选牌\n");
        }
        else
        {
            printf("出牌\n");
            printf("*card--[%d]  count--[%d]\n", *card, count);
            if (g_usr->usr_id == g_usr->Table.m_wCurrentUser)
            {
                if (*card == 31)
                {
                    up_arr_to_hand_card(m_bHandCardUp, g_usr->usr_id, g_usr->Table.m_bHandCardData, bCard, &count);
                    //////////////////////////////////////////////////////////////////////////////////////////

                    while (!OnUserOutCard(&g_usr->Table, g_usr->usr_id, bCard, count))
                    {
                        // user = (user + 1) % 3;
                        // break;
                        goto OUTCARD;
                    }
                }
                else if (*card == 30)
                {
                    printf("不出\n");
                    while (!OnUserPassCard(&g_usr->Table, g_usr->usr_id))
                    {
                        // user = (user + 1) % 3;
                        /// break;
                        goto OUTCARD;
                    }
                    count = 1;
                }
                else if (*card == 32)
                {
                    g_usr->Table.m_bHandCardData[g_usr->usr_id][0] = 255;
                    send_message(g_usr);
                    goto LOGIN;
                }

                memset(m_bHandCardUp, 0, sizeof(m_bHandCardUp));
                // show_table(g_usr);
                printf("count [%d]\n", count);
                if (count != 0)
                {
                    timer_count_on_off(0);
                    send_message(g_usr);
                    count = 0;
                }

                if (g_usr->Table.m_bCardCount[g_usr->usr_id] == 0)
                {
                    break;
                }
            }
        }
    }
    // OnEventGameEnd(&g_usr->Table, g_usr->usr_id, 0);
    g_usr->Table.m_bHandCardData[g_usr->usr_id][0] = 255;
    send_message(g_usr);

    while (g_usr->Stage != CALL_LANDLORD)
    {
        printf("阶段：%d\n", g_usr->Stage);
        usleep(2000 * 200);
    }

    goto LABLE;

    pause();
    //     OnEventGameStart(table);
    //     uint8_t bCardData[20] = {0};
    //     char buf[40] = {0};
    //     uint8_t bCardCount = 0;

    //     int user = table->m_wFirstUser;
    //     uint8_t sorce = 0;

    //     my_card_to_palette(user, table->m_bHandCardData, table->m_bCardCount[user]);
    //     palette_to_screen();
    //     bmp_show_size("71.bmp", 150, 210);
    //     bmp_show_size("70.bmp", 550, 210);

    //     printf("用户%d叫分：", user);
    //     scanf("%hhd", &sorce);

    //     while (OnUserLandScore(table, user, sorce))
    //     {
    //         user++;
    //         printf("用户%d叫分：", user % 3);
    //         scanf("%hhd", &sorce);
    //     }

    //     bmp_show_size("71.bmp", 150, 210);
    //     bmp_show_size("70.bmp", 550, 210);
    //     while (1)
    //     {
    //         printf("用户%d出牌\n", user);

    //         int card = touch_select_card(table->m_bHandCardData[user], table->m_bCardCount[user]);
    //         if (card > 0 && card < table->m_bCardCount[user])
    //         {
    //             card_on_click(user, table->m_bHandCardData, table->m_bCardCount[user], card % table->m_bCardCount[user]);
    //         }
    //         else
    //         {
    //             if (card == 31)
    //             {
    //                 uint8_t bCard[20];
    //                 uint8_t count = 0;
    //                 up_arr_to_hand_card(m_bHandCardUp, user, table->m_bHandCardData, bCard, &count);
    // //////////////////////////////////////////////////////////////////////////////////////////
    //                 while (!OnUserOutCard(table, user, bCard, count))
    //                 {
    //                     //user = (user + 1) % 3;
    //                     break;
    //                 }
    //             }
    //             else if (card == 30)
    //             {
    //                 if (OnUserPassCard(table, user))
    //                 {
    //                     //user = (user + 1) % 3;
    //                 }
    //             }
    //         }
    //         //memset(m_bHandCardUp, 0, 20 * sizeof(uint8_t));
    //     }

    //     printf("用户%d叫分：", user);
    //     scanf("%hhd", &sorce);

    //     while (OnUserLandScore(table, user, sorce))
    //     {
    //         user++;
    //         printf("用户%d叫分：", user % 3);
    //         scanf("%hhd", &sorce);
    //     }

    //     int count = table->m_wBankerUser;
    //     while (1)
    //     {
    //         user_card(table, count);
    //         do
    //         {
    //             bCardCount = 0;
    //             memset(buf, 0, 40);
    //             printf("玩家%d出牌：", count);
    //             scanf("%s", buf);
    //             slice_str_blank(buf, bCardData, &bCardCount);
    //             if (bCardData[0] == 0)
    //             {
    //                 OnUserPassCard(table, count);
    //                 break;
    //             }
    //         } while (!OnUserOutCard(table, count, bCardData, bCardCount));
    //         if (table->m_bCardCount[count] == 0)
    //         {
    //             OnEventGameEnd(table, count, GER_NORMAL);
    //             break;
    //         }
    //         count = (count + 1) % 3;
    //     }

    pause();

    return 0;
}