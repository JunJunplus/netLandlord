#include "touch.h"

typedef struct block block_t;

int touchFd;
struct input_event inEv;
touchEvInfo_t evInfo;
touchPos_t firstPos;
sys_mode_t sys_mode;
char acc[20];
char passwd[20];

int num;
bool flag;

void touch_init()
{
    touchFd = open("/dev/input/event0", O_RDWR);
    if (touchFd == -1)
    {
        perror("open event0 fail");
        return;
    }
    // 初始化信息
    // ioctl(touchFd, FIONBIO);

    evInfo.endPos.x = evInfo.endPos.y = 0;
    evInfo.startPos.x = evInfo.startPos.y = 0;
    evInfo.startTime.tv_sec = evInfo.endTime.tv_sec = 0;
    evInfo.startTime.tv_nsec = evInfo.endTime.tv_nsec = 0;
    evInfo.evKind = NON;
    memset(acc, 0, 20);
    memset(passwd, 0, 20);
    num = 0;

    ptouch_arr = malloc(sizeof(touch_arr));
    ptouch_arr->arr_card = malloc(sizeof(uint8_t) * 20);
    ptouch_arr->count_card = 0;

    sys_mode = NON;
    flag = true;
    return;
}

void touch_destory()
{
    close(touchFd);
}

void start_press()
{
    // 无事件发生 && types:3 code:0 则存储开始x
    if (evInfo.evKind == NON && inEv.type == EV_ABS && inEv.code == ABS_X)
    {
        evInfo.startPos.x = inEv.value;
    }
    // 无事件发生 && types:3 code:1 则存储开始y
    if (evInfo.evKind == NON && inEv.type == EV_ABS && inEv.code == ABS_Y)
    {
        evInfo.startPos.y = inEv.value;
    }
    // 无事件发生 && types:1 code:330 value = 1 则按键被按下
    if (evInfo.evKind == NON && inEv.type == EV_KEY && inEv.code == BTN_TOUCH && inEv.value > 0)
    {
        evInfo.evKind = PRESS;
    }
}

void start_lossen()
{
    // 按键被按下 && types:3 code:0 value:326   则记录结束x
    if (evInfo.evKind == PRESS && inEv.type == EV_ABS && inEv.code == ABS_X)
    {
        evInfo.endPos.x = inEv.value;
    }
    // 按键被按下 && types:3 code:1 value:326   则记录结束y
    if (evInfo.evKind == PRESS && inEv.type == EV_ABS && inEv.code == ABS_Y)
    {
        evInfo.endPos.y = inEv.value;
    }
    // 按键被按下 && types:1 code:330 value:0    松开 记录结束时间
    if (evInfo.evKind == PRESS && inEv.type == EV_KEY && inEv.code == BTN_TOUCH && inEv.value == 0)
    {
        evInfo.evKind = LOOSEN;
    }
}

// 一次完整事件
bool complete_event()
{
    if (evInfo.evKind == LOOSEN)
    {
        if ((evInfo.endPos.x == 0 && evInfo.endPos.x == 0))
        {
            evInfo.evKind = CLICK;
            return true;
        }
    }
    return false;
}

int trigger_touch_event()
{
    evInfo.endPos.x = evInfo.endPos.y = 0;
    evInfo.startPos.x = evInfo.startPos.y = 0;
    evInfo.evKind = NON;
    while (1)
    {
        read(touchFd, &inEv, sizeof(inEv));
        start_press();
        start_lossen();
        if (complete_event())
        {
            break;
        }
    }
    return evInfo.evKind;
}

// void *touch_wait_longin(void *arg)
// {
//     touchPos_t pos;
//     while (sys_mode != SLOGIN)
//     {
//         struct input_event info; // 存储触摸屏的数据
//         printf("touc 272 line \n");

//         read(touchFd, &info, sizeof(info));

