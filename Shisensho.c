#include <stdio.h>
#include <stdlib.h>
#include "Shisensho.h"



/* コマの種類・状態・画像データを初期化 */
void InitPieceData(int type, int state, ImageData *img_Data, PieceData *p_Data) {
  //printf("== InitPieceData ==\n");
  p_Data->type = type;
  p_Data->state = state;
  p_Data->imagedata = img_Data;
}



/* コマをランダムに並び替える */
void RandPieceData(PieceData *p_Data) {
  printf("== RandPieceData ==\n");
  int i = 0, rnd = 0;
  PieceData buff;

  for (i = 0; i < PIECE_SIZE * PIECE_SIZE; i++) {
    rnd = rand() % (PIECE_SIZE * PIECE_SIZE);
    
    buff = p_Data[i];
    p_Data[i] = p_Data[rnd];
    p_Data[rnd] = buff;
  }
}



void InitDistance (int distance[]) {
  printf("== InitDistance ==\n");
  int i = 0;

  /* 外周 1 マスを「55: 壁」、内側を「1: 存在」に設定 */
  for (i = 0; i < FRAME_SIZE * FRAME_SIZE; i++) {
    if (FRAME_SIZE < i && i < FRAME_SIZE * (FRAME_SIZE - 1) && i % FRAME_SIZE != FRAME_SIZE - 1 && i % FRAME_SIZE != 0) {
      distance[i] = 1;
    } else {
      distance[i] = 55;
    }
  }
}



/* 配列の添字をピクセル数に変換 */
int Conv14toX(int i) {
  return 50 + ONE_PIECE_SIZE * (i % PIECE_SIZE);
}

int Conv14toY(int i) {
  return 570 - ONE_PIECE_SIZE * (i / PIECE_SIZE);
}



/* [DEBUG] Distanceデータ可視化 */
void DebugDistance(int distance[]) {
  int i = 0;

  printf("\n == Distance ==\n");
  for (i = 0; i < FRAME_SIZE * FRAME_SIZE; i++) {
    printf(" %2d", distance[i]);
    if (i % FRAME_SIZE == (FRAME_SIZE - 1)) printf("\n");
  }
  printf("\n");
}
