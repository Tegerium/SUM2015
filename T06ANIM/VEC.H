#include <math.h>
#define PI 3.14159265358979323846
#define D2R(A) ((A) * PI / 180.0)


typedef double DBL;
typedef struct tagVEC
{
  DBL X, Y, Z;
} VEC;
typedef struct tagMATR
{
  DBL A[4][4];
} MATR;

__inline MATR MatrTranslate( DBL Dx, DBL Dy, DBL Dz )
{
  MATR M =
  {
    {
      { 1,  0,  0, 0},
      { 0,  1,  0, 0},
      { 0,  0,  1, 0},
      {Dx, Dy, Dz, 1}
    }
  };

  return M;
}

// *********MATR MatrScale( DBL Sx, DBL Sy, DBL Sz );
__inline MATR MatrRotateX( DBL AngleDegree )
{
  DBL a = D2R(AngleDegree), si = sin(a), co = cos(a);
  MATR M = 
  {
    {
      { 1,  0,  0, 0 },
      { 0, co, si, 0 },
      { 0,-si, co, 0 },
      { 0,  0,  0, 1 }
    }
  };
  return M;
}

__inline MATR MatrRotateY( DBL AngleDegree )
{
  DBL a = D2R(AngleDegree), si = sin(a), co = cos(a);
  MATR M = 
  {
    {
      { co, 0,-si, 0 },
      {  0, 1,  0, 0 },
      { si, 0, co, 0 },
      {  0, 0,  0, 1 }
    }
  };
  return M;
}

__inline MATR MatrRotateZ( DBL AngleDegree )
{
  DBL a = D2R(AngleDegree), si = sin(a), co = cos(a);
  MATR M = 
  {                 
    {
      { co, si, 0, 0 },
      {-si, co, 0, 0 },
      {  0, 0,  1, 0 },
      {  0, 0,  0, 1 }
    }
  };
  return M;
}
__inline MATR MatrRotate( DBL AngleDegree, DBL X, DBL Y, DBL Z )
{
  DBL a, si, co, len;
  MATR M;
  a = D2R(AngleDegree);
  si = sin(a);
  co = cos(a);
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
  X *= si;
  Y *= si;
  Z *= si;

  M.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  M.A[0][1] = 2 * X * Y - 2 * co * Z;
  M.A[0][2] = 2 * co * Y + 2 * X * Z;
  M.A[0][3] = 0;

  M.A[1][0] = 2 * X * Y + 2 * co * Z;
  M.A[1][1] = 1 - 2 * (X * X + Z * Z);
  M.A[1][2] = 2 * Y * Z - 2 * co * X;
  M.A[1][3] = 0;

  M.A[2][0] = 2 * X * Z - 2 * co * Y;
  M.A[2][1] = 2 * co * X + 2 * Y * Z;
  M.A[2][2] = 1 - 2 * (X * X + Y * Y);
  M.A[2][3] = 0;

  M.A[3][0] = 0;
  M.A[3][1] = 0;
  M.A[3][2] = 0;
  M.A[3][3] = 1;
  return M;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR M;
  int i, j, k;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (M.A[i][j] = 0, k = 0; k < 4; k++)
        M.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return M;
}

//***********MATR MatrTranspose( MATR M );

__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                            DBL A21, DBL A22, DBL A23,
                            DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 +
        -A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]);
}

__inline MATR MatrIdentity( VOID )
{
  MATR M =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}

__inline MATR MatrInverse( MATR M )
{
  int i, j;
  MATR r;
  DBL det = MatrDeterm(M);
  INT p[4][3] =
  {
    {1, 2, 3},
    {0, 2, 3},
    {0, 1, 3},
    {0, 1, 2}
  };

  if (det == 0)
    return MatrIdentity();

/*  r.A[0][0] = MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] = MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] = MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] = MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][1] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[2][1], M.A[2][2], M.A[2][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[2][0], M.A[2][2], M.A[2][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[2][0], M.A[2][1], M.A[2][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[2][0], M.A[2][1], M.A[2][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][2] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[1][0], M.A[1][2], M.A[1][3],
                            M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[1][0], M.A[1][1], M.A[1][2],
                            M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.A[0][3] = MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                            M.A[1][1], M.A[1][2], M.A[1][3],
                            M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] = MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                            M.A[1][0], M.A[1][2], M.A[1][3],
                            M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                            M.A[1][0], M.A[1][1], M.A[1][3],
                            M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] = MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                            M.A[1][0], M.A[1][1], M.A[1][2],
                            M.A[2][0], M.A[2][1], M.A[2][2]) / det;
*/
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)  
      r.A[j][i] =
       ( 1 - ( ( i + j ) % 2) * 2 ) * MatrDeterm3x3(M.A[p[i][0]][p[j][0]], M.A[p[i][0]][p[j][1]], 
                      M.A[p[i][0]][p[j][2]], M.A[p[i][1]][p[j][0]], M.A[p[i][1]][p[j][1]], 
                      M.A[p[i][1]][p[j][2]], M.A[p[i][2]][p[j][0]], M.A[p[i][2]][p[j][1]], 
                      M.A[p[i][2]][p[j][2]]) / det;

  return r;
}

