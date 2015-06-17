/* FILENAME: UBALL.C
 * PROGRAMMER: PK2
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

typedef struct tagPK2UNIT_BALL
{
  PK2_UNIT_BASE_FIELDS;

  VEC Pos;    
  DWORD Color; 
  DBL
    Amplitude,  
    PhaseShift,
    ScaleShift; 
} PK2UNIT_BALL;

static VOID PK2_AnimUnitInit( PK2UNIT_BALL *Uni, PK2ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'PK2_AnimUnitInit' function */

static VOID PK2_AnimUnitResponse( PK2UNIT_BALL *Uni, PK2ANIM *Ani )
{
  Uni->Amplitude += 30 * 30 * Ani->MsWheel * Ani->GlobalDeltaTime;
} /* End of 'PK2_AnimUnitResponse' function */

static VOID PK2_AnimUnitRender( PK2UNIT_BALL *Uni, PK2ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * Ani->Time + Uni->PhaseShift));

  /*
  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
  */
} /* End of 'PK2_AnimUnitRender' function */

PK2UNIT * PK2_UnitBallCreate( VOID )
{
  PK2UNIT_BALL *Uni;

  if ((Uni = (VOID *)PK2_AnimUnitCreate(sizeof(PK2UNIT_BALL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)PK2_AnimUnitInit;
  Uni->Response = (VOID *)PK2_AnimUnitResponse;
  Uni->Render = (VOID *)PK2_AnimUnitRender;
  return (PK2UNIT *)Uni;
} /* End of 'PK2_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
