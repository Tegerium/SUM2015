#include <stdio.h>
#include <conio.h> 

#define N 30

double A[N][N];
int P[N];
int Parity = 0, SUM, PROD, L;

void swap(int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void Determ(void)
{  
  int i;
  PROD = 1;
  for(i=0; i < N; i++)
    PROD *= A[i][P[i]];
  if(!Parity)
    SUM += PROD;
  else
    SUM -= PROD;
}

void Load( char *FileName )
{
  int i, j;
  FILE *F;
  if((F=fopen(FileName, "r")) != NULL)
  {
    fscanf(F, "%d", &L);
    for(i=0; i < L; i++)
      for(j=0; j < L; i++)
        fscanf(F, "%lf", &A[i][j]);
    fclose(F);
  }
}

void perm(int Pos, FILE *F)
{
  int i;
  if(Pos == N)
  {
    Determ();
    return;
  }
  else
    for(i = 0; i< N; i++)
    {
      Parity = !Parity;
      swap(&P[Pos], &P[i]);
      perm(Pos +1, F);
    } 
}

void main (void)
{
  int x;
  Load("Q:\0604\VG4\m1.txt");
  for(x=0; x<N; x++)
    P[x] = x + 1;
  perm(0, F);
}