//         // 判断当前产生的时候触摸屏事件 ,进一步判断是X轴事件
//         if (info.type == EV_ABS && info.code == ABS_X)
//         {
//             pos.x = info.value;
//         }
//         // 判断当前产生的时候触摸屏事件 ,进一步判断是Y轴事件
//         if (info.type == EV_ABS && info.code == ABS_Y)
//         {
//             pos.y = info.value;
//         }
//         // 松开的时候
//         if (info.type == EV_KEY && info.code == BTN_TOUCH && info.value == 0)
//         {
//             // 黑色屏幕坐标范围： X  0-1024  Y  0-600
//             // 转换坐标范围
//             pos.x = (pos.x) * 800 * 1.0 / 1024;
//             pos.y = (pos.y) * 480 * 1.0 / 600;
//             // 蓝色屏幕坐标范围： X  0-800  Y  0-480
//             // printf("pos.x : %d pos.y : %d\n", pos.x, pos.y);
//             if (sys_mode == NON || sys_mode == SACC)
//             {
//                 char c = touch_login(pos);
//                 if (c != ' ')
//                 {
//                     if (num == 0)
//                     {
//                         acc_font(lcd_p, f, "", 70, 120);
//                     }
//                     if (c == 'c')
//                     {
//                         sys_mode = SPASSWD;
//                         num = 0;
//                         passwd_font(lcd_p, f, "", 70, 230);

//                         printf("%s\n", acc);

//                         return acc;
//                     }
//                     else if (c == 'd')
//                     {
//                         if (num > 0)
//                         {
//                             num--;
//                             acc[num] = '\0';
//                             acc_font(lcd_p, f, acc, 70, 120);
//                         }
//                     }
//                     else
//                     {
//                         acc[num] = c;
//                         acc_font(lcd_p, f, acc, 70, 120);
//                         num++;
//                     }
//                 }
//             }
//             else if (sys_mode == SPASSWD)
//             {
//                 char c = touch_login(pos);
//                 if (c != ' ')
//                 {
//                     if (c == 'c')
//                     {
//                         if (strcmp(acc, "123") == 0 && strcmp(passwd, "456") == 0)
//                         {
//                             sys_mode = SLOGIN;
//                         }
//                         else
//                         {
//                             num = 0;
//                             sys_mode = SACC;
//                             memset(acc, 0, 20);
//                             memset(passwd, 0, 20);
//                         }
//                     }
//                     else if (c == 'd')
//                     {
//                         if (num > 0)
//                         {
//                             num--;
//                             passwd[num] = '\0';
//                             passwd_font(lcd_p, f, passwd, 70, 230);
//                         }
//                     }
//                     else
//                     {
//                         passwd[num] = c;
//                         passwd_font(lcd_p, f, passwd, 70, 230);
//                         num++;
//                     }
//                 }
//             }
//         }
//     }
//     return acc;
// }

char choose_mode_touch()
{
    touchPos_t pos;
    struct input_event info; // 存储触摸屏的数据
    printf("touc 272 line \n");

    read(touchFd, &info, sizeof(info));

    // 判断当前产生的时候触摸屏事件 ,进一步判断是X轴事件
    if (info.type == EV_ABS && info.code == ABS_X)
    {
        pos.x = info.value;
    }
    // 判断当前产生的时候触摸屏事件 ,进一步判断是Y轴事件
    if (info.type == EV_ABS && info.code == ABS_Y)
    {
        pos.y = info.value;
    }
    // 松开的时候
    if (info.type == EV_KEY && info.code == BTN_TOUCH && info.value == 0)
    {
        // 黑色屏幕坐标范围： X  0-1024  Y  0-600
        // 转换坐标范围
        pos.x = (pos.x) * 800 * 1.0 / 1024;
        pos.y = (pos.y) * 480 * 1.0 / 600;
        // 蓝色屏幕坐标范围： X  0-800  Y  0-480
        printf("pos.x : %d pos.y : %d------\n", pos.x, pos.y);

        if (pos.x > 10 && pos.x < 110 && pos.y > 10 && pos.y < 110)
        {
            return '1';
        }
        else if (pos.x > 110 && pos.x < 250 && pos.y > 10 && pos.y < 110)
        {
            return '2';
        }
        else if (pos.x > 700 && pos.x < 800 && pos.y > 10 && pos.y < 110)
        {
            return '3';
        }
        // else if (pos.x > 250 && pos.x < 350 && pos.y > 10 && pos.y < 110)
        // {
        //     return '4';
        // }
    }
}

