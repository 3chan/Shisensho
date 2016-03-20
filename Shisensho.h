#ifndef __SHISENSHO_H__
#define __SHISENSHO_H__

#include "ImageData.h"

#define ONE_PIECE_SIZE 40  /* コマ 1 つのピクセル数 ( 1 辺 ) */
#define PIECE_SIZE 12      /* パズルの大きさ ( 1 辺 ) */
#define FRAME_SIZE (PIECE_SIZE+2)  /* あたり判定に用いるマス目の大きさ ( 1 辺 ) */
#define ALPHABET 26
#define ASCII 97


extern ImageData g_startImage;
extern ImageData g_clearImage;
extern ImageData g_charImages[ALPHABET];
extern ImageData g_pieceImages[PIECE_SIZE * PIECE_SIZE / 4];

/* あるコマを基準にした、他のマスに辿り着くために必要な角数を格納 */
extern int g_distance[FRAME_SIZE * FRAME_SIZE];  /* 55: 壁,  0: 消滅,  1: 存在,  88: 押されている */

enum Intend {
  START,
  PLAY,
  NEW_GAME,
  GAME_CLEAR,
  RANKING
};

typedef struct {
  int type;
  int state;  /* 0: 消滅,  1: 存在,  88: 押されている */
  ImageData *imagedata;
} PieceData;

extern PieceData g_pieceData[PIECE_SIZE * PIECE_SIZE];


/* main.c 内 */
void GetResources(void);


/* Shisehsno.c 内 */
void InitPieceData(int type, int state, ImageData *img_Data, PieceData *p_Data);
void RandPieceData(PieceData *p_Data);
void InitDistance(int distance[]);
void ChangePieceState(PieceData p_Data); 
//void CalcDistance(int distance[]);  <= TODO

int Conv14toX(int i);
int Conv14toY(int i);
int Conv12to14(int i);

void DebugDistance(int distance[]);

#endif
