#include "bmp.h"
#include "lcd.h"

int *palette;

int bmp_show(const char *fileName)
{
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    // printf("图片的宽度:%d 高度:%d 大小:%d 位深:%d\n",infoHeader.biWidth,
    //                                                infoHeader.biHeight,
    //                                              infoHeader.biSizeImage+54,
    //                                             infoHeader.biBitCount);

    // 3、读取图片的数据
    char bmpbuf[800 * 480 * 3] = {0};
    read(bmpFd, bmpbuf, 800 * 480 * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[800 * 480] = {0};
    for (int i = 0, j = 0; i < 800 * 480; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    // int lcdbuf[800*480] = {0};
    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            lcd_p[800 * j + i] = tempbuf[(479 - j) * 800 + i];
        }
    }

    // memcpy(lcd_p,lcdbuf,800*480*4);
    //  if (!lcd_flag)
    //  {
    //      lcd_cache_change();
    //  }

    close(bmpFd);

    return 0;
}

int bmp_show_another(const char *fileName)
{
    int *lcd_p2 = lcd_p;
    if (lcd_flag)
    {
        lcd_p2 += 800 * 480;
    }
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    // printf("图片的宽度:%d 高度:%d 大小:%d 位深:%d\n",infoHeader.biWidth,
    //                                                infoHeader.biHeight,
    //                                              infoHeader.biSizeImage+54,
    //                                             infoHeader.biBitCount);

    // 3、读取图片的数据
    char bmpbuf[800 * 480 * 3] = {0};
    read(bmpFd, bmpbuf, 800 * 480 * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[800 * 480] = {0};
    for (int i = 0, j = 0; i < 800 * 480; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    // int lcdbuf[800*480] = {0};
    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            lcd_p2[800 * j + i] = tempbuf[(479 - j) * 800 + i];
        }
    }

    // memcpy(lcd_p,lcdbuf,800*480*4);

    close(bmpFd);
    lcd_cache_change();
    return 0;
}

int bmp_show_swap(const char *fileName)
{
    if (lcd_flag)
    {
        bmp_show_another(fileName);
    }
    else
    {
        bmp_show(fileName);
    }
}

// 在指定位置上显示指定大小（<=800 ）的图片
int bmp_show_size(const char *fileName, int posX, int posY)
{
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;

    // 3、读取图片的数据
    char bmpbuf[width * height * 3];
    read(bmpFd, bmpbuf, width * height * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[width * height];
    for (int i = 0, j = 0; i <= width * height; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    int lcdbuf[width * height];
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            lcdbuf[width * j + i] = tempbuf[(height - 1 - j) * width + i];
        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            lcd_p[posY * 800 + posX + 800 * j + i] = lcdbuf[j * width + i];
        }
    }

    close(bmpFd);

    return 0;
}

int bmp_show_double(int (*lcdbuf)[800 * 480], int x, int i)
{
    int *lcd_temp = lcd_p;

    if (i == 0)
    {
        for (int j = 0; j < 480; j++)
        {
            for (int i = 0; i < x; i++)
            {
                lcd_temp[800 * j + i] = lcdbuf[0][800 * j + i + 799 - x];
                // lcd_p[800 * j + i] = 0x0;
            }
        }
        for (int j = 0; j < 480; j++)
        {
            for (int i = x; i < 800; i++)
            {
                lcd_temp[800 * j + i] = lcdbuf[1][800 * j + i - x];
            }
        }
    }
    else
    {

        for (int j = 0; j < 480; j++)
        {
            for (int i = x; i < 800; i++)
            {
                lcd_temp[800 * j + i] = lcdbuf[0][800 * j + i - x];
            }
        }
        for (int j = 0; j < 480; j++)
        {
            for (int i = 0; i < x; i++)
            {
                lcd_temp[800 * j + i] = lcdbuf[2][800 * j + i + 799 - x];
                // lcd_p[800 * j + i] = 0x0;
            }
        }
    }
    // lcd_cache_change();
}

/////////////////////////////////////////////////////////////////////////////////////

void init_bmp()
{
    memset(m_bHandCardUp, 0, 20 * sizeof(uint8_t));
    palette = (int *)malloc(800 * 480 * 4);
    black_bmp = (int *)malloc(800 * 480 * 4);
    back_bmp_to_buf("200.bmp");
}

