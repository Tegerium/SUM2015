/* FILENAME: PRIM.C
 * PROGRAMMER: PK2
 * PURPOSE: Primitive handle module.
 * LAST UPDATE: 13.06.2015
 */

#include "anim.h"
#include "render.h"
MATR PK2_RndPrimMatrConvert = PK2_UNIT_MATR;
VOID PK2_PrimCreate( PK2PRIM *Prim, PK2PRIM_TYPE Type,
                     INT NoofV, INT NoofI, PK2VERTEX *Vertices, INT *Indices)
{
  Prim->Type = Type;
  Prim->NumOfI = NoofI;
  glGenVertexArrays(1, &Prim->VA);
  glGenBuffers(1, &Prim->VBuf);
  glGenBuffers(1, &Prim->IBuf);
  glBindVertexArray(Prim->VA);
  glBindBuffer(GL_ARRAY_BUFFER, Prim->VBuf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(PK2VERTEX) * NoofV, Vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Prim->IBuf);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(PK2VERTEX), (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(PK2VERTEX), (VOID *)sizeof(VEC)); 
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(PK2VERTEX), (VOID *)(sizeof(VEC) + sizeof(PK2UV)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(PK2VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(PK2UV))); 
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glBindVertexArray(0);
} /* End of 'PK2_PrimCreate' function */

VOID PK2_PrimFree( PK2PRIM *Prim )
{
  glBindVertexArray(Prim->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Prim->VBuf);
  glDeleteBuffers(1, &Prim->IBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Prim->VA);

  memset(Prim, 0, sizeof(PK2PRIM));
} /* End of 'PK2_PrimFree' function */

VOID PK2_PrimDraw( PK2PRIM *Prim )
{
  INT loc;
  MATR M;

  PK2_RndMatrWorldViewProj = MatrMulMatr(MatrMulMatr(PK2_RndMatrWorld, PK2_RndMatrView), PK2_RndMatrProj);

  glLoadMatrixf(PK2_RndMatrWorldViewProj.A[0]);

  glBindVertexArray(Prim->VA);
  glUseProgram(PK2_RndProg);

  loc = glGetUniformLocation(PK2_RndProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, PK2_RndMatrWorld.A[0]);
  loc = glGetUniformLocation(PK2_RndProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, PK2_RndMatrView.A[0]);
  loc = glGetUniformLocation(PK2_RndProg, "MatrProj");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, PK2_RndMatrProj.A[0]);
  loc = glGetUniformLocation(PK2_RndProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, PK2_RndMatrWorldViewProj.A[0]);

  M = MatrTranspose(MatrInverse(MatrMulMatr(PK2_RndMatrWorld, PK2_RndMatrView)));
  loc = glGetUniformLocation(PK2_RndProg, "MatrWVInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrTranspose(MatrInverse(PK2_RndMatrWorld));
  loc = glGetUniformLocation(PK2_RndProg, "MatrWInverse");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  M = MatrMulMatr(PK2_RndMatrWorld, PK2_RndMatrView);
  loc = glGetUniformLocation(PK2_RndProg, "MatrWV");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, M.A[0]);

  loc = glGetUniformLocation(PK2_RndProg, "Time");
  if (loc != -1)
    glUniform1f(loc, PK2_Anim.Time);

  loc = glGetUniformLocation(PK2_RndProg, "Ka");
  if (loc != -1)
    glUniform3fv(loc, 1, &PK2_MtlLib[Prim->MtlNo].Ka.X);
  loc = glGetUniformLocation(PK2_RndProg, "Kd");
  if (loc != -1)
    glUniform3fv(loc, 1, &PK2_MtlLib[Prim->MtlNo].Kd.X);
  loc = glGetUniformLocation(PK2_RndProg, "Ks");
  if (loc != -1)
    glUniform3fv(loc, 1, &PK2_MtlLib[Prim->MtlNo].Ks.X);
  loc = glGetUniformLocation(PK2_RndProg, "Kp");
  if (loc != -1)
    glUniform1f(loc, PK2_MtlLib[Prim->MtlNo].Kp);
  loc = glGetUniformLocation(PK2_RndProg, "Kt");
  if (loc != -1)
    glUniform1f(loc, PK2_MtlLib[Prim->MtlNo].Kt);

  loc = glGetUniformLocation(PK2_RndProg, "IsTextureUse");
  if (PK2_MtlLib[Prim->MtlNo].TexId == 0)
    glUniform1f(loc, 0);
  else
  {
    glUniform1f(loc, 1);
    glBindTexture(GL_TEXTURE_2D, PK2_MtlLib[Prim->MtlNo].TexId);
  }

  glPrimitiveRestartIndex(0xFFFFFFFF);
  if (Prim->Type == PK2_PRIM_GRID)
    glDrawElements(GL_TRIANGLE_STRIP, Prim->NumOfI, GL_UNSIGNED_INT, NULL);
  else
    glDrawElements(GL_TRIANGLES, Prim->NumOfI, GL_UNSIGNED_INT, NULL);

  glUseProgram(0);
  glBindVertexArray(0);
} /* End of 'PK2_PrimDraw' function */

BOOL PK2_PrimCreatePlane( PK2PRIM *Prim, VEC Du, VEC Dv, INT N, INT M )
{
} /* End of 'PK2_PrimCreatePlane' function */

BOOL PK2_PrimCreateSphere( PK2PRIM *Prim, VEC C, FLT R, INT N, INT M )
{
} /* End of 'PK2_PrimCreateSphere' function */

/* END OF 'PRIM.C' FILE */