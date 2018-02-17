#ifndef HGS_QUEUE_H
#define HGS_QUEUE_H

#include "hgs.h"

typedef struct hgs_queued_player
{
  
  hgs_player                 *player;
  
  struct hgs_queued_player   *next;
  
} 
hgs_queued_player;

typedef struct hgs_player_queue
{
  hgs_queued_player          *head;
  
  hgs_queued_player          *tail;
  
  int                         size;
  
} 
hgs_player_queue;

void 
hgs_player_add(hgs_player *player);

#endif