char touch_login(touchPos_t temppos)
{
    touchPos_t pos;
    pos.x = temppos.y;
    pos.y = temppos.x;

    if (pos.x > 100 && pos.x < 180 && pos.y > 420 && pos.y < 510)
    {
        return '1';
    }
    else if (pos.x > 100 && pos.x < 180 && pos.y > 520 && pos.y < 610)
    {
        return '2';
    }
    else if (pos.x > 100 && pos.x < 180 && pos.y > 630 && pos.y < 720)
    {
        return '3';
    }
    else if (pos.x > 210 && pos.x < 280 && pos.y > 420 && pos.y < 510)
    {
        return '4';
    }
    else if (pos.x > 210 && pos.x < 280 && pos.y > 520 && pos.y < 610)
    {
        return '5';
    }
    else if (pos.x > 210 && pos.x < 280 && pos.y > 630 && pos.y < 720)
    {
        return '6';
    }
    else if (pos.x > 300 && pos.x < 360 && pos.y > 420 && pos.y < 510)
    {
        return '7';
    }
    else if (pos.x > 300 && pos.x < 360 && pos.y > 520 && pos.y < 610)
    {
        return '8';
    }
    else if (pos.x > 300 && pos.x < 360 && pos.y > 630 && pos.y < 720)
    {
        return '9';
    }
    else if (pos.x > 380 && pos.x < 430 && pos.y > 430 && pos.y < 550)
    {
        return 'd';
    }
    else if (pos.x > 380 && pos.x < 430 && pos.y > 600 && pos.y < 730)
    {
        return 'c';
    }
    else
    {
        return ' ';
    }
}

void *touch_client_server(void *arg)
{
    touchPos_t pos;
    struct input_event info;
    while (1)
    {
        read(touchFd, &info, sizeof(info));

        // 判断当前产生的时候触摸屏事件 ,进一步判断是X轴事件
        if (info.type == EV_ABS && info.code == ABS_X)
        {
            pos.x = info.value;
        }
        // 判断当前产生的时候触摸屏事件 ,进一步判断是Y轴事件
        if (info.type == EV_ABS && info.code == ABS_Y)
        {
            pos.y = info.value;
        }
        // 松开的时候
        if (info.type == EV_KEY && info.code == BTN_TOUCH && info.value == 0)
        {
            pos.x = (pos.x) * 800 * 1.0 / 1024;
            pos.y = (pos.y) * 480 * 1.0 / 600;
            // 蓝色屏幕坐标范围： X  0-800  Y  0-480
            printf("pos.x : %d pos.y : %d------client_server\n", pos.x, pos.y);
            if (pos.x > 300 && pos.x < 500 && pos.y > 200 && pos.y < 283)
            {
                if (init_tcp() == 1)
                {
                    start_backstage_secv();
                    break;
                }
            }
        }
    }
}

void *touch_land_score(void *arg)
{
    touchPos_t pos;
    struct input_event info;
    while (((Usr_t *)arg)->Stage == CALL_LANDLORD)
    {
        read(touchFd, &info, sizeof(info));

        // 判断当前产生的时候触摸屏事件 ,进一步判断是X轴事件
        if (info.type == EV_ABS && info.code == ABS_X)
        {
            pos.x = info.value;
        }
        // 判断当前产生的时候触摸屏事件 ,进一步判断是Y轴事件
        if (info.type == EV_ABS && info.code == ABS_Y)
        {
            pos.y = info.value;
        }
        // 松开的时候
        if (info.type == EV_KEY && info.code == BTN_TOUCH && info.value == 0)
        {
            pos.x = (pos.x) * 800 * 1.0 / 1024;
            pos.y = (pos.y) * 480 * 1.0 / 600;
            // 蓝色屏幕坐标范围： X  0-800  Y  0-480
            printf("pos.x : %d pos.y : %d------land_score\n", pos.x, pos.y);
            int score = -1;
            int maxScore = ((Usr_t *)arg)->Table.m_bLandScore;

            printf("maxScore : %d\n", maxScore);
            if (pos.x > 140 && pos.x < 240 && pos.y > 200 && pos.y < 249 && maxScore == 0)
            {
                score = 1;
            }
            else if (pos.x > 250 && pos.x < 350 && pos.y > 200 && pos.y < 249 && maxScore <= 1)
            {
                score = 2;
            }
            else if (pos.x > 360 && pos.x < 460 && pos.y > 200 && pos.y < 249 && maxScore <= 2)
            {
                score = 3;
            }
            else if (pos.x > 550 && pos.x < 650 && pos.y > 200 && pos.y < 249)
            {
                score = 255;
            }
            if (score != -1)
            {
                OnUserLandScore(&((Usr_t *)arg)->Table, ((Usr_t *)arg)->usr_id, score);

                send_message((Usr_t *)arg);
                return NULL;
            }
        }
    }
}

