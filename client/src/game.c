// #include "game.h"
// typedef enum _game_event
// {
//     G_READ,
//     G_STATRT,
//     G_LANDSCORE,
//     G_SELECTCART,
//     G_OUTCART,
//     G_CANNOTOUT,
//     G_OVER,
// } game_event;

// static FsmTable_T g_stFsmTable[] =
//     {
//         /* 触发事件         初态            动作函数             次态  */
//         {-1, G_READ, handel_reply, G_STATRT},
//         {-1, G_STATRT, handel_changed, G_LANDSCORE},
//         {-1, G_LANDSCORE, handel_back, G_SELECTCART},
//         {-1, G_SELECTCART, handel_back, G_OUTCART},
//         {-1, G_SELECTCART, handel_back, G_CANNOTOUT},
//         {-1, G_OUTCART, handel_back, G_SELECTCART},
//         {-1, G_CANNOTOUT, handel_back, G_SELECTCART},
//         {-1, G_OUTCART, handel_back, G_OVER},
//         {-1, G_OVER, handel_back, G_READ},
// };
// void start_game(TableFrame *table)
// {
//     table = tableInit();

//     OnEventGameStart(table);

//     back_all_to_palette();

//     bmp_pox_to_palette("72.bmp", 160, 210);
//     bmp_pox_to_palette("74.bmp", 280, 210);
//     bmp_pox_to_palette("76.bmp", 300, 210);
//     bmp_pox_to_palette("77.bmp", 420, 210);

    
// }
