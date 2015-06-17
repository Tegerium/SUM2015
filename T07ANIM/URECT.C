/* FILENAME: URECT.C
 * PROGRAMMER: PK2
 * PURPOSE: Bounce rectangle unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"


typedef struct tagPK2UNIT_RECT
{
  PK2_UNIT_BASE_FIELDS;

  VEC Pos;     
  DWORD Color; 
  DBL
    PhaseShift; 
} PK2UNIT_RECT;



static VOID PK2_RectUnitInit( PK2UNIT_RECT *Uni, PK2ANIM *Ani )
{
  DBL t = clock() / (DOUBLE)CLOCKS_PER_SEC;

  Uni->Pos = VecSet(rand() % 1920, rand() % 1080, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = t * 30 * 59;
} /* End of 'PK2_AnimUnitInit' function */


static VOID PK2_RectUnitClose( PK2UNIT_RECT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitClose' function */


static VOID PK2_RectUnitResponse( PK2UNIT_RECT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitResponse' function */



static VOID PK2_RectUnitRender( PK2UNIT_RECT *Uni, PK2ANIM *Ani )
{
  DBL shift = fabs(sin(Uni->PhaseShift + (DBL)clock() / CLOCKS_PER_SEC * 3)) * 30;

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Rectangle(Ani->hDC, Uni->Pos.X - 5 - shift, Uni->Pos.Y - 5, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'PK2_AnimUnitRender' function */


PK2UNIT * PK2_UnitRectCreate( VOID )
{
  PK2UNIT_RECT *Uni;

  if ((Uni = (VOID *)PK2_AnimUnitCreate(sizeof(PK2UNIT_RECT))) == NULL)
    return NULL;

  Uni->Init = (VOID *)PK2_RectUnitInit;
  Uni->Close = (VOID *)PK2_RectUnitClose;
  Uni->Response = (VOID *)PK2_RectUnitResponse;
  Uni->Render = (VOID *)PK2_RectUnitRender;
  return (PK2UNIT *)Uni;
} /* End of 'PK2_UnitRectCreate' function */

/* END OF 'URECT.C' FILE */