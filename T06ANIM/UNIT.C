/* FILENAME: UNIT.C
 * PROGRAMMER: VG4
 * PURPOSE: Animation unit handle module.
 * LAST UPDATE: 08.06.2015
 */

#include <string.h>

#include "anim.h"

/* ������� ��-��������� ������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitInit( vg4UNIT *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_AnimUnitInit' function */

/* ������� ��-��������� ��������������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitClose( vg4UNIT *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_AnimUnitClose' function */

/* ������� ��-��������� ���������� ����������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitResponse( vg4UNIT *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_AnimUnitResponse' function */

/* ������� ��-��������� ���������� ������� ��������.
 * ���������:
 *   - ��������� �� "����" - ��� ������ ��������:
 *       vg4UNIT *Uni;
 *   - ��������� �� �������� ��������:
 *       vg4ANIM *Ani;
 * ������������ ��������: ���.
 */
static VOID VG4_AnimUnitRender( vg4UNIT *Uni, vg4ANIM *Ani )
{
} /* End of 'VG4_AnimUnitRender' function */

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (vg4UNIT *) ��������� �� ��������� ������ ��������.
 */
vg4UNIT * VG4_AnimUnitCreate( INT Size )
{
  vg4UNIT *Uni;

  if (Size < sizeof(vg4UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* ��������� ���� ��-��������� */
  Uni->Size = Size;
  Uni->Init = VG4_AnimUnitInit;
  Uni->Close = VG4_AnimUnitClose;
  Uni->Response = VG4_AnimUnitResponse;
  Uni->Render = VG4_AnimUnitRender;
  return Uni;
} /* End of 'VG4_AnimUnitCreate' function */

/* END OF 'UNIT.C' FILE */
