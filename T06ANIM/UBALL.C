/* FILENAME: UBALL.C
 * PROGRAMMER: VG4
 * PURPOSE: Bounce ball unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>
#include <time.h>

#include "anim.h"
#include "vec.h"

/* ��� ������������� ���� */
typedef struct tagvg4UNIT_BALL
{
  VG4_UNIT_BASE_FIELDS;

  VEC Pos;     /* ������� ���� */
  DWORD Color; /* ���� ���� */
  DBL
    Amplitude,  /* ��������� */
    PhaseShift, /* ����� �� ���� */
    ScaleShift; /* ������� ������� */
} vg4UNIT_BALL;

/* ������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitInit( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  Uni->Pos = VecSet(rand() % 1000, rand() % 700, 0);
  Uni->Color = RGB(rand() % 256, rand() % 256, rand() % 256);
  Uni->PhaseShift = rand() % 3000;
  Uni->ScaleShift = 5 + 0.30 * rand() / RAND_MAX;
  Uni->Amplitude = 30 + 59.0 * rand() / RAND_MAX;
} /* End of 'VG4_AnimUnitInit' function */

/* ������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitClose( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_AnimUnitClose' function */

/* ������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitResponse( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
    VG4_AnimDoExit();
  if (GetAsyncKeyState('F') & 0x8000)
    VG4_AnimFlipFullScreen();
} /* End of 'VG4_AnimUnitResponse' function */

/* ������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT_BALL *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitRender( vg4UNIT_BALL *Uni, vg4ANIM *Ani )
{
  DBL shift = Uni->Amplitude * fabs(sin(Uni->ScaleShift * (DBL)clock() / CLOCKS_PER_SEC + Uni->PhaseShift));

  SetDCBrushColor(Ani->hDC, Uni->Color);
  Ellipse(Ani->hDC, Uni->Pos.X - 5, Uni->Pos.Y - 5 - shift, Uni->Pos.X + 5, Uni->Pos.Y + 5 - shift);
} /* End of 'VG4_AnimUnitRender' function */

/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (vg4UNIT *) ��������� �� ��������� ������ ��������.
 */
vg4UNIT * VG4_UnitBallCreate( VOID )
{
  vg4UNIT_BALL *Uni;

  if ((Uni = (VOID *)VG4_AnimUnitCreate(sizeof(vg4UNIT_BALL))) == NULL)
    return NULL;
  /* ��������� ���� */
  Uni->Init = (VOID *)VG4_AnimUnitInit;
  Uni->Close = (VOID *)VG4_AnimUnitClose;
  Uni->Response = (VOID *)VG4_AnimUnitResponse;
  Uni->Render = (VOID *)VG4_AnimUnitRender;
  return (vg4UNIT *)Uni;
} /* End of 'VG4_UnitBallCreate' function */

/* END OF 'UBALL.C' FILE */
