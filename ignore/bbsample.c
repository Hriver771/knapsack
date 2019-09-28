#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _Item
{
  int b ;
  int w ;
  float bw ;
}Item ;

typedef struct _Vertex
{
  int item ;
  int benefit ;
  int weight ;
  int bound ;
  struct _Vertex *parent ;
  struct _Vertex *left ;
  struct _Vertex *right ;
}Vertex ;

int Bound( Item I[], int N, int W ) ;

void newChild(Vertex *P, Item I[], int N, int W) {
  P->right = malloc( sizeof(Vertex) ) ;
  P->left = malloc( sizeof(Vertex) ) ;
  P->right->parent = P ;
  P->left->parent = P ;

  //choose I[item]
  P->right->item = P->item + 1 ;
  P->right->benefit = P->benefit + I[P->right->item].b ;
  P->right->weight = P->weight + I[P->right->item].w ;
  P->right->bound = P->right->benefit + Bound( &I[P->right->item+1], N - P->right->item - 1 , W - P->right->weight ) ;

  //don't choose I[item]
  P->left->item = P->item + 1 ;
  P->left->benefit = P->benefit ;
  P->left->weight = P->weight ;
  P->left->bound = Bound( &I[P->right->item+1], N - P->left->item - 1, W - P->left->weight ) ;

}

void
Swap(Item *x, Item *y)
{
    Item temp;
    temp = *x;
    *x = *y;
    *y = temp;
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

int
Bound( Item I[], int N, int W )
{
  int totalB = 0 ;
  int totalW = 0 ;

  for(int i=0; totalW<W && i<N; i++)
  {
    if( I[i].w < (W-totalW) )
    {
      totalB += I[i].b ;
      totalW += I[i].w ;
    }
    else
    {
      totalB += I[i].bw * (W-totalW) ;
      totalW += W ;
    }
  }
  return totalB ;
}

//freetree()
//init

void
BranchBound( Item I[], int N, int W )
{
  Item *sortedI;
  sortedI = (Item *)malloc( sizeof(Item)*N );
  Copy(I, sortedI, N);
  Sort(sortedI, 0, N-1);

  int maxBene = 0 ;
  int totwight = 0 ;
  Vertex *root;
  root = malloc( sizeof(Vertex) ) ;
  root->item = -1 ;
  root->benefit = 0 ;
  root->weight = 0 ;
  root->bound = Bound( sortedI, N, W ) ;

  newChild(root, sortedI, N, W) ;


  printf("root: benefit = %d, weight = %d, bound = %d\n", root->benefit, root->weight, root->bound) ;
  printf("Rchild: parentI = %d, benefit = %d, weight = %d, bound = %d\n", root->right->parent->item, root->right->benefit, root->right->weight, root->right->bound);
  printf("Lchild: parentI = %d, benefit = %d, weight = %d, bound = %d\n", root->left->parent->item, root->left->benefit, root->left->weight, root->left->bound);

}

int
main()
{
  Item *sample;
  sample = (Item *)malloc( sizeof(Item)*4 );

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

  BranchBound(sample, 4, 16) ;

  //printf("%d", BranchBound(sample, 4, 16)) ;
}