/**
 * @brief bmp图片输出到画板的指定位置
 * @param fileName
 * @param posX
 * @param posY
 * @return
 */
int bmp_pox_to_palette(const char *fileName, int posX, int posY)
{
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;

    // 3、读取图片的数据
    char bmpbuf[width * height * 3];
    read(bmpFd, bmpbuf, width * height * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[width * height];
    for (int i = 0, j = 0; i <= width * height; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    int lcdbuf[width * height];
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            lcdbuf[width * j + i] = tempbuf[(height - 1 - j) * width + i];
        }
    }
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            palette[posY * 800 + posX + 800 * j + i] = lcdbuf[j * width + i];
        }
    }
    close(bmpFd);

    return 0;
}

int bmp_pox_to_palette_with_part(const char *fileName, int posX, int posY, int width, int height)
{
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    // 3、读取图片的数据
    char bmpbuf[width * height * 3];
    read(bmpFd, bmpbuf, width * height * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[width * height];
    for (int i = 0, j = 0; i <= width * height; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    int lcdbuf[width * height];
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            lcdbuf[width * j + i] = tempbuf[(height - 1 - j) * width + i];
        }
    }
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            palette[posY * 800 + posX + 800 * j + i] = lcdbuf[j * width + i];
        }
    }
    close(bmpFd);

    return 0;
}

int bmp_scale_to_palette(const char *fileName, int posX, int posY, int scale)
{
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;

    // 3、读取图片的数据
    char bmpbuf[width * height * 3];
    read(bmpFd, bmpbuf, width * height * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[width * height];
    for (int i = 0, j = 0; i <= width * height; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    int lcdbuf[width * height];
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            lcdbuf[width * j + i] = tempbuf[(height - 1 - j) * width + i];
        }
    }

    // 将图片进行压缩16倍
    int minbuf[(width / scale) * (height / scale)];
    // int minbuf[200*120];
    for (int j = 0; j < height / scale; j++)
    {
        for (int i = 0; i < (width / scale); i++)
        {
            minbuf[(j * (width / scale) + i)] = lcdbuf[(j * width * scale + i * scale)];
        }
    }

    for (int j = 0; j < (height / scale); j++)
    {
        for (int i = 0; i < (width / scale); i++)
        {
            palette[posY * 800 + posX + j * 800 + i] = minbuf[j * (width / scale) + i];
        }
    }

    close(bmpFd);
}

/**
 * @brief 画板画到屏幕
 * @return
 */
int palette_to_screen()
{
    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            lcd_p[800 * j + i] = palette[800 * j + i];
        }
    }
}

/**
 * @brief 初始化画板背景
 */
void init_palette()
{
    bmp_pox_to_palette("200.bmp", 0, 0);
}

/**
 * @brief 用户手牌画到画板
 * @param wChairID
 * @param m_bHandCardData
 * @param bCardCount
 */
void my_card_to_palette(int wChairID, uint8_t m_bHandCardData[3][20], uint8_t bCardCount)
{
    int mid = bCardCount / 2;
    int start = CARD_MID - mid * LEAVE_WIDTH;
    for (int i = 0; i < bCardCount; i++)
    {
        char cardBmpName[10] = {0};
        sprintf(cardBmpName, "%d.bmp", m_bHandCardData[wChairID][i]);
        bmp_pox_to_palette(cardBmpName, start + i * LEAVE_WIDTH, CARD_HEIGHT);
    }
}

/**
 * @brief 用户点击牌弹起或落下
 * @param wChairID
 * @param m_bHandCardData
 * @param bCardCount
 * @param cardId
 */
