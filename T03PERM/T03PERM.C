#include <stdio.h>

#define N 7

int P[N];
int Parity = 0;

void swap(int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int count_parity (int *P)
{
  int i, j, cnt = 0;
  for( i = 0; i < N - 1; i++ )
    for( j = i + 1; j < N; j++ )
      cnt += P[i] > P[j];
  return cnt;
}

void perm(int Pos, FILE *F)
{
  int i, x, save;
  if(Pos == N)
  {
    for(i=0; i<N; i++)
      fprintf(F ,"%d ", P[i]);
    fprintf(F, " - %s\n", Parity ? "odd" : "even");
    return;
  }   
  save = Parity;
  perm(Pos +1, F);
  for(i = Pos +1; i< N; i++)
  {
    Parity = !Parity;
    swap(&P[Pos], &P[i]);
    perm(Pos +1, F);
  }
  Parity = save;
  x = P[Pos];
  for(i = Pos +1; i< N; i++)
    P[i - 1] = P[i] ;
  P[N-1] = x;
}

void main (void)
{
  int x;
  FILE *F;
  
  if((F=fopen("Nex.log", "w`")) == NULL)
    return;
  fclose(F);
  if((F=fopen("Nex.log", "a")) == NULL)
    return;
  for(x=0; x<N; x++)
    P[x] = x + 1;
  
  perm(0, F);
}