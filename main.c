#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "ImageData.h"
#include "ImageIO.h"

#include "GenericTimer.h"

#include "Shisensho.h"


int g_WindowWidth = 580;    /* �E�B���h�E�̉��� */
int g_WindowHeight = 655;   /* �E�B���h�E�̏c�� */

#define IMAGE_FILE "lenna128x128.ppm"

ImageData g_Image;
int g_ImagePosX = 256;	/* �摜�̍����_�́A���� x ���W */
int g_ImagePosY = 0;	/* �摜�̍����_�́A���� y ���W */
int g_ImageVelocityX = -3;	/* ���W�X�V���́Ax �����̈ړ����� */
int g_ImageVelocityY = 2;	/* ���W�X�V���́Ay �����̈ړ����� */
int g_AnimationDulation = 10;/* ���~���b���ƂɍX�V���邩�̍��ݕ� */
                                                                            
double g_PrevTime = 0.0;


/* �I���W�i���̕ϐ� */
ImageData g_startImage;
ImageData g_clearImage;
ImageData g_charImages[ALPHABET];
ImageData g_pieceImages[PIECE_SIZE * PIECE_SIZE /4];
PieceData g_pieceData[PIECE_SIZE * PIECE_SIZE];



/* ���\�[�X�ǂݍ��� */
void GetResources(void) {
  printf("\n== GetResorces ==\n");
  int i = 0;
  char filename[32];

  /* �Q�[���J�n��� */
  InitImageData(&g_startImage);
  if (LoadPPMImage("res/wall/start.ppm", &g_startImage)) FlipImageData(&g_startImage);

  /* �Q�[���N���A��� */
  InitImageData(&g_clearImage);
  if (LoadPPMImage("res/wall/clear.ppm", &g_clearImage)) FlipImageData(&g_clearImage);

  /* �R�} */
  for (i = 0; i < PIECE_SIZE * PIECE_SIZE / 4; i++) {
    InitImageData(&g_pieceImages[i]);
    sprintf(filename, "res/piece/piece_%02d.ppm", i+1);
    if (LoadPPMImage(filename, &g_pieceImages[i])) FlipImageData(&g_pieceImages[i]);
  }

  /* �A���t�@�x�b�g */
  for (i = 0; i < ALPHABET; i++) {
    InitImageData(&g_charImages[i]);
    sprintf(filename, "res/char/ch%03d.ppm", ASCII + i);
    if (LoadPPMImage(filename, &g_charImages[i])) FlipImageData(&g_charImages[i]);
  }  
}



/* ���������� */
void init(void) {
  printf("\n== Init ==\n");
  int i = 0, j = 0;
  glClearColor(1.0, 1.0, 1.0, 1.0);   /* �E�B���h�E����������Ƃ��̐F��ݒ� */
  
  InitImageData(&g_Image);	/* �摜�f�[�^�������� */	
  if ( LoadPPMImage(IMAGE_FILE, &g_Image) ) {  /* �摜�f�[�^�̓ǂݍ��݂ɐ��� */
    FlipImageData(&g_Image); /* �摜�̏㉺�𔽓]���� */
  }

  for (i=0; i < PIECE_SIZE * PIECE_SIZE / 4; i++) {
    for (j=0; j<4; j++) {
      SetPieceData(i, 0, &g_pieceImages[i], &(g_pieceData[4 * i + j]));
    }
  }
  
  StartTimer();	/* ���Ԍv���̊J�n */
}



/* �\�������̂��߂̃R�[���o�b�N�֐� */
void display(void) {
  /* �E�B���h�E������ �c glClearColor �Ŏw�肵���F�œh��Ԃ� */
  glClear(GL_COLOR_BUFFER_BIT);
  
  if (IsImageDataAllocated(&g_startImage)) {
    DrawImageData(&g_startImage, 0, 0);
  }

  if (IsImageDataAllocated(&g_Image)) {  /* �����摜���ǂݍ��߂Ă����� */
    /* ���W (g_ImagePosX, g_ImagePosY) �̈ʒu�ɁA�摜��\�� */
    DrawImageData(&g_Image, g_ImagePosX, g_ImagePosY);
  }
  
  glutSwapBuffers();
}



/* ����I�ɌĂяo����A�摜�̈ʒu���X�V���邽�߂̊֐� */
void idle(void)
{
	/* �����O��̍X�V�����莞�Ԃ��߂��Ă����� */
	if ( GetRapTime(g_PrevTime) >= g_AnimationDulation )
	{
		/* x, y ���W���X�V */
		g_ImagePosX += g_ImageVelocityX;
		g_ImagePosY += g_ImageVelocityY;

		/* ��ʂ̊O�ɏo�Ȃ��悤�ɍ��W�𒲐� */
		
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

		/* �ŏI�X�V�������L�^���� */
		g_PrevTime = GetTime();
	}

	display();
}



/* �L�[�{�[�h���͂̂��߂̃R�[���o�b�N�֐� */
void keyboard(unsigned char key, int x, int y) {
  switch (intend) {
  case START:
    if (key == '\r' || key == '\n') {
      StartTimer();
      intend = PLAY;
    }
    else if (key = '\33') exit(0);
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
  
  glutPostRedisplay();    /* �E�B���h�E�`��֐����Ă� */
}



/* �}�E�X���͂̂��߂̃R�[���o�b�N�֐� */
void mouse(int button, int state, int x, int _y) {
  glutPostRedisplay();    /* �E�B���h�E�`��֐����Ă� */
}



/* �E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��̂��߂̃R�[���o�b�N�֐� */
void reshape(int w, int h)
{
	if (h < 1) return;

	g_WindowWidth = w;
	g_WindowHeight = h;

	glViewport(0, 0, w, h); /* �E�B���h�E�̕`��͈͂��w�� */

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glutPostRedisplay();    /* �E�B���h�E�`��֐����Ă� */
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
