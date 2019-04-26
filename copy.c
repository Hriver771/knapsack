#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Item
{
  int b ;
  int w ;
  float bw ;
}Item ;

void
Swap(Item *x, Item *y)
{
    Item temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void
Sort(Item a[], int first, int last)
{
    int pivot, i, j;
    if(first < last) {
        pivot = first;
        i = first;
        j = last;
        while (i < j) {
            while(a[i].bw >= a[pivot].bw && i < last)
                i++;
            while(a[j].bw < a[pivot].bw)
                j--;
            if(i < j) {
                Swap(&a[i], &a[j]);
            }
        }
        Swap(&a[pivot], &a[j]);
        Sort(a, first, j - 1);
        Sort(a, j + 1, last);
    }
}

void
Copy(Item origin[], Item new[], int N)
{
  for (int i=0; i<N; i++) {
    new[i].b = origin[i].b;
    new[i].w = origin[i].w;
    new[i].bw = origin[i].bw;
  }
}

float
Greedy( Item I[], int N, int W )
{
  Item *sortedI;
  sortedI = (Item *)malloc( sizeof(Item)*N );
  Copy(I, sortedI, N);
  Sort(sortedI, 0, N-1);

  // for ( int i=0; i<N; i++ )
  // {
  //   printf("S: %d %d %f\n",sortedI[i].b, sortedI[i].w, sortedI[i].bw) ;
  // }

  float totalB = 0 ;
  int totalW = 0 ;

  for(int i=0; totalW<W && i<N; i++)
  {
    if( sortedI[i].w < (W-totalW) )
    {
      totalB += sortedI[i].b ;
      totalW += sortedI[i].w ;
    }
    else
    {
      totalB += sortedI[i].bw * (float)(W-totalW) ;
    }
  }
  return totalB;
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
  //int num_items = 10 ;

  for ( int i=0; i<9; i++ )
  {
    items = (Item *)malloc(num_items[i]*sizeof(Item)) ;
    for ( int j=0; j<num_items[i]; j++ )
    {
      items[j].b = rd(min_b, max_v) ;
      items[j].w = rd(min_w, max_w) ;
      items[j].bw = (float)items[j].b / (float)items[j].w ;
    }
    printf("greedy %d = %f\n",num_items[i],Greedy( items, num_items[i], num_items[i]*40 ) ) ;
    free(items);
  }
  // items = (Item *)malloc(num_items*sizeof(Item)) ;
  // for ( int i=0; i<num_items; i++ )
  // {
  //   items[i].b = rd(min_b, max_v) ;
  //   items[i].w = rd(min_w, max_w) ;
  //   items[i].bw = (float)items[i].b / (float)items[i].w ;
  // }
  //printf("G %f\n", Greedy(items, num_items, num_items*40));


}
