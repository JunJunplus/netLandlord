#pragma once
#include "card.h"
#include "table.h"
#include "bmp.h"
#include "fsm.h"

typedef enum _game_event
{
    G_STATRT,
    G_LANDSCORE,
    G_SELECTCART,
    G_OUTCART,
    G_CANNOTOUT,
    G_OVER,
}game_event;