void card_on_click(int wChairID, uint8_t m_bHandCardData[3][20], uint8_t bCardCount, uint8_t cardId)
{

    int mid = bCardCount / 2;
    int start = CARD_MID - mid * LEAVE_WIDTH;
    for (int i = 0; i < 20; i += 2)
    {
        for (int j = 0; j < bCardCount; j++)
        {
            char cardBmpName[10] = {0};
            sprintf(cardBmpName, "%d.bmp", m_bHandCardData[wChairID][j]);
            if (j == cardId)
            {
                if (m_bHandCardUp[cardId] == 0)
                {
                    if (j == 0 || m_bHandCardUp[j - 1])
                    {
                        back_part_to_palette(start + j * LEAVE_WIDTH, CARD_HEIGHT - i + 149, 100);
                        back_part_to_palette(start + j * LEAVE_WIDTH, CARD_HEIGHT - i + 149 + 1, 100);
                    }
                    else
                    {
                        back_part_to_palette(start + j * LEAVE_WIDTH + 100 - LEAVE_WIDTH, CARD_HEIGHT - i + 149, LEAVE_WIDTH);
                        back_part_to_palette(start + j * LEAVE_WIDTH + 100 - LEAVE_WIDTH, CARD_HEIGHT - i + 149 + 1, LEAVE_WIDTH);
                    }

                    bmp_pox_to_palette(cardBmpName, start + j * LEAVE_WIDTH, CARD_HEIGHT - i);
                }
                else
                {
                    if (j != 0 && m_bHandCardUp[j - 1])
                    {
                        back_part_to_palette(start + j * LEAVE_WIDTH + 100 - LEAVE_WIDTH, 280 + i - 1, LEAVE_WIDTH);
                        back_part_to_palette(start + j * LEAVE_WIDTH + 100 - LEAVE_WIDTH, 280 + i - 2, LEAVE_WIDTH);
                    }
                    else
                    {
                        back_part_to_palette(start + j * LEAVE_WIDTH, 280 + i - 1, 100);
                        back_part_to_palette(start + j * LEAVE_WIDTH, 280 + i - 2, 100);
                    }

                    bmp_pox_to_palette(cardBmpName, start + j * LEAVE_WIDTH, 280 + i);
                }
            }
            else
            {
                if (m_bHandCardUp[j])
                {
                    bmp_pox_to_palette(cardBmpName, start + j * LEAVE_WIDTH, CARD_HEIGHT - 20);
                }
                else
                {
                    bmp_pox_to_palette(cardBmpName, start + j * LEAVE_WIDTH, CARD_HEIGHT);
                }
            }
        }
        palette_to_screen();
    }
    if (m_bHandCardUp[cardId])
        m_bHandCardUp[cardId] = 0;
    else
        m_bHandCardUp[cardId] = 1;
}

void card_on_click_slide(int wChairID, uint8_t m_bHandCardData[3][20], uint8_t bCardCount, uint8_t cardId)
{
    int mid = bCardCount / 2;
    int start = CARD_MID - mid * LEAVE_WIDTH;
    int width = bCardCount * LEAVE_WIDTH + 120 - LEAVE_WIDTH;
    for (int i = 0; i <= 20; i += 2)
    {
        for (int k = CARD_HEIGHT - 20 + i - 2; k <= CARD_HEIGHT - i + 179 + 1; k++)
        {
            back_part_to_palette(start, k, width);
        }
        for (int j = 0; j < bCardCount; j++)
        {
            char cardBmpName[10] = {0};
            sprintf(cardBmpName, "%d.bmp", m_bHandCardData[wChairID][j]);
            if (j >= ptouch_arr->arr_card[0] && j <= ptouch_arr->arr_card[ptouch_arr->count_card - 1])
            {
                if (m_bHandCardUp[j] == 0)
                {
                    if (j == 0 || (j > 1 && m_bHandCardUp[j - 1]))
                    {
                        // back_part_to_palette(start + j * LEAVE_WIDTH, CARD_HEIGHT - i + 179, 120);
                        // back_part_to_palette(start + j * LEAVE_WIDTH, CARD_HEIGHT - i + 179 + 1, 120);
                    }
                    else
                    {
                        // back_part_to_palette(start + j * LEAVE_WIDTH + 120 - LEAVE_WIDTH, CARD_HEIGHT - i + 179, LEAVE_WIDTH);
                        // back_part_to_palette(start + j * LEAVE_WIDTH + 120 - LEAVE_WIDTH, CARD_HEIGHT - i + 179 + 1, LEAVE_WIDTH);
                    }

                    bmp_pox_to_palette(cardBmpName, start + j * LEAVE_WIDTH, CARD_HEIGHT - i);
                }
                else
                {
                    if (j != 0 && (j > 1 && m_bHandCardUp[j - 1]))
                    {
                        // back_part_to_palette(start + j * LEAVE_WIDTH + 120 - LEAVE_WIDTH, CARD_HEIGHT - 20 + i - 1, LEAVE_WIDTH);
                        // back_part_to_palette(start + j * LEAVE_WIDTH + 120 - LEAVE_WIDTH, CARD_HEIGHT - 20 + i - 2, LEAVE_WIDTH);
                    }
                    else
                    {
                        // back_part_to_palette(start + j * LEAVE_WIDTH, CARD_HEIGHT - 20 + i - 1, 120);
                        // back_part_to_palette(start + j * LEAVE_WIDTH, CARD_HEIGHT - 20 + i - 2, 120);
                    }

                    bmp_pox_to_palette(cardBmpName, start + j * LEAVE_WIDTH, CARD_HEIGHT - 20 + i);
                }
            }
            else
            {
                if (m_bHandCardUp[j])
                {
                    bmp_pox_to_palette(cardBmpName, start + j * LEAVE_WIDTH, CARD_HEIGHT - 20);
                }
                else
                {
                    bmp_pox_to_palette(cardBmpName, start + j * LEAVE_WIDTH, CARD_HEIGHT);
                }
            }
        }
        palette_to_screen();
    }
    for (int j = ptouch_arr->arr_card[0]; j <= ptouch_arr->arr_card[ptouch_arr->count_card - 1]; j++)
    {
        if (m_bHandCardUp[j])
            m_bHandCardUp[j] = 0;
        else
            m_bHandCardUp[j] = 1;
    }
    ptouch_arr->count_card = -1;
}

