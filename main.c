#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>

#include "ImageData.h"
#include "ImageIO.h"

#include "GenericTimer.h"

#include "Shisensho.h"


int g_WindowWidth = 580;    /* ウィンドウの横幅 */
int g_WindowHeight = 655;   /* ウィンドウの縦幅 */

#define IMAGE_FILE "lenna128x128.ppm"

ImageData g_Image;
int g_ImagePosX = 256;	/* 画像の左下点の、初期 x 座標 */
int g_ImagePosY = 0;	/* 画像の左下点の、初期 y 座標 */
int g_ImageVelocityX = -3;	/* 座標更新時の、x 方向の移動距離 */
int g_ImageVelocityY = 2;	/* 座標更新時の、y 方向の移動距離 */
int g_AnimationDulation = 10;/* 何ミリ秒ごとに更新するかの刻み幅 */

double g_PrevTime = 0.0;


/* オリジナルの変数 */
ImageData g_startImage;
ImageData g_clearImage;
ImageData g_charImages[ALPHABET];
ImageData g_pieceImages[PIECE_SIZE * PIECE_SIZE /4];
PieceData g_pieceData[PIECE_SIZE * PIECE_SIZE];

int g_distance[FRAME_SIZE * FRAME_SIZE];
int g_prevClickedPiece;

enum Intend intend;



/* リソース読み込み */
void GetResources(void) {
  printf("\n== GetResorces ==\n");
  int i = 0;
  char filename[32];

  /* ゲーム開始画面 */
  InitImageData(&g_startImage);
  if (LoadPPMImage("res/wall/start.ppm", &g_startImage)) FlipImageData(&g_startImage);

  /* ゲームクリア画面 */
  InitImageData(&g_clearImage);
  if (LoadPPMImage("res/wall/clear.ppm", &g_clearImage)) FlipImageData(&g_clearImage);

  /* コマ */
  for (i = 0; i < PIECE_SIZE * PIECE_SIZE / 4; i++) {
    InitImageData(&g_pieceImages[i]);
    sprintf(filename, "res/piece/piece_%02d.ppm", i+1);
    if (LoadPPMImage(filename, &g_pieceImages[i])) FlipImageData(&g_pieceImages[i]);
  }

  /* アルファベット */
  for (i = 0; i < ALPHABET; i++) {
    InitImageData(&g_charImages[i]);
    sprintf(filename, "res/char/ch%03d.ppm", ASCII + i);
    if (LoadPPMImage(filename, &g_charImages[i])) FlipImageData(&g_charImages[i]);
  }
}



/* 初期化処理 */
void init(void) {
  printf("\n== Init ==\n");
  int i = 0, j = 0;
  glClearColor(1.0, 1.0, 1.0, 1.0);   /* ウィンドウを消去するときの色を設定 */

  InitImageData(&g_Image);	/* 画像データを初期化 */
  if ( LoadPPMImage(IMAGE_FILE, &g_Image) ) {  /* 画像データの読み込みに成功 */
    FlipImageData(&g_Image); /* 画像の上下を反転する */
  }

  /* コマ 1 種につき 4 つ分のデータを作る */
  for (i=0; i < PIECE_SIZE * PIECE_SIZE / 4; i++) {
    for (j=0; j<4; j++) {
      InitPieceData(i, 1, &g_pieceImages[i], &(g_pieceData[4 * i + j]));  /* コマの種類, 状態「1: 存在」, 画像データ, データ入力先 */
    }
  }

  RandPieceData(g_pieceData);  /* 作ったデータをランダムに並び替え */
  
  g_prevClickedPiece = InitPrevClickedPiece();  /* ??? NEW_GAME するときに init() は呼び出すのか、それによりこの行を書く場所が変わる ??? */
  
  InitDistance(g_distance);

  intend = START;

  /* 時間計測の開始 */
  srand((int)time(NULL));
  StartTimer();
}



/* 表示処理のためのコールバック関数 */
void display(void) {
  int i = 0;
  
  /* ウィンドウを消去 … glClearColor で指定した色で塗りつぶし */
  glClear(GL_COLOR_BUFFER_BIT);

  switch (intend) {
  case START:
    if (IsImageDataAllocated(&g_startImage)) {
      DrawImageData(&g_startImage, 0, 0);
    }
    break;
  case PLAY:
    for (i = 0; i < (PIECE_SIZE * PIECE_SIZE); i++) {
      if (IsImageDataAllocated(g_pieceData[i].imagedata) && (g_pieceData[i].state == 1)) {
	  DrawImageData(g_pieceData[i].imagedata, Conv12toX(i), Conv12toY(i));
      }
    }
    break;
  default:
    break;
  }

  if (IsImageDataAllocated(&g_Image)) {  /* もし画像が読み込めていたら */
    /* 座標 (g_ImagePosX, g_ImagePosY) の位置に、画像を表示 */
    DrawImageData(&g_Image, g_ImagePosX, g_ImagePosY);
  }
  
  glutSwapBuffers();
}



