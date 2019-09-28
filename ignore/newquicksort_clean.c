#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Item
{
  int b ;
  int w ;
  float bw;
}Item;

typedef struct _Vertex
{
  int benefit;
  int wight;
  int bound;
}Vertex;

void Swap(Item *x, Item *y) {
    Item Temp;
    Temp = *x;
    *x = *y;
    *y = Temp;
}

void quickSort(Item a[], int first, int last) {
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
        quickSort(a, first, j - 1);
        quickSort(a, j + 1, last);
    }
}

int
main()
{
  Item *sample;
  sample = (Item *)malloc( sizeof(Item)*4 );
  Item *sorted;
  sorted = (Item *)malloc( sizeof(Item)*4 );

  sample[0].b = 40;
  sample[0].w = 2;
  sample[0].bw = sample[0].b / sample[0].w ;

  sample[1].b = 30;
  sample[1].w = 5;
  sample[1].bw = sample[1].b / sample[1].w ;

  sample[2].b = 50;
  sample[2].w = 10;
  sample[2].bw = sample[2].b / sample[2].w ;

  sample[3].b = 10;
  sample[3].w = 5;
  sample[3].bw = sample[3].b / sample[3].w ;

  quick_Sort(sample, 0, 3);

  for ( int i=0; i<4; i++ )
  {
    printf("%d %d %f\n",sample[i].b, sample[i].w, sample[i].bw);
  }
}
