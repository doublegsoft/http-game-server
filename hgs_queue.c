#include <stdlib.h>

#include "hgs.h"
#include "hgs_queue.h"

static hgs_player_queue queue = {NULL, NULL, 0};


void 
hgs_player_add(hgs_player *player) 
{
  hgs_queued_player *queued_player = malloc(sizeof(hgs_queued_player));
  queued_player->player = player;
  
  if (queue.head == NULL)
  {
    queue.head = queued_player;
    queue.tail = queued_player;
  } 
  else
  {
    queue.tail->next = queued_player;
    queue.tail = queued_player;
  }
  queue.size++;
}

hgs_player*
hgs_player_find(int id) 
{
  hgs_queued_player *found;
  for (found = queue.head; found != NULL; ) 
  {
    if (found->player->id == id)
      return found->player;
  }
  return NULL;
}