//Векторы:

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC r = {X, Y, Z};

  return r;
}

__inline VEC VecAddVec( VEC A, VEC B )
{
  return VecSet(A.X + B.X, A.Y + B.Y, A.Z + B.Z);
}
VEC VecSubVec( VEC A, VEC B );
VEC VecMulNum( VEC A, DBL N );
VEC VecDivNum( VEC A, DBL N );
__inline VEC VecNeg( VEC A )
{
  return VecSet(-A.X, -A.Y, -A.Z);
}
//******************DBL VecDotVec( VEC A, VEC B );
//******************VEC VecCrossVec( VEC A, VEC B );
//******************DBL VecLen2( VEC V );
//******************DBL VecLen( VEC V );
//******************VEC VecNormalize( VEC V );// -- приведение к 1-ой длине

//!!! все функции делаем ВСТРАИВАЕМЫМИ (INLINE):

//в VisualStudio __inline - в файле *.H можно писать реализацию функции и она доступна только в файле включения:

//VEC.H

//#ifndef ...

__inline DBL VecDotVec( VEC A, VEC B )
{
  return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
}

//!!!
__inline VEC PointTransform( VEC V, MATR M )
{
  return VecSet(
    V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
    V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
    V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}
//******************VEC VectorTransform( VEC V, MATR M );
/*!!! для нормалей:
MATR Q = MatrTranspose(MatrInverse(M));
N1 = VectorTransform(N, Q);

//!!! НАСЧЕТ ОПТИМИЗАЦИИ ВЫЧИСЛЕНИЯ sin/cos:
*/
#define DEGREE2RADIANL 0.01745329251994329576L
/* Multiplyer degree to radian constant */
long double MultiplierDegree2Radian = DEGREE2RADIANL;

__inline VEC VecRotateZ( VEC V, DBL AngleInDegree )
{
  DBL sine, cosine, tmp;

  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian
    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    fstp cosine /* cos -> cosine */
    fstp sine   /* sin -> sine */
  }
  tmp = V.X * cosine - V.Y * sine;
  V.Y = V.X * sine + V.Y * cosine;
  V.X = tmp;
  return V;
} /* End of 'VecRotateZ' function */

__inline MATR MatrSetRotateZ( DBL AngleInDegree )
{
  MATR M = MatrIdentity();
  __asm {
    /* FST(0) Angle (from degree to radian) */
    fld  AngleInDegree
    fmul MultiplierDegree2Radian    
    /* FST(0) - cos, FST(1) - sin */
    fsincos
    /* cos ->A[0][0] A[1][1] */
    fld  st(0)
    fstp M.A + 0; 8bytes * (0стр * 4ширина + 0ст)
    fstp M.A + 40; 8bytes * (1стр * 4ширина + 1ст)
                                                                               
    /* sin ->A[0][1], -sin A[1][0] */
    fld  st(0)
    fstp M.A + 8; 8bytes * (0стр * 4ширина + 1ст)
    fldz       /* push 0 (st(0) = 0) */
    fsubr      /* st(0) = 0 - sin() */
    fstp M.A + 32; 8bytes * (1стр * 4ширина + 0ст)
  }
  return M;
}
/*
  MATR M = MatrIdentity();
  DBL sine, cosine;
  __asm {
    // FST(0) Angle (from degree to radian) 
    fld  AngleInDegree
    fmul MultiplierDegree2Radian    
    // FST(0) - cos, FST(1) - sin 
    fsincos      
    fstp cosine // cos -> cosine 
    fstp sine   // sin -> sine 
  }
  M.A[0][0] =  M.A[1][1] = cosine;
  M.A[0][1] = sine;
  M.A[1][0] = -sine;
  return M;    */