/**
 * @brief 背景图片到buf
 * @param fileName
 * @return
 */
int back_bmp_to_buf(const char *fileName)
{
    // 2、打开图片
    int bmpFd = open(fileName, O_RDONLY);
    if (bmpFd == -1)
    {
        printf("%s open bmp error:%s\n", fileName, strerror(errno));
        return -1;
    }
    // 偏移文件光标位置到  第54个字节开始 ，因为从这里开始，才是图形的颜色数据
    // 文件头+信息头 ---》关于这个bmp文件的信息描述
    BITMAPFILEHEADER fileHeader;
    read(bmpFd, &fileHeader, 14);
    BITMAPINFOHEADER infoHeader;
    read(bmpFd, &infoHeader, 40);

    // 3、读取图片的数据
    char bmpbuf[800 * 480 * 3] = {0};
    read(bmpFd, bmpbuf, 800 * 480 * 3);
    // 4、将读取出来的图片数据的每个像素点 BGR ---转换成 ARGB
    int tempbuf[800 * 480] = {0};
    for (int i = 0, j = 0; i < 800 * 480; i++, j += 3)
    {
        tempbuf[i] = 0x00 << 24 | bmpbuf[2 + j] << 16 | bmpbuf[1 + j] << 8 | bmpbuf[0 + j];
    }
    // 将图片的数据进行上下 180度颠倒
    // int lcdbuf[800*480] = {0};
    for (int j = 0; j < 480; j++)
    {
        for (int i = 0; i < 800; i++)
        {
            black_bmp[800 * j + i] = tempbuf[(479 - j) * 800 + i];
        }
    }
    close(bmpFd);

    return 0;
}

/**
 * @brief 背景buf部分到画板
 * @param posX
 * @param posY
 * @param w
 * @return
 */
int back_part_to_palette(int posX, int posY, int w)
{
    int *p = black_bmp + posY * 800 + posX;
    int *ppalette = palette + posY * 800 + posX;
    memcpy(ppalette, p, w * 4);
    return 0;
}

/**
 * @brief 背景全部输出到画板
 * @return
 */
int back_all_to_palette()
{
    for (int i = 0; i < 480; i++)
    {
        back_part_to_palette(0, i, 800);
    }
    return 0;
}

int show_table(Usr_t *usr)
{
    switch (usr->Stage)
    {
    case LOGIN:
        login_show_table();
        break;
    case CALL_LANDLORD:
        // printf("刷新\n");
        landlord_show_table(usr);
        break;
    case GAMING:
        gaming_show_table(usr);
        break;
    default:
        break;
    }
}

