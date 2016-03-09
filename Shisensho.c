#include <stdio.h>
#include <stdlib.h>

#include "Shisensho.h"


/* コマの種類・状態・画像データを管理 */
void SetPieceData(int type, int state, ImageData *img_Data, PieceData *p_Data) {
  p_Data->type = type;
  p_Data->state = state;
  p_Data->imagedata = img_Data;
}
