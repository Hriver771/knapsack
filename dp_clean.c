#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Item
{
  int b ;
  int w ;
}Item;

int
DynamicProgramming( Item I[], int N, int W )
{
  int return_value;
  int **B;
  B = (int **)malloc( sizeof(int *)*(N+1) );
  for (int i=0; i<=N; i++) {
    B[i] = (int *)malloc(sizeof(int)*(W+1));
  }

  for (int w=0; w<=W; w++ ) {
    B[0][w] = 0;
  }
  for (int i=1; i<=N; i++) {
    B[i][0] = 0;
    for (int w=1; w<=W; w++) {
      if ( I[i-1].w <= w ) {
        if ( (I[i-1].b) + B[i-1][w-I[i-1].w] > B[i-1][w]) {
          B[i][w] = (I[i-1].b) + B[i-1][w-(I[i-1].w)];
        }
        else {
          B[i][w] = B[i-1][w];
        }
      }
      else {
        B[i][w] = B[i-1][w];
      }
    }
  }
  return_value = B[N][W];

  for (int i=0; i<=N; i++){
    free(B[i]);
  }
  free(B);

  return B[N][W];
}

int
rd( int min, int max )
{
  return ( rand() % (max - min +1) ) + min ;
}

int
main()
{
  srand( time(NULL) ) ;

  Item *items;
  int min_b = 1 ;
  int max_v = 300 ;
  int min_w = 1 ;
  int max_w = 100 ;
  int num_items[9] = { 10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000 } ;

  for ( int i=0; i<9; i++ )
  {
    items = (Item *)malloc(num_items[i]*sizeof(Item)) ;
    for ( int j=0; j<num_items[i]; j++ )
    {
      items[j].b = rd(min_b, max_v) ;
      items[j].w = rd(min_w, max_w) ;
    }
    printf("dp %d = %d\n",num_items[i],DynamicProgramming( items, num_items[i], num_items[i]*40 ) ) ;
    free(items);
  }
}
