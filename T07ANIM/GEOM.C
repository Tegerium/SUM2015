/* FILENAME: GEOM.C
 * PROGRAMMER: PK2
 * PURPOSE: Rendering system implementation module.
 * LAST UPDATE: 13.06.2015
 */

#include <stdio.h>
#include <stdlib.h>

#include "render.h"

INT PK2_GeomAddPrim( PK2GEOM *G, PK2PRIM *Prim )
{
  PK2PRIM *new_bulk;

  if ((new_bulk = malloc(sizeof(PK2PRIM) * (G->NumOfPrimitives + 1))) == NULL)
    return -1;

  if (G->Prims != NULL)
  {
    memcpy(new_bulk, G->Prims, sizeof(PK2PRIM) * G->NumOfPrimitives);
    free(G->Prims);
  }
  G->Prims = new_bulk;

  G->Prims[G->NumOfPrimitives] = *Prim;
  return G->NumOfPrimitives++;
} /* End of 'PK2_GeomAddPrim' function */

VOID PK2_GeomFree( PK2GEOM *G )
{
  INT i;

  if (G->Prims != NULL)
  {
    for (i = 0; i < G->NumOfPrimitives; i++)
      PK2_PrimFree(&G->Prims[i]);
    free(G->Prims);
  }
  memset(G, 0, sizeof(PK2GEOM));
} /* End of 'PK2_GeomFree' function */

VOID PK2_GeomDraw( PK2GEOM *G )
{
  INT i, loc;

  glUseProgram(PK2_RndProg);
  loc = glGetUniformLocation(PK2_RndProg, "TotalParts");
  if (loc != -1)
    glUniform1f(loc, G->NumOfPrimitives);
  glUseProgram(0);

  for (i = 0; i < G->NumOfPrimitives; i++)
    if (PK2_MtlLib[G->Prims[i].MtlNo].Kt == 1)
    {
      glUseProgram(PK2_RndProg);
      loc = glGetUniformLocation(PK2_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      PK2_PrimDraw(&G->Prims[i]);
    }

  for (i = 0; i < G->NumOfPrimitives; i++)
    if (PK2_MtlLib[G->Prims[i].MtlNo].Kt != 1)
    {
      glUseProgram(PK2_RndProg);
      loc = glGetUniformLocation(PK2_RndProg, "PartNo");
      if (loc != -1)
        glUniform1f(loc, i);
      glUseProgram(0);
      PK2_PrimDraw(&G->Prims[i]);
    }
} /* End of 'PK2_GeomDraw' function */

BOOL PK2_GeomLoad( PK2GEOM *G, CHAR *FileName )
{
  FILE *F;
  INT i, j, n;
  CHAR Sign[4];
  MATR M;
  static CHAR MtlName[300];
  static CHAR
    path_buffer[_MAX_PATH],
    drive[_MAX_DRIVE],
    dir[_MAX_DIR],
    fname[_MAX_FNAME],
    ext[_MAX_EXT];

  _splitpath(FileName, drive, dir, fname, ext);

  memset(G, 0, sizeof(PK2GEOM));
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;

  M = MatrTranspose(MatrInverse(PK2_RndPrimMatrConvert));

  fread(Sign, 1, 4, F);
  if (*(DWORD *)Sign != *(DWORD *)"G3D")
  {
    fclose(F);
    return FALSE;
  }

  fread(&n, 4, 1, F);
  fread(MtlName, 1, 300, F);

  _makepath(path_buffer, drive, dir, MtlName, "");
  PK2_MtlLoad(path_buffer);

  for (i = 0; i < n; i++)
  {
    INT nv, ni, *Ind;
    PK2VERTEX *Vert;
    PK2PRIM P;

    fread(&nv, 4, 1, F);
    fread(&ni, 4, 1, F);
    fread(MtlName, 1, 300, F);

    if ((Vert = malloc(sizeof(PK2VERTEX) * nv + sizeof(INT) * ni)) == NULL)
      break;
    Ind = (INT *)(Vert + nv);

    fread(Vert, sizeof(PK2VERTEX), nv, F);
    for (j = 0; j < nv; j++)
    {
      Vert[j].P = VecMulMatr(Vert[j].P, PK2_RndPrimMatrConvert);
      Vert[j].N = VecMulMatr3(Vert[j].N, M);
    }
    fread(Ind, sizeof(INT), ni, F);

    PK2_PrimCreate(&P, PK2_PRIM_TRIMESH, nv, ni, Vert, Ind);
    P.MtlNo = PK2_MtlFind(MtlName);

    free(Vert);

    PK2_GeomAddPrim(G, &P);
  }
  fclose(F);
  PK2_RndPrimMatrConvert = MatrIdentity();
  return TRUE;
} /* End of 'PK2_GeomDraw' function */

/* END OF 'GEOM.C' FILE */