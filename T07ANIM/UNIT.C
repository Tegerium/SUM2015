/* FILENAME: UNIT.C
 * PROGRAMMER: PK2
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       PK2UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       PK2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PK2_AnimUnitInit( PK2UNIT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       PK2UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       PK2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PK2_AnimUnitClose( PK2UNIT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       PK2UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       PK2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PK2_AnimUnitResponse( PK2UNIT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       PK2UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       PK2ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID PK2_AnimUnitRender( PK2UNIT *Uni, PK2ANIM *Ani )
{
} /* End of 'PK2_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (PK2UNIT *) ��������� �� ��������� ������ ��������.
 */
PK2UNIT * PK2_AnimUnitCreate( INT Size )
{
  PK2UNIT *Uni;

  if (Size < sizeof(PK2UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* ��������� ���� ��-��������� */
  Uni->Size = Size;
  Uni->Init = PK2_AnimUnitInit;
  Uni->Close = PK2_AnimUnitClose;
  Uni->Response = PK2_AnimUnitResponse;
  Uni->Render = PK2_AnimUnitRender;
  return Uni;
} /* End of 'PK2_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
