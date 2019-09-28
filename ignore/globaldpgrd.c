#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Item
{
  int b ;
  int w ;
  float bw ;
}Item ;

Item *ITEMLIST = NULL;
Item *BBITEMLIST = NULL ;
Item *GRDITEMLIST = NULL ;
int NUMOFITEMS = 0 ;
int TOTALWEIGHT = 0 ;

void
Swap( Item *x, Item *y )
{
    Item temp ;
    temp = *x ;
    *x = *y ;
    *y = temp ;
}

void
Sort( Item a[], int first, int last )
{
    int pivot, i, j ;
    if(first < last)
    {
        pivot = first ;
        i = first ;
        j = last ;
        while (i < j)
        {
            while(a[i].bw >= a[pivot].bw && i < last)
                i++ ;
            while(a[j].bw < a[pivot].bw)
                j-- ;
            if(i < j)
            {
                Swap(&a[i], &a[j]) ;
            }
        }
        Swap(&a[pivot], &a[j]) ;
        Sort(a, first, j - 1) ;
        Sort(a, j + 1, last) ;
    }
}

void
Copy(Item origin[], Item new[], int N)
{
  for (int i=0; i<N; i++)
  {
    new[i].b = origin[i].b ;
    new[i].w = origin[i].w ;
    new[i].bw = origin[i].bw ;
  }
}

float
Greedy()
{
  GRDITEMLIST = (Item *)malloc( sizeof(Item)*NUMOFITEMS ) ;
  Copy(ITEMLIST, GRDITEMLIST, NUMOFITEMS) ;
  Sort(GRDITEMLIST, 0, NUMOFITEMS-1) ;

  float sumB = 0 ;
  int sumW = 0 ;

  for(int i=0; sumW<TOTALWEIGHT && i<NUMOFITEMS; i++)
  {
    if( GRDITEMLIST[i].w < (TOTALWEIGHT-sumW) )
    {
      sumB += GRDITEMLIST[i].b ;
      sumW += GRDITEMLIST[i].w ;
    }
    else
    {
      sumB += GRDITEMLIST[i].bw * (float)(TOTALWEIGHT-sumW) ;
      sumW += TOTALWEIGHT ;
    }
  }
  free(GRDITEMLIST) ;
  return sumB ;
}

int
DynamicProgramming()
{
  int result ;

  int **B ;
  B = (int **)malloc( sizeof(int *)*(NUMOFITEMS+1) ) ;
  for (int i=0; i<=NUMOFITEMS; i++)
  {
    B[i] = (int *)malloc( sizeof(int)*(TOTALWEIGHT+1) ) ;
  }

  for ( int w=0; w<=TOTALWEIGHT; w++ )
  {
    B[0][w] = 0 ;
  }
  for ( int i=1; i<=NUMOFITEMS; i++ )
  {
    B[i][0] = 0 ;
    for ( int w=1; w<=TOTALWEIGHT; w++ )
    {
      if ( ITEMLIST[i-1].w <= w )
      {
        if ( ITEMLIST[i-1].b + B[i-1][w-ITEMLIST[i-1].w] > B[i-1][w] )
        {
          B[i][w] = ITEMLIST[i-1].b + B[i-1][w-(ITEMLIST[i-1].w)] ;
        }
        else
        {
          B[i][w] = B[i-1][w] ;
        }
      }
      else
      {
        B[i][w] = B[i-1][w] ;
      }
    }
  }
  result = B[NUMOFITEMS][TOTALWEIGHT] ;

  for ( int i=0; i<=NUMOFITEMS; i++ )
  {
    free(B[i]) ;
  }
  free(B) ;

  return result ;
}

int
Random( int min, int max )
{
  return ( rand() % (max - min +1) ) + min ;
}

int
main()
{
  srand( time(NULL) ) ;
  int min_b = 1 ;
  int max_v = 300 ;
  int min_w = 1 ;
  int max_w = 100 ;
  int num_items[9] = { 10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000 } ;

  for ( int i=0; i<9; i++ )
  {
    ITEMLIST = (Item *)malloc(num_items[i]*sizeof(Item)) ;
    for ( int j=0; j<num_items[i]; j++ )
    {
      ITEMLIST[j].b = Random(min_b, max_v) ;
      ITEMLIST[j].w = Random(min_w, max_w) ;
      ITEMLIST[j].bw = (float)ITEMLIST[j].b / (float)ITEMLIST[j].w ;
    }
    NUMOFITEMS = num_items[i] ;
    TOTALWEIGHT = num_items[i]*40 ;

    printf("greedy  %d = %f\n",num_items[i],Greedy() ) ;
    printf("dp      %d = %d\n",num_items[i],DynamicProgramming() ) ;
    free(ITEMLIST) ;
  }
  return 0;
}
