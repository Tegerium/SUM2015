/* FILENAME: ANIM.H
 * PROGRAMMER: pk2
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 08.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "def.h"

/* ������������ ���������� ��������� �������� */
#define PK2_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagpk2UNIT pk2UNIT;

/* ��������� �������� ��������� �������� */
BYTE
    Keys[256],       /* �������� ������ ���������� � ���� */
    KeysOld[256],    /* �������� ������ �� ���������� ����� */
    KeysClick[256];  /* ����� ������������ ������� */
  INT
    MsDeltaX, MsDeltaY, /* ������������� ���������� ������� ���� */
    MsX, MsY,        /* ���������� ���������� ������� ���� */
    MsWheel;         /* ��������� ������ ���� */
  BYTE
    JButs[32],       /* �������� ������ ��������� */
    JButsOld[32],    /* �������� ������ ��������� �� ���������� ����� */
    JButsClick[32],  /* ����� ������������ ������� ������ ��������� */
    JPOV;            /* ������������� POV - 0..8 */
  DBL
    JX, JY, JZ, JR, JU, JV; /* ��� ��������� (-1.0 .. 1.0*/
typedef struct
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HBITMAP hBmFrame;   /* ����� ����� */
  HDC hDC;            /* �������� ������ */

  /* ������ ��������� �������� � �� ���������� */
  pk2UNIT *Units[PK2_MAX_UNITS]; 
  INT NumOfUnits;

} pk2ANIM;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ������������� ��������:
 *     INT Id;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( pk2UNIT *Unit, pk2ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( pk2UNIT *Unit, pk2ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( pk2UNIT *Unit, pk2ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( pk2UNIT *Unit, pk2ANIM *Ani );
 */
#define pk2_UNIT_BASE_FIELDS 
  INT Size;                                         \ 
  VOID (*Init)( pk2UNIT *Unit, pk2ANIM *Ani );      \ 
  VOID (*Close)( pk2UNIT *Unit, pk2ANIM *Ani );     \
  VOID (*Response)( pk2UNIT *Unit, pk2ANIM *Ani );  \
  VOID (*Render)( pk2UNIT *Unit, pk2ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagpk2UNIT
{
  pk2_UNIT_BASE_FIELDS;
};

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������: ���.
 */
VOID pk2_AnimInit( HWND hWnd );

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID pk2_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID pk2_AnimResize( INT W, INT H );

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID pk2_AnimRender( VOID );

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID pk2_AnimCopyFrame( VOID );

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       pk2UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID pk2_AnimAddUnit( pk2UNIT *Unit );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID pk2_AnimFlipFullScreen( VOID );

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (pk2UNIT *) ��������� �� ��������� ������ ��������.
 */
pk2UNIT * pk2_AnimUnitCreate( INT Size );

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID pk2_AnimDoExit( VOID );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */
