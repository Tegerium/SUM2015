/* FILENAME: RENDER.H
 * PROGRAMMER: PK2
 * PURPOSE: Rendering system declaration module.
 * LAST UPDATE: 13.06.2015
 */

#ifndef __RENDER_H_
#define __RENDER_H_

#include "vec.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

/* ������� */
extern MATR
  PK2_RndMatrWorld,
  PK2_RndMatrView,
  PK2_RndMatrProj,
  PK2_RndMatrWorldViewProj;

/* ������� ��������� ��������� ��� �������� */
extern MATR PK2_RndPrimMatrConvert;

/* ��������� ������������� */
extern DBL
  PK2_RndWp, PK2_RndHp,    /* ������� ������� ������������� */
  PK2_RndProjDist;         /* ���������� �� ��������� �������� */

/* ������ �� ��������� */
extern UINT PK2_RndProg;

/* ������� �������� �������� ��� ����� ���������.
 * ���������:
 *   - ������� ����� �����:
 *       CHAR *FileNamePrefix;
 * ������������ ��������:
 *   (UINT) ����� ����������� ���������.
 */
UINT PK2_ShaderLoad( CHAR *FileNamePrefix );

/* ������� ����������� �������� ��� ���������.
 * ���������:
 *   - ����� ���������:
 *       UINT PrgId;
 * ������������ ��������: ���.
 */
VOID PK2_ShaderFree( UINT PrgId );

/***
 * ������ � �����������
 ***/

/* ��� ������� ��������� ����������� */
typedef struct tagPK2MATERIAL
{
  /* ��� ��������� */
  CHAR Name[300];

  /* ������������ ��������� */
  VEC Ka, Kd, Ks;
  FLT Kp; /* ������� "����������" */

  /* ����������� ������������ */
  FLT Kt;

  /* �������� ����������� */
  INT TexId;
} PK2MATERIAL;

/* ���������� ������� ���������� */
#define PK2_MATERIAL_MAX 300
extern INT PK2_MtlLibSize;
extern PK2MATERIAL PK2_MtlLib[PK2_MATERIAL_MAX];

/* ������� ���������� ��������� � ����������.
 * ���������:
 *   - ��������� � ����������� ���������:
 *       PK2MATERIAL *Mtl;
 * ������������ ��������:
 *   (INT) ���������� ����� ������������ ��������� (0 ��� ������).
 */
INT PK2_MtlAdd( PK2MATERIAL *Mtl );

/* ������� ������ ��������� �� �����.
 * ���������:
 *   - ��� ���������:
 *       CHAR *Name;
 * ������������ ��������:
 *   (INT) ���������� ����� ���������� ��������� (0 ��� ������).
 */
INT PK2_MtlFind( CHAR *Name );

/* ������� �������� ��������� �� ����� (*.MTL).
 * ���������:
 *   - ��� ����� ���������:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT) ���������� ����������� ����������.
 */
INT PK2_MtlLoad( CHAR *FileName );

/***
 * ������ � �����������
 ***/

/* ���� ���������� */
typedef enum tagPK2PRIM_TYPE
{
  PK2_PRIM_TRIMESH,  /* �������� - ����� ������������� */
  PK2_PRIM_GRID      /* ���������� ����� ������������� (����� triangle strip) */
} PK2PRIM_TYPE;

/* ��� �������� ��������� */
typedef struct tagPK2PRIM
{
  PK2PRIM_TYPE Type; /* ��� ��������� (PK2_PRIM_***) */
  INT
    VA,          /* ������ ������ */
    VBuf, IBuf,  /* ������ ������ � �������� */
    NumOfI,      /* ���������� �������� ��� ������ ��������� */
    MtlNo;       /* ����� ��������� �� ���������� */
} PK2PRIM;

/* ��� �������� ���������� ��������� */
typedef struct tagPK2UV
{
  FLT U, V; /* ���������� ���������� */
} PK2UV;

