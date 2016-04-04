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



int InitPrevClickedPiece(void) {
  return -1;
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



int ChangePieceState(PieceData p_Data, int state) {
  if (state == 88) {
    return 88;
  }
  else if (state == 0) {
    return 0;
  }
  else if (state == 1) {
    return 1;
  }
  else {
    return -1;
  }
}

  

int SaveDistance (int distance[], int _pushedPiece) {
  printf("== SaveDistance ==\n");
  int pushedPiece = Conv12to14(_pushedPiece);

  if (distance[pushedPiece] == 0) {
    printf("0: 消滅\n");
    return 0;
  } else if (distance[pushedPiece] == 55) {
    printf("55: 壁\n");
    return 55;
  } else {
    distance[pushedPiece] = 88;  /* クリックされたコマの distance を「88: 押されている」に設定 */
    CheckDistance(distance, pushedPiece, 11);
    return 88;
  }
}



void CheckDistance(int distance[], int startPiece, int checkingDistance) {
  printf("== CheckDistance ==\n");
  int i = 0;
  int nowPiece = 0;
  int goNextPiece = 0;
  int nextPiece = 0;

  for (i = 0; i < 4; i++) {  /* 0: 上,  1: 右,  2: 下,  3: 左 */
    switch (i) {
    case 0:
      printf("0: ↑\n");
      goNextPiece = -FRAME_SIZE;
      break;
    case 1:
      printf("1: →\n");
      goNextPiece = 1;
      break;
    case 2:
      printf("2: ←\n");
      goNextPiece = -1;
      break;
    case 3:
      printf("3; ↓\n");
      goNextPiece = FRAME_SIZE;
      break;
    }

    nowPiece = startPiece;
    nextPiece = nowPiece + goNextPiece;
    
    /* 壁か「1: 存在」のコマに当たるまで for文で指定された方向に進み distance を引数の checkingDistance に置き換える */
    while (0 <= nextPiece && nextPiece < FRAME_SIZE * FRAME_SIZE && distance[nextPiece] != 55 &&  distance[nextPiece] != 1) {
      printf("== Coming ==\n");
      distance[nextPiece] = checkingDistance;
      nowPiece = nowPiece + goNextPiece;
      nextPiece = nowPiece + goNextPiece;
    }
  }
  
}



void ResetDistance (int distance[]) {
  printf("== ResetDistance ==\n");
  int i = 0;

  for (i = 0; i < FRAME_SIZE * FRAME_SIZE; i++) {  /* distance[] の要素数だけ回す。今は FRAME_SIZE * FRAME_SIZE */
    if (distance[i] == 11 || distance[i] == 22 || distance[i] == 33) {
      distance[i] = 0;  /* 角数探索に使った部分を「0: 消滅」にリセット */
    }
    else if (distance[i] == 88) {
      distance[i] = 1;  /* もし「88: 押されている」のままのマスがあれば「1: 存在」にリセット */
    }
  }
}



int LoadDistance(int distance[], int _pushedPiece, int _prevPushedPiece) {
  printf("== LoadDistance ==\n");
  int pushedPiece = Conv12to14(_pushedPiece);
  int prevPushedPiece = Conv12to14(_prevPushedPiece);
  int flag = 0;

  if (0 < pushedPiece - FRAME_SIZE) {
    if (distance[pushedPiece - FRAME_SIZE] == 11 || distance[pushedPiece - FRAME_SIZE] == 88) {
      printf("flag++ ↑\n");
      flag++;
    }
  }

  if (pushedPiece + FRAME_SIZE < FRAME_SIZE * FRAME_SIZE) {
    if (distance[pushedPiece + FRAME_SIZE] == 11 || distance[pushedPiece + FRAME_SIZE] == 88) {
      printf("flag++ ↓\n");
      flag++;
    }
  }

  if (distance[pushedPiece - 1] == 11 || distance[pushedPiece - 1] == 88) {
    printf("flag++ ←\n");
    flag++;
  }

  if (distance[pushedPiece + 1] == 11 || distance[pushedPiece + 1] == 88) {
    printf("flag++ →\n");
    flag++;
  }

  if (0 < flag) {
    distance[pushedPiece] = 0;
    distance[prevPushedPiece] = 0;
    return 0;
  }
  else {
    return -1;
  }
}



/* 配列の添字をピクセル数に変換 */
int Conv12toX(int i) {
  return 50 + ONE_PIECE_SIZE * (i % PIECE_SIZE);
}

int Conv12toY(int i) {
  return 570 - ONE_PIECE_SIZE * (i / PIECE_SIZE);
}


int Conv12to14(int i) {
  int j = FRAME_SIZE + 1, cnt = 0;;

  while (j < FRAME_SIZE * (FRAME_SIZE - 1)) {
    if (i == cnt) return j;
    if (j % FRAME_SIZE != 0 && j % FRAME_SIZE != FRAME_SIZE - 1) cnt++;
    j++;
  }
  return -1;
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



/* [DEBUG] コマの状態可視化 */
void DebugPieceState(PieceData p_Data[]) {
  int i = 0;

  printf("\n == PieceState ==\n");
  for (i = 0; i < PIECE_SIZE * PIECE_SIZE; i++) {
    printf(" %2d", p_Data[i].state);
    if (i % PIECE_SIZE == (PIECE_SIZE - 1)) printf("\n");
  }
  printf("\n");
}