void *touch_select_card(void *arg)
{
    touchPos_t pos;
    touchPos_t endPos;
    struct input_event info; // 存储触摸屏的数据
    int mode = 0;
    bool flagx = true;
    bool flagy = true;
    while (1)
    {

        read(touchFd, &info, sizeof(info));
        int touchArr[20] = {-1};
        int mid = g_usr->Table.m_bCardCount[g_usr->usr_id] / 2;
        int start = CARD_MID - mid * LEAVE_WIDTH;
        int width = g_usr->Table.m_bCardCount[g_usr->usr_id] * LEAVE_WIDTH;

        // 判断当前产生的时候触摸屏事件 ,进一步判断是X轴事件
        if (info.type == EV_ABS && info.code == ABS_X)
        {
            if (flagx)
            {
                pos.x = info.value;
                flagx = false;
            }
            endPos.x = info.value;
        }
        // 判断当前产生的时候触摸屏事件 ,进一步判断是Y轴事件
        if (info.type == EV_ABS && info.code == ABS_Y)
        {
            if (flagy)
            {
                pos.y = info.value;
                flagy = false;
            }
            endPos.y = info.value;
        }
        // 松开的时候
        if (info.type == EV_KEY && info.code == BTN_TOUCH && info.value == 0)
        {
            // 黑色屏幕坐标范围： X  0-1024  Y  0-600
            // 转换坐标范围
            pos.x = (pos.x) * 800 * 1.0 / 1024;
            pos.y = (pos.y) * 480 * 1.0 / 600;
            endPos.x = endPos.x * 800 * 1.0 / 1024;
            endPos.y = endPos.y * 480 * 1.0 / 600;
            // 蓝色屏幕坐标范围： X  0-800  Y  0-480
            printf("pos.x : %d pos.y : %d------select_card\n", pos.x, pos.y);

            if (pos.x >= start && pos.x <= start + width && pos.y > CARD_HEIGHT && pos.y < CARD_HEIGHT + 179 && endPos.x >= start && endPos.x <= start + width && endPos.y > CARD_HEIGHT && endPos.y < CARD_HEIGHT + 179)
            {
                int start_card, end_card;
                if ((pos.x - start) / LEAVE_WIDTH >= 0 && (pos.x - start) / LEAVE_WIDTH <= g_usr->Table.m_bCardCount[g_usr->usr_id])
                {
                    start_card = (pos.x - start) / LEAVE_WIDTH;
                }
                if ((endPos.x - start) / LEAVE_WIDTH >= 0 && (endPos.x - start) / LEAVE_WIDTH <= g_usr->Table.m_bCardCount[g_usr->usr_id])
                {
                    end_card = (endPos.x - start) / LEAVE_WIDTH;
                }

                if (start_card > end_card)
                {
                    int temp = end_card;
                    end_card = start_card;
                    start_card = temp;
                }
                ptouch_arr->count_card = end_card - start_card + 1;
                for (int i = 0; i < ptouch_arr->count_card; i++)
                {
                    ptouch_arr->arr_card[i] = i + start_card;
                }
                // printf("min[%d]--max[%d]\n", start_card, end_card);
                // printf("min[%d]--max[%d]------------\n", ptouch_arr->arr_card[0], ptouch_arr->arr_card[ptouch_arr->count_card - 1]);
                return &ptouch_arr[0];
            }
            if (endPos.x > 250 && endPos.x < 350 && endPos.y > 200 && endPos.y < 260)
            {
                num = 30;
                return &num;
            }
            if (endPos.x > 550 && endPos.x < 650 && endPos.y > 200 && endPos.y < 260)
            {
                num = 31;

                return &num;
            }
            if (endPos.x > 750 && endPos.x < 800 && endPos.y > 0 && endPos.y < 50)
            {
                num = 32;

                return &num;
            }
            flagx = true;
            flagy = true;
        }
    }
}