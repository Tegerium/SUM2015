/* FILENAME: UCONTRL.C
 * PROGRAMMER: PK2
 * PURPOSE: Control unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <stdio.h>

#include "anim.h"

typedef struct tagPK2UNIT_CTRL
{
  PK2_UNIT_BASE_FIELDS;
  HFONT hFnt; 
} PK2UNIT_CTRL;

static VOID PK2_AnimUnitInit( PK2UNIT_CTRL *Uni, PK2ANIM *Ani )
{
  Uni->hFnt = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE,
    FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY,
    VARIABLE_PITCH, "Bookman Old Style");
} /* End of 'PK2_AnimUnitInit' function */

static VOID PK2_AnimUnitClose( PK2UNIT_CTRL *Uni, PK2ANIM *Ani )
{
  DeleteObject(Uni->hFnt);
} /* End of 'PK2_AnimUnitClose' function */

static VOID PK2_AnimUnitResponse( PK2UNIT_CTRL *Uni, PK2ANIM *Ani )
{
  if (Ani->Keys[VK_ESCAPE])
    PK2_AnimDoExit();
  if (Ani->KeysClick['F'])
    PK2_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    PK2_AnimSetPause(!Ani->IsPause);
} /* End of 'PK2_AnimUnitResponse' function */

static VOID PK2_AnimUnitRender( PK2UNIT_CTRL *Uni, PK2ANIM *Ani )
{
  HFONT hFntOld = SelectObject(Ani->hDC, Uni->hFnt);
  static DBL count = 30;
  static CHAR Buf[1000];

  count += Ani->GlobalDeltaTime;
  if (count > 1)
  {
    count = 0;
    sprintf(Buf, "FPS: %.3f", Ani->FPS);
    SetWindowText(Ani->hWnd, Buf);
  }


  /*
  SetTextColor(Ani->hDC, RGB(255, 255, 255));
  SetBkMode(Ani->hDC, TRANSPARENT);
  rc.left = 0;
  rc.top = 0;
  rc.right = Ani->W;
  rc.bottom = Ani->H;
  DrawText(Ani->hDC, Buf,
    sprintf(Buf,
      "FPS: %.3f\n"
      "J: %i %i %i, POV:%i\n"
      "%.5f %.5f %.5f %.5f %.5f %.5f",
      Ani->FPS,
      Ani->JButs[0], Ani->JButs[1], Ani->JButs[2], Ani->JPOV,
      Ani->JX, Ani->JY, Ani->JZ, Ani->JR, Ani->JU, Ani->JV),
    &rc,
    DT_TOP | DT_LEFT);
  SelectObject(Ani->hDC, hFntOld);
  */
} /* End of 'PK2_AnimUnitRender' function */

PK2UNIT * PK2_UnitControlCreate( VOID )
{
  PK2UNIT_CTRL *Uni;

  if ((Uni = (VOID *)PK2_AnimUnitCreate(sizeof(PK2UNIT_CTRL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)PK2_AnimUnitInit;
  Uni->Close = (VOID *)PK2_AnimUnitClose;
  Uni->Response = (VOID *)PK2_AnimUnitResponse;
  Uni->Render = (VOID *)PK2_AnimUnitRender;
  return (PK2UNIT *)Uni;
} /* End of 'PK2_UnitBallCreate' function */

/* END OF 'UCONTRL.C' FILE */