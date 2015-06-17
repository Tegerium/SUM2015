/* FILENAME: UMODEL.C
 * PROGRAMMER: PK2
 * PURPOSE: Model view unit handle module.
 * LAST UPDATE: 10.06.2015
 */

#include "anim.h"

typedef struct tagPK2UNIT_MODEL
{
  PK2_UNIT_BASE_FIELDS;

  PK2GEOM Model;
  PK2GEOM Geom; 
  PK2PRIM Pr;
  INT TextId;  
} PK2UNIT_MODEL;

static VOID PK2_AnimUnitInit( PK2UNIT_MODEL *Uni, PK2ANIM *Ani )
{
  PK2VERTEX V[]= 
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {5, 0}, {0, 0, 1}, {1, 0, 1, 1}},
    {{0, 1, 0}, {0, 5}, {0, 0, 1}, {1, 1, 0, 1}},
    {{1, 1, 0}, {5, 5}, {0, 0, 1}, {1, 1, 0, 1}},
  };
  INT I[] = {0, 1, 2, 2, 1, 3};
  BYTE txt[2][2][3] =
  {
    {{255, 255, 255}, {0, 0, 0}},
    {{0, 0, 0}, {255, 255, 255}}
  };

  //Uni->TextId = PK2_TextureLoad("M.BMP");

  PK2_PrimCreate(&Uni->Pr, PK2_PRIM_TRIMESH, 4, 6, V, I);

  //PK2_RndPrimMatrConvert = MatrMulMatr(MatrScale(15, 15, 15), MatrRotateX(0));
  //PK2_GeomLoad(&Uni->Model, "Q:\\Models\\Avent\\avent.G3D");

  //PK2_RndPrimMatrConvert = MatrMulMatr(MatrScale(5, 5, 5), MatrRotateX(-90));
  //PK2_GeomLoad(&Uni->Model, "Q:\\Models\\Nissan_Pathfinder\\NISPF.G3D");

  //tank
  
  PK2_RndPrimMatrConvert = MatrMulMatr(MatrScale(3, 3, 3), MatrRotateY(137.5));
  PK2_GeomLoad(&Uni->Geom, "Models\\Tiger.G3D");
 
  //PK2_RndPrimMatrConvert = MatrScale(0.30, 0.30, 0.30);
  //PK2_GeomLoad(&Uni->Geom, "COW.G3D");
} /* End of 'PK2_AnimUnitInit' function */

static VOID PK2_AnimUnitClose( PK2UNIT_MODEL *Uni, PK2ANIM *Ani )
{
  PK2_GeomFree(&Uni->Model);
  PK2_GeomFree(&Uni->Geom);
  PK2_PrimFree(&Uni->Pr);
} /* End of 'PK2_AnimUnitClose' function */

static VOID PK2_AnimUnitRender( PK2UNIT_MODEL *Uni, PK2ANIM *Ani )
{
  INT i, j;

  PK2_RndMatrView = MatrView(VecSet(8, 8, 8),
                             VecSet(0, 0, 0),
                             VecSet(0, 1, 0));

  if (Ani->KeysClick['W'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if (Ani->KeysClick['Q'])
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_DEPTH_TEST);

  for (i = 0; i < 1; i++)
    for (j = 0; j < 1; j++)
    {
      PK2_RndMatrWorld =
        MatrMulMatr(MatrMulMatr(MatrMulMatr(
          MatrTranslate(Ani->JX * 59, Ani->JY * 88, 0),
          MatrScale(0.1, 0.1, 0.1)),
          MatrRotateY(30 * Ani->Time + Ani->JR * 180)),
          MatrTranslate(j * 1.30, 0, i * 1.30 + 100 * Ani->JZ));
      glColor3d(i & 1, j & 1, 1 - ((i & 1) + (j & 1)) / 2);
      PK2_GeomDraw(&Uni->Model);
    }
  PK2_RndMatrWorld = MatrRotateY(0 * 30 * Ani->Time);
  //MatrMulMatr(MatrRotateZ(30 * sin(Ani->Time * 3.0)), MatrRotateY(30 * Ani->Time));
  PK2_GeomDraw(&Uni->Geom);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Uni->TextId);
  PK2_PrimDraw(&Uni->Pr);
} /* End of 'PK2_AnimUnitRender' function */

PK2UNIT * PK2_UnitModelCreate( VOID )
{
  PK2UNIT_MODEL *Uni;

  if ((Uni = (VOID *)PK2_AnimUnitCreate(sizeof(PK2UNIT_MODEL))) == NULL)
    return NULL;
  Uni->Init = (VOID *)PK2_AnimUnitInit;
  Uni->Close = (VOID *)PK2_AnimUnitClose;
  Uni->Render = (VOID *)PK2_AnimUnitRender;
  return (PK2UNIT *)Uni;
} /* End of 'PK2_UnitModelCreate' function */

/* END OF 'UMODEL.C' FILE */