void login_show_table()
{
    back_all_to_palette();

    bmp_pox_to_palette("78.bmp", 300, 200);

    palette_to_screen();
}

void landlord_show_table(Usr_t *usr)
{
    back_all_to_palette();
    int maxScore = usr->Table.m_bLandScore;

    if (usr->usr_turn && maxScore == 0)
    {
        printf("用户叫分\n");
        bmp_pox_to_palette("72.bmp", 140, 200);
        bmp_pox_to_palette("74.bmp", 250, 200);
        bmp_pox_to_palette("76.bmp", 360, 200);
        bmp_pox_to_palette("77.bmp", 550, 200);
    }
    else if (usr->usr_turn && maxScore == 1)
    {
        bmp_pox_to_palette("73.bmp", 140, 200);
        bmp_pox_to_palette("74.bmp", 250, 200);
        bmp_pox_to_palette("76.bmp", 360, 200);
        bmp_pox_to_palette("77.bmp", 550, 200);
    }
    else if (usr->usr_turn && maxScore == 2)
    {
        bmp_pox_to_palette("73.bmp", 140, 200);
        bmp_pox_to_palette("75.bmp", 250, 200);
        bmp_pox_to_palette("76.bmp", 360, 200);
        bmp_pox_to_palette("77.bmp", 550, 200);
    }

    // out_card_to_palette(table->m_bHandCardData[wChairID], table->m_bCardCount[wChairID]);

    my_card_to_palette(usr->usr_id, usr->Table.m_bHandCardData, usr->Table.m_bCardCount[usr->usr_id]);

    show_back_card_back(usr->Table.m_bBackCard);

    other_card_to_palette(usr->Table.m_bCardCount[(usr->usr_id + 1) % 3], 1);
    other_card_to_palette(usr->Table.m_bCardCount[(usr->usr_id + 2) % 3], 0);

    palette_to_screen();
}

void out_card_to_palette(uint8_t m_bTurnCardData[20], uint8_t m_bTurnCardCount)
{
    int mid = m_bTurnCardCount / 2;
    int start = 360 - mid * 20;
    for (int i = 0; i < m_bTurnCardCount; i++)
    {
        char cardBmpName[10] = {0};
        sprintf(cardBmpName, "%d.bmp", m_bTurnCardData[i]);
        bmp_scale_to_palette(cardBmpName, start + i * 20, 80, 2);
    }
}

void other_card_to_palette(uint8_t bCardCount, int flag)
{
    int edge;
    if (flag)
    {
        edge = 120;
    }
    else
    {
        edge = 650;
    }
    int mid = bCardCount / 2;
    int start = 100 - mid * 10;

    for (int i = 0; i < bCardCount; i++)
    {
        char cardBmpName[10] = {0};
        sprintf(cardBmpName, "%d.bmp", 100);
        bmp_pox_to_palette(cardBmpName, edge, start + i * 10);
    }
}

void show_back_card_back()
{
    bmp_pox_to_palette("101.bmp", 370, 0);
    bmp_pox_to_palette("101.bmp", 400, 0);
    bmp_pox_to_palette("101.bmp", 430, 0);
}

void show_back_card_front(uint8_t m_bBackCard[3])
{
    for (int i = 0; i < 3; i++)
    {
        char cardBmpName[10] = {0};
        sprintf(cardBmpName, "%d.bmp", m_bBackCard[i]);
        bmp_scale_to_palette(cardBmpName, 370 + i * 30, 0, 4);
    }
}

void gaming_show_table(Usr_t *usr)
{
    back_all_to_palette();

    printf("gaming_show_table\n");
    if (usr->usr_id == usr->Table.m_wCurrentUser)
    {
        bmp_pox_to_palette("71.bmp", 250, 200);
        bmp_pox_to_palette("70.bmp", 550, 200);
    }
    my_card_to_palette(usr->usr_id, usr->Table.m_bHandCardData, usr->Table.m_bCardCount[usr->usr_id]);

    show_back_card_front(usr->Table.m_bBackCard);
    out_card_to_palette(usr->Table.m_bTurnCardData, usr->Table.m_bTurnCardCount);

    other_card_to_palette(usr->Table.m_bCardCount[(usr->usr_id + 1) % 3], 1);
    other_card_to_palette(usr->Table.m_bCardCount[(usr->usr_id + 2) % 3], 0);
    user_pic_to_palette(usr);

    palette_to_screen();
}

