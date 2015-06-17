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

/* Матрицы */
extern MATR
  PK2_RndMatrWorld,
  PK2_RndMatrView,
  PK2_RndMatrProj,
  PK2_RndMatrWorldViewProj;

/* Матрица изменения примитива при создании */
extern MATR PK2_RndPrimMatrConvert;

/* Параметры проецирования */
extern DBL
  PK2_RndWp, PK2_RndHp,    /* размеры обрасти проецирования */
  PK2_RndProjDist;         /* расстояние до плоскости проекции */

/* Шейдер по умолчанию */
extern UINT PK2_RndProg;

/* Функция загрузки шейдеров для одной программы.
 * АРНУМЕНТЫ:
 *   - префикс имени файла:
 *       CHAR *FileNamePrefix;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (UINT) номер загруженной программы.
 */
UINT PK2_ShaderLoad( CHAR *FileNamePrefix );

/* Функция уничтодения шейдеров для программы.
 * АРНУМЕНТЫ:
 *   - номер программы:
 *       UINT PrgId;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_ShaderFree( UINT PrgId );

/***
 * Работа с материалами
 ***/

/* Тип задания материала поверхности */
typedef struct tagPK2MATERIAL
{
  /* Имя материала */
  CHAR Name[300];

  /* Коэффициенты отражения */
  VEC Ka, Kd, Ks;
  FLT Kp; /* Степень "бликовости" */

  /* Коэффициент прозрачности */
  FLT Kt;

  /* Текстура поверхности */
  INT TexId;
} PK2MATERIAL;

/* Глобальная таблица материалов */
#define PK2_MATERIAL_MAX 300
extern INT PK2_MtlLibSize;
extern PK2MATERIAL PK2_MtlLib[PK2_MATERIAL_MAX];

/* Функция добавления материала в библиотеку.
 * АРГУМЕНТЫ:
 *   - структура с памаметрами материала:
 *       PK2MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер добавленного материала (0 при ошибке).
 */
INT PK2_MtlAdd( PK2MATERIAL *Mtl );

/* Функция поиска материала по имени.
 * АРГУМЕНТЫ:
 *   - имя материала:
 *       CHAR *Name;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) порядковый номер найденного материала (0 при ошибке).
 */
INT PK2_MtlFind( CHAR *Name );

/* Функция загрузки материала из файла (*.MTL).
 * АРГУМЕНТЫ:
 *   - имя файла материала:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) количество загруженных материалов.
 */
INT PK2_MtlLoad( CHAR *FileName );

/***
 * Работа с примитивами
 ***/

/* Виды примитивов */
typedef enum tagPK2PRIM_TYPE
{
  PK2_PRIM_TRIMESH,  /* примитив - набор треугольников */
  PK2_PRIM_GRID      /* регулярная сетка треугольников (через triangle strip) */
} PK2PRIM_TYPE;

/* Тип хранения примитива */
typedef struct tagPK2PRIM
{
  PK2PRIM_TYPE Type; /* Тип примитива (PK2_PRIM_***) */
  INT
    VA,          /* массив вершин */
    VBuf, IBuf,  /* буфера вершин и индексов */
    NumOfI,      /* количество индексов для вывода примитива */
    MtlNo;       /* номер материала из библиотеки */
} PK2PRIM;

/* Тип хранения текстурных координат */
typedef struct tagPK2UV
{
  FLT U, V; /* текстурные координаты */
} PK2UV;

/* Функция задания текстурных координат.
 * АРГУМЕНТЫ:
 *   - новые значени координат:
 *       FLT U, V;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (PK2UV) построенные координаты.
 */
__inline PK2UV UVSet( FLT U, FLT V )
{
  PK2UV UV = {U, V};

  return UV;
} /* End of 'UVSet' function */

/* Тип хранения одной вершины паримитива */
typedef struct tagPK2VERTEX
{
  VEC P;    /* позиция вершины */
  PK2UV T;  /* текстурные координаты вершины */
  VEC N;    /* нормаль в вершине */
  COLOR C;  /* цвет вершины */
} PK2VERTEX;

/* Функция создания примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       PK2PRIM *Prim;
 *   - тип примитива (PK2_PRIM_***):
 *       PK2PRIM_TYPE Type;
 *   - количество вершин и индексов:
 *       INT NoofV, NoofI;
 *   - массив вершин:
 *       PK2VERTEX *Vertices;
 *   - массив индексов:
 *       INT *Indices;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_PrimCreate( PK2PRIM *Prim, PK2PRIM_TYPE Type,
                     INT NoofV, INT NoofI, PK2VERTEX *Vertices, INT *Indices);

/* Функция удаления примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       PK2PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_PrimFree( PK2PRIM *Prim );

/* Функция отрисовки примитива.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       PK2PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_PrimDraw( PK2PRIM *Prim );

/* Функция создания примитива плоскость.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       PK2PRIM *Prim;
 *   - касательные вектора-стороны:
 *       VEC Du, Dv;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL PK2_PrimCreatePlane( PK2PRIM *Prim, VEC Du, VEC Dv, INT N, INT M );

/* Функция создания примитива сфера.
 * АРГУМЕНТЫ:
 *   - указатель на примитив:
 *       PK2PRIM *Prim;
 *   - центр сферы:
 *       VEC С;
 *   - радиус сферы:
 *       FLT R;
 *   - разбиение:
 *       INT N, M;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе FALSE.
 */
BOOL PK2_PrimCreateSphere( PK2PRIM *Prim, VEC C, FLT R, INT N, INT M );

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - структура объекта для загрузки:
 *       PK2PRIM *GObj;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, FALSE иначе.
 */
BOOL PK2_PrimLoad( PK2PRIM *GObj, CHAR *FileName );

/***
 * Работа с геометрическими объектами
 ***/

/* Геометрический объект - набор примитивов */
typedef struct tagPK2GEOM
{
  INT NumOfPrimitives; /* количество примитивов */
  PK2PRIM *Prims;      /* массив примитивов */
} PK2GEOM;

/* Функция добавления примитива к геометрическому объекту.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       PK2GEOM *G;
 *   - указатель на добавляемый примитив:
 *       PK2PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива в массиве (-1 при ошибке).
 */
INT PK2_GeomAddPrim( PK2GEOM *G, PK2PRIM *Prim );

/* Функция освобождения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       PK2GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_GeomFree( PK2GEOM *G );

/* Функция отображения геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       PK2GEOM *G;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ: Нет.
 */
VOID PK2_GeomDraw( PK2GEOM *G );

/* Функция загрузки геометрического объекта из G3D файла.
 * АРГУМЕНТЫ:
 *   - указатель на геометрический объект:
 *       PK2GEOM *G;
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе, иначе - FALSE.
 */
BOOL PK2_GeomLoad( PK2GEOM *G, CHAR *FileName );

/* Функция загрузки текстуры.
 * АРГУМЕНТЫ:
 *   - имя файла:
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT ) идентификатор OpenGL для текстуры.
 */
INT PK2_TextureLoad( CHAR *FileName );

#endif /* __RENDER_H_ */

/* END OF 'RENDER.H' FILE */