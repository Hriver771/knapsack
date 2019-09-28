#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Item
{
  int b ;
  int w ;
}Item;

int
dp( Item I[], int N, int W )
{

  int **B;

  B = (int **)malloc( sizeof(int *)*(N+1) );
  for (int i=0; i<=N; i++)
  {
    B[i] = (int *)malloc(sizeof(int)*(W+1));
  }

  for (int w=0; w<=W; w++ )
  {
    B[0][w] = 0;
  }
  for (int i=1; i<=N; i++)
  {
    B[i][0] = 0;
    // for (int w=1; w<=W; w++)
    // {
    //   if ( ((I+sizeof(Item)*(i-1))->w) <= w )
    //   {
    //     if ( ((I+sizeof(Item)*(i-1))->b) + B[i-1][w-((I+sizeof(Item)*(i-1))->w)] > B[i-1][w])
    //     {
    //       B[i][w] = ((I+sizeof(Item)*(i-1))->b) + B[i-1][w-((I+sizeof(Item)*(i-1))->w)];
    //     }
    //     else
    //     {
    //       B[i][w] = B[i-1][w];
    //     }
    //   }
    //   else
    //   {
    //     B[i][w] = B[i-1][w];
    //   }
    // }
    for (int w=1; w<=W; w++)
    {
      if ( I[i-1].w <= w )
      {
        if ( (I[i-1].b) + B[i-1][w-I[i-1].w] > B[i-1][w])
        {
          B[i][w] = (I[i-1].b) + B[i-1][w-(I[i-1].w)];
        }
        else
        {
          B[i][w] = B[i-1][w];
        }
      }
      else
      {
        B[i][w] = B[i-1][w];
      }
    }
  }
  for (int i=0; i<=N; i++) {
    for (int j=0; j<=W; j++) {
      printf("B[%d][%d]=%d\n", i, j, B[i][j]);
    }
  }
  return B[N][W];
}

int
main()
{
  Item *sample;
  sample = (Item *)malloc( sizeof(Item)*4 );

  // ( sample+sizeof(Item)*0 )->b = 4 ;
  // ( sample+sizeof(Item)*0 )->w = 3 ;
  //
  // ( sample+sizeof(Item)*1 )->b = 3 ;
  // ( sample+sizeof(Item)*1 )->w = 2 ;
  //
  // ( sample+sizeof(Item)*2 )->b = 5 ;
  // ( sample+sizeof(Item)*2 )->w = 4 ;
  //
  // ( sample+sizeof(Item)*3 )->b = 6 ;
  // ( sample+sizeof(Item)*3 )->w = 5 ;

  sample[0].b = 4;
  sample[0].w = 3;

  sample[1].b = 3;
  sample[1].w = 2;

  sample[2].b = 5;
  sample[2].w = 4;

  sample[3].b = 6;
  sample[3].w = 5;

  for (int i = 0; i <4; i++) {
    printf("%d %d\n",sample[i].b, sample[i].w);
    printf("%d %d\n", ((sample+sizeof(Item)*(i))->b), ((sample+sizeof(Item)*(i))->w));
  }

  printf("%d", dp(sample, 4, 5)) ;
}