void user_pic_to_palette(Usr_t *usr)
{
    if (usr->Table.m_wBankerUser == usr->usr_id)
    {
        bmp_pox_to_palette("p0.bmp", 10, 380);
        bmp_pox_to_palette("p1.bmp", 10, 20);
        bmp_pox_to_palette("p1.bmp", 695, 20);
    }
    else
    {
        bmp_pox_to_palette("p1.bmp", 10, 380);
        if ((usr->usr_id + 1) % 3 == usr->Table.m_wBankerUser)
        {
            bmp_pox_to_palette("p1.bmp", 695, 20);
            bmp_pox_to_palette("p0.bmp", 10, 20);
        }
        else
        {
            bmp_pox_to_palette("p0.bmp", 695, 20);
            bmp_pox_to_palette("p1.bmp", 10, 20);
        }
    }
}

// 倒计时出牌
void timer_count_down()
{
    time_count = 15;
    timer_count_on_off(1);
}

void timer_to_palette()
{
    char cardBmpName[20] = {0};
    sprintf(cardBmpName, "clock%d.bmp", time_count);

    if (g_usr->Table.m_wCurrentUser == 0)
    {
        if (g_usr->usr_id == 0)
        {
            bmp_pox_to_palette(cardBmpName, 10, 275);
            bmp_show_size(cardBmpName, 10, 275);
        }
        else if (g_usr->usr_id == 1)
        {
            bmp_pox_to_palette(cardBmpName, 695, 100);
            bmp_show_size(cardBmpName, 695, 100);
        }
        else if (g_usr->usr_id == 2)
        {
            bmp_pox_to_palette(cardBmpName, 10, 100);
            bmp_show_size(cardBmpName, 10, 100);
        }
    }
    else if (g_usr->Table.m_wCurrentUser == 1)
    {
        if (g_usr->usr_id == 0)
        {
            bmp_pox_to_palette(cardBmpName, 10, 100);
            bmp_show_size(cardBmpName, 10, 100);
        }
        else if (g_usr->usr_id == 1)
        {
            bmp_pox_to_palette(cardBmpName, 10, 275);
            bmp_show_size(cardBmpName, 10, 275);
        }
        else if (g_usr->usr_id == 2)
        {
            bmp_pox_to_palette(cardBmpName, 695, 100);
            bmp_show_size(cardBmpName, 695, 100);
        }
    }
    else if (g_usr->Table.m_wCurrentUser == 2)
    {
        if (g_usr->usr_id == 0)
        {
            bmp_pox_to_palette(cardBmpName, 695, 100);
            bmp_show_size(cardBmpName, 695, 100);
        }
        else if (g_usr->usr_id == 1)
        {
            bmp_pox_to_palette(cardBmpName, 10, 100);
            bmp_show_size(cardBmpName, 10, 100);
        }
        else if (g_usr->usr_id == 2)
        {
            bmp_pox_to_palette(cardBmpName, 10, 275);
            bmp_show_size(cardBmpName, 10, 275);
        }
    }

    //palette_to_screen();
    time_count--;

    if (time_count == 0)
    {
        OnUserPassCard(&g_usr->Table, g_usr->usr_id);
        show_table(g_usr);
        send_message(g_usr);
        timer_count_on_off(0);
    }
}

void timer_count_on_off(int on_off)
{
    struct itimerval tick;

    signal(SIGALRM, timer_to_palette);
    memset(&tick, 0, sizeof(tick));

    // Timeout to run first time
    tick.it_value.tv_sec = 0;
    tick.it_value.tv_usec = on_off;

    // After first, the Interval time for clock
    tick.it_interval.tv_sec = 1;
    tick.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &tick, NULL) < 0)
        printf("Set timer failed!\n");
}
/////////////////////////////////////////////////////////////////////////////////////
