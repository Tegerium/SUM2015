/* FILENAME: UNITS.H
 * PROGRAMMER: PK2
 * PURPOSE: Animation units samples declaration module.
 * LAST UPDATE: 10.06.2015
 */

#ifndef __UNITS_H_
#define __UNITS_H_

#include "def.h"

/* ������� �������� ������� �������� "���".
 * ���������: ���.
 * ������������ ��������:
 *   (PK2UNIT *) ��������� �� ��������� ������ ��������.
 */
PK2UNIT * PK2_UnitBallCreate( VOID );

/* ������� �������� ������� �������� "����������".
 * ���������: ���.
 * ������������ ��������:
 *   (PK2UNIT *) ��������� �� ��������� ������ ��������.
 */
PK2UNIT * PK2_UnitControlCreate( VOID );

/* ������� �������� ������� �������� "������".
 * ���������: ���.
 * ������������ ��������:
 *   (PK2UNIT *) ��������� �� ��������� ������ ��������.
 */
PK2UNIT * PK2_UnitModelCreate( VOID );

#endif /* __UNITS_H_ */

/* END OF 'UNITS.H' FILE */
