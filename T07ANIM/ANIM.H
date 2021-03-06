/* FILENAME: ANIM.H
 * PROGRAMMER: PK2
 * PURPOSE: Animation system declaration module.
 * LAST UPDATE: 11.06.2015
 */

#ifndef __ANIM_H_
#define __ANIM_H_

#include "render.h"

/* ������������ ���������� ��������� �������� */
#define PK2_MAX_UNITS 3000

/* ���������� ���� �������� �������� "������" */
typedef struct tagPK2UNIT PK2UNIT;

/* ��������� �������� ��������� �������� */
typedef struct
{
  HWND hWnd;          /* ���� ������ */
  INT W, H;           /* ������ ���� ������ */
  HDC hDC;            /* �������� ���� ������ */
  HGLRC hGLRC;        /* �������� ���������� */

  /* ������ ��������� �������� � �� ���������� */
  PK2UNIT *Units[PK2_MAX_UNITS]; 
  INT NumOfUnits;

  /* ���������� ������������� */
  DBL
    Time,            /* ����� � �������� �� ������ �������� */
    GlobalTime,      /* ����� -"-, �� ��� ����� */
    DeltaTime,       /* ����������� ����� � �������� */
    GlobalDeltaTime, /* ����������� ����� � �������� ��� ����� */
    FPS;             /* ���������� ������ � ������� */
  BOOL
    IsPause;         /* ���� ����� */

  /* ���������� ����� */
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
} PK2ANIM;

/* ��������� �������� �������� */
extern PK2ANIM PK2_Anim;

/* ������� ���� ������� ��������:
 * - ������ ��������� ��� ������������
 *     INT Size;
 * - ������������� ��������:
 *     INT Id;
 * - ��������� �� ������� �������������
 *     VOID (*Init)( PK2UNIT *Unit, PK2ANIM *Ani );
 * - ��������� �� ������� ���������������
 *     VOID (*Close)( PK2UNIT *Unit, PK2ANIM *Ani );
 * - ��������� �� ������� ���������� ����������� ����������
 *     VOID (*Response)( PK2UNIT *Unit, PK2ANIM *Ani );
 * - ��������� �� ������� ����������
 *     VOID (*Render)( PK2UNIT *Unit, PK2ANIM *Ani );
 */
#define PK2_UNIT_BASE_FIELDS \
  INT Size;                                          \
  VOID (*Init)( PK2UNIT *Unit, PK2ANIM *Ani );       \
  VOID (*Close)( PK2UNIT *Unit, PK2ANIM *Ani );      \
  VOID (*Response)( PK2UNIT *Unit, PK2ANIM *Ani );   \
  VOID (*Render)( PK2UNIT *Unit, PK2ANIM *Ani )

/* ������� ��� ������� �������� */
struct tagPK2UNIT
{
  PK2_UNIT_BASE_FIELDS;
};

/* ���������� ���������� - ������� ��������� ������ ���� */
extern INT PK2_MouseWheel;

/* ������� ������������� ��������.
 * ���������:
 *   - ���������� ����:
 *       HWND hWnd;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL PK2_AnimInit( HWND hWnd );

/* ������� ��������������� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID PK2_AnimClose( VOID );

/* ������� ��������� ��������� ������� ������� ������.
 * ���������:
 *   - ����� ������ ������� ������:
 *       INT W, H;
 * ������������ ��������: ���.
 */
VOID PK2_AnimResize( INT W, INT H );

/* ������� ���������� ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID PK2_AnimRender( VOID );

/* ������� ������ ����� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID PK2_AnimCopyFrame( VOID );

/* ������� ���������� � ������� ������� ��������.
 * ���������:
 *   - ����������� ������ ��������:
 *       PK2UNIT *Unit;
 * ������������ ��������: ���.
 */
VOID PK2_AnimAddUnit( PK2UNIT *Unit );

/* ������� ������������ �/�� �������������� ������
 * � ������ ���������� ���������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID PK2_AnimFlipFullScreen( VOID );

/* ������� �������� ������� ��������.
 * ���������:
 *   - ������ ��������� ������� ��������:
 *       INT Size;
 * ������������ ��������:
 *   (PK2UNIT *) ��������� �� ��������� ������ ��������.
 */
PK2UNIT * PK2_AnimUnitCreate( INT Size );

/* ������� ������ �� ��������.
 * ���������: ���.
 * ������������ ��������: ���.
 */
VOID PK2_AnimDoExit( VOID );

/* ������� ��������� ����� ��������.
 * ���������:
 *   - ���� �����:
 *       BOOL NewPauseFlag;
 * ������������ ��������: ���.
 */
VOID PK2_AnimSetPause( BOOL NewPauseFlag );

#endif /* __ANIM_H_ */

/* END OF 'ANIM.H' FILE */