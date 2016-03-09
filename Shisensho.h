#ifndef __SHISENSHO_H__
#define __SHISENSHO_H__

#include "ImageData.h"

#define ONE_PIECE_SIZE 40  /* コマ 1 つのピクセル数 ( 1 辺 ) */
#define PIECE_SIZE 12      /* パズルの大きさ ( 1 辺 ) */
#define FRAME_SIZE PIECE_SIZE+2  /* あたり判定に用いるマス目の大きさ ( 1 辺 ) */
#define ALPHABET 26
#define ASCII 97


extern ImageData g_startImage;
extern ImageData g_clearImage;
extern ImageData g_charImages[ALPHABET];
extern ImageData g_pieceImages[PIECE_SIZE * PIECE_SIZE / 4];

enum Intend {
  START,
  PLAY,
  NEW_GAME,
  GAME_CLEAR,
  RANKING
};

typedef struct {
  int type;
  int state;
  ImageData *imagedata;
} PieceData;

extern PieceData g_pieceData[PIECE_SIZE * PIECE_SIZE];


/* main.c 内 */
void GetResources(void);


/* Shisehsno.c 内 */
void SetPieceData(int type, int state, ImageData *img_Data, PieceData *p_Data);

#endif
