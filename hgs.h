#ifndef HGS_H
#define HGS_H

typedef struct hgs_position
{
  int x;
  
  int y;
  
} hgs_position;

typedef struct hgs_player 
{
  int id;
  
  hgs_position *pos;
  
} hgs_player;

#endif