/* 定期的に呼び出され、画像の位置を更新するための関数 */
void idle(void)
{
	/* もし前回の更新から一定時間が過ぎていたら */
	if ( GetRapTime(g_PrevTime) >= g_AnimationDulation )
	{
		/* x, y 座標を更新 */
		g_ImagePosX += g_ImageVelocityX;
		g_ImagePosY += g_ImageVelocityY;

		/* 画面の外に出ないように座標を調整 */

		if (g_ImagePosX < 0)
		{
			g_ImagePosX = 0;
			g_ImageVelocityX = -g_ImageVelocityX;
		}
		else if (g_WindowWidth - g_Image.width <= g_ImagePosX)
		{
			g_ImagePosX = g_WindowWidth - g_Image.width;
			g_ImageVelocityX = -g_ImageVelocityX;
		}

		if (g_ImagePosY < 0)
		{
			g_ImagePosY = 0;
			g_ImageVelocityY = -g_ImageVelocityY;
		}
		else if (g_WindowHeight - g_Image.height <= g_ImagePosY )
		{
			g_ImagePosY = g_WindowHeight - g_Image.height;
			g_ImageVelocityY = -g_ImageVelocityY;
		}

		/* 最終更新時刻を記録する */
		g_PrevTime = GetTime();
	}

	display();
}



/* キーボード入力のためのコールバック関数 */
void keyboard(unsigned char key, int x, int y) {
  switch (intend) {
  case START:
    if (key == '\r' || key == '\n') {
      StartTimer();
      intend = PLAY;
    }
    else if (key == '\33') exit(0);
    break;

  case PLAY:
    if (key == 'n' || key == 'N') intend = NEW_GAME;
    if (key == '\33') intend = START;
    break;

  case NEW_GAME:
    break;

  case GAME_CLEAR:
    break;

  case RANKING:
    break;

  default:
    break;
  }

  glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */
}



/* マウス入力のためのコールバック関数 */
void mouse(int button, int state, int x, int _y) {
  int i = 0, y = g_WindowHeight - _y;
  glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */

  if (state == GLUT_UP) {
    switch (intend) {
    case START:
      break;

    case PLAY:
      for (i = 0; i < PIECE_SIZE * PIECE_SIZE; i++) {
	if (Conv12toX(i) < x && x < Conv12toX(i) + ONE_PIECE_SIZE) {
	  if (Conv12toY(i) < y && y < Conv12toY(i) + ONE_PIECE_SIZE) {
	    g_pieceData[i].state = ChangePieceState(g_pieceData[i]);  // 関数分けするか否か (現在はしている)
	    /* 1回目のクリック */
	    if (g_prevClickedPiece == -1) {
	      g_prevClickedPiece = i;
	      if (SaveDistance(g_distance, i) != 88) {  /* 壁や消滅したコマをクリックしていた時 */
		g_prevClickedPiece = InitPrevClickedPiece();
		g_pieceData[i].state = ChangePieceState(g_pieceData[i]);  //関数分け(ry
		break;  // break文でfor文のループを抜けられる
	      }
	    }
	    /* 2回目のクリック */
	    else {
	      printf("== else ==\n");
	      if (g_pieceData[i].type == g_pieceData[g_prevClickedPiece].type) {
		LoadDistance(g_distance, i, g_prevClickedPiece);
	      }
	      g_pieceData[i].state = ChangePieceState(g_pieceData[i]);  // 関数分け(ry
	      g_pieceData[g_prevClickedPiece].state = ChangePieceState(g_pieceData[g_prevClickedPiece]);  // 関数分け(ry
	      g_prevClickedPiece = InitPrevClickedPiece();
	      ResetDistance(g_distance);
	    }
	  }
	}
      }
      break;

    case NEW_GAME:
      break;

    case GAME_CLEAR:
      break;

    case RANKING:
      break;

    default:
      break;

    }
    DebugDistance(g_distance);
    DebugPieceState(g_pieceData);
  }
}



/* ウィンドウサイズが変更されたときのためのコールバック関数 */
void reshape(int w, int h)
{
	if (h < 1) return;

	g_WindowWidth = w;
	g_WindowHeight = h;

	glViewport(0, 0, w, h); /* ウィンドウの描画範囲を指定 */

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glutPostRedisplay();    /* ウィンドウ描画関数を呼ぶ */
}



int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(g_WindowWidth, g_WindowHeight);
  glutCreateWindow("Shisensho");
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutReshapeFunc(reshape);

  GetResources();
  init();
  glutMainLoop();

  return 0;
}
