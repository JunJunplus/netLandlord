#ifndef __BMP_H
#define __BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <stdint.h>
#include <sys/time.h> 
#include <signal.h> 

#include "touch.h"
#include "table.h"
#include "font.h"

#define LEAVE_WIDTH 28
#define CARD_MID 406
#define CARD_HEIGHT 295

#define UINT unsigned short
#define DWORD unsigned int
#define LONG unsigned int
#define WORD unsigned short

// typedef struct TableFrameSink TableFrame;

extern int *palette;
int *black_bmp;
uint8_t m_bHandCardUp[20]; //
int time_count;
int usr_count;
int usr_id;

typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;      // 位图文件的类型，必须为BM(1-2字节）
    DWORD bfSize;     // 位图文件的大小，以字节为单位（3-6字节）
    WORD bfReserved1; // 位图文件保留字，必须为0(7-8字节）
    WORD bfReserved2; // 位图文件保留字，必须为0(9-10字节）
    DWORD bfOffBits;  // 位图数据的起始位置，以相对于位图（11-14字节）
    // 文件头的偏移量表示，以字节为单位
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;    // 本结构所占用字节数（15-18字节）
    LONG biWidth;    // 位图的宽度，以像素为单位（19-22字节）    ------图片的宽度
    LONG biHeight;   // 位图的高度，以像素为单位（23-26字节）   ------图片的高度
    WORD biPlanes;   // 目标设备的级别，必须为1(27-28字节）
    WORD biBitCount; // 每个像素所需的位数，必须是1（双色），（29-30字节）  ---位深
    // 4(16色），8(256色）或24（真彩色）之一
    DWORD biCompression; // 位图压缩类型，必须是 0（不压缩），（31-34字节）
    // 1(BI_RLE8压缩类型）或2(BI_RLE4压缩类型）之一
    DWORD biSizeImage;    // 位图的大小，以字节为单位（35-38字节）   ---图片颜色数据的大小
    LONG biXPelsPerMeter; // 位图水平分辨率，每米像素数（39-42字节）
    LONG biYPelsPerMeter; // 位图垂直分辨率，每米像素数（43-46字节)
    DWORD biClrUsed;      // 位图实际使用的颜色表中的颜色数（47-50字节）
    DWORD biClrImportant; // 位图显示过程中重要的颜色数（51-54字节）
} BITMAPINFOHEADER;

extern int bmp_show(const char *fileName);
extern int bmp_show_swap(const char *fileName);
extern int bmp_show_another(const char *fileName);
extern int bmp_show_double(int (*lcdbuf)[800 * 480], int x, int i);
extern int bmp_show_size(const char *fileName, int posX, int posY);

////////////////////////////////////////////////////////////////////////////////////////////////////////
//bmp输出到画板指定位置
extern int bmp_pox_to_palette(const char *fileName, int posX, int posY);
//bmp部分输出到画板指定位置
extern int bmp_pox_to_palette_with_part(const char *fileName, int posX, int posY, int width, int height);
//画板全部输出到屏幕
extern int palette_to_screen();
//初始化画板
extern void init_palette();
//该用户手牌出到画板
extern void my_card_to_palette(int wChairID, uint8_t m_bHandCardData[3][20], uint8_t bCardCount);
//bmp改变大小输出到画板指定位置
extern int bmp_scale_to_palette(const char *fileName, int posX, int posY, int scale);
//弹起或落下手牌
extern void card_on_click(int wChairID, uint8_t m_bHandCardData[3][20], uint8_t bCardCount, uint8_t cardId);
//
extern void  card_on_click_slide(int wChairID, uint8_t m_bHandCardData[3][20], uint8_t bCardCount, uint8_t cardId);
//初始化背景buf
extern void init_bmp();
//叫分阶段场景输出
extern void landlord_show_table(Usr_t *usr);
//出的牌输出到画板
extern void out_card_to_palette(uint8_t m_bTurnCardData[20], uint8_t m_bTurnCardCount);
//场景总输出
extern int show_table(Usr_t *usr);
//登录阶段场景输出
extern void login_show_table();


//背景bmp存储到buf
int back_bmp_to_buf(const char *fileName);
//背景部分横条画到画板
int back_part_to_palette(int posX, int posY, int w);
// 背景全部输出到画板
int back_all_to_palette();
//其他卡输出到画板
void other_card_to_palette(uint8_t bCardCount, int flag);
//底牌正面输出
void show_back_card_front(uint8_t m_bBackCard[3]);
//底牌背面输出
void show_back_card_back();
//正在游戏画面输出
void gaming_show_table(Usr_t *usr);
//把地主人物输出到画板
void user_pic_to_palette(Usr_t *usr);

//计时器--(每秒钟减一)
void timer_count_down();
//计时器画面到画板
void timer_to_palette();
//计时器开关
void timer_count_on_off(int on_off);
////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