/* ������� ������� ���������� ���������.
 * ���������:
 *   - ����� ������� ���������:
 *       FLT U, V;
 * ������������ ��������:
 *   (PK2UV) ����������� ����������.
 */
__inline PK2UV UVSet( FLT U, FLT V )
{
  PK2UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* ��� �������� ����� ������� ���������� */
typedef struct tagPK2VERTEX
{
  VEC P;    /* ������� ������� */
  PK2UV T;  /* ���������� ���������� ������� */
  VEC N;    /* ������� � ������� */
  COLOR C;  /* ���� ������� */
} PK2VERTEX;

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       PK2PRIM *Prim;
 *   - ��� ��������� (PK2_PRIM_***):
 *       PK2PRIM_TYPE Type;
 *   - ���������� ������ � ��������:
 *       INT NoofV, NoofI;
 *   - ������ ������:
 *       PK2VERTEX *Vertices;
 *   - ������ ��������:
 *       INT *Indices;
 * ������������ ��������: ���.
 */
VOID PK2_PrimCreate( PK2PRIM *Prim, PK2PRIM_TYPE Type,
                     INT NoofV, INT NoofI, PK2VERTEX *Vertices, INT *Indices);

/* ������� �������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       PK2PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID PK2_PrimFree( PK2PRIM *Prim );

/* ������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       PK2PRIM *Prim;
 * ������������ ��������: ���.
 */
VOID PK2_PrimDraw( PK2PRIM *Prim );

/* ������� �������� ��������� ���������.
 * ���������:
 *   - ��������� �� ��������:
 *       PK2PRIM *Prim;
 *   - ����������� �������-�������:
 *       VEC Du, Dv;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL PK2_PrimCreatePlane( PK2PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* ������� �������� ��������� �����.
 * ���������:
 *   - ��������� �� ��������:
 *       PK2PRIM *Prim;
 *   - ����� �����:
 *       VEC �;
 *   - ������ �����:
 *       FLT R;
 *   - ���������:
 *       INT N, M;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� FALSE.
 */
BOOL PK2_PrimCreateSphere( PK2PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* ������� �������� ��������������� �������.
 * ���������:
 *   - ��������� ������� ��� ��������:
 *       PK2PRIM *GObj;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, FALSE �����.
 */
BOOL PK2_PrimLoad( PK2PRIM *GObj, CHAR *FileName );

/***
 * ������ � ��������������� ���������
 ***/

/* �������������� ������ - ����� ���������� */
typedef struct tagPK2GEOM
{
  INT NumOfPrimitives; /* ���������� ���������� */
  PK2PRIM *Prims;      /* ������ ���������� */
} PK2GEOM;

/* ������� ���������� ��������� � ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       PK2GEOM *G;
 *   - ��������� �� ����������� ��������:
 *       PK2PRIM *Prim;
 * ������������ ��������:
 *   (INT) ����� ������������ ��������� � ������� (-1 ��� ������).
 */
INT PK2_GeomAddPrim( PK2GEOM *G, PK2PRIM *Prim );

/* ������� ������������ ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       PK2GEOM *G;
 * ������������ ��������: ���.
 */
VOID PK2_GeomFree( PK2GEOM *G );

/* ������� ����������� ��������������� �������.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       PK2GEOM *G;
 * ������������ ��������: ���.
 */
VOID PK2_GeomDraw( PK2GEOM *G );

/* ������� �������� ��������������� ������� �� G3D �����.
 * ���������:
 *   - ��������� �� �������������� ������:
 *       PK2GEOM *G;
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������, ����� - FALSE.
 */
BOOL PK2_GeomLoad( PK2GEOM *G, CHAR *FileName );

/* ������� �������� ��������.
 * ���������:
 *   - ��� �����:
 *       CHAR *FileName;
 * ������������ ��������:
 *   (INT ) ������������� OpenGL ��� ��������.
 */
INT PK2_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */