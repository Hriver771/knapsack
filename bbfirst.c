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
  struct _Vertex *link ;
}Vertex ;

Item *ITEMLIST = NULL;
Item *BBITEMLIST = NULL ;
Item *GRDITEMLIST = NULL ;
int NUMOFITEMS = 0 ;
int TOTALWEIGHT = 0 ;
int MAXBENEFIT = 0;
Vertex *PROMISINGLIST = NULL ;

int findBound( Item I[], int N, int W ) {
  int sumB = 0 ;
  int sumW = 0 ;

  for(int i=0; sumW<W && i<N; i++)
  {
    if( I[i].w < (W-sumW) )
    {
      sumB += I[i].b ;
      sumW += I[i].w ;
    }
    else
    {
      sumB += I[i].bw * (W-sumW) ;
      sumW += W ;
    }
  }
  return sumB ;
}

int
isPromising(Vertex v)
{
  return (v.bound > MAXBENEFIT) && (v.weight <= TOTALWEIGHT) ;
}

Vertex
popPromising()
{
  Vertex *tmp;
  Vertex pop;

	if(PROMISINGLIST != NULL)
	{
		tmp = PROMISINGLIST;
    pop = *tmp ;
		PROMISINGLIST = PROMISINGLIST->link;
		free(tmp);
    return pop;
	}
}

void
insertPromising( Vertex v )
{
  Vertex *tmp ;
  Vertex *q ;

  if(v.benefit > MAXBENEFIT) {
    MAXBENEFIT = v.benefit ;
  }

  tmp = (Vertex *)malloc(sizeof(Vertex));
  *tmp = v ;


  if( PROMISINGLIST == NULL || v.bound > PROMISINGLIST->bound )
	{
		tmp->link = PROMISINGLIST;
		PROMISINGLIST = tmp;
	}
	else
	{
		q = PROMISINGLIST;
		while( q->link != NULL && q->link->bound >= v.bound )
		q=q->link;
		tmp->link = q->link;
		q->link = tmp;
	}
}

void
renewPromising()
{
  Vertex *ptr = PROMISINGLIST ;
  PROMISINGLIST = NULL;
  Vertex pop ;
  Vertex *tmp ;

  while (ptr != NULL)
  {
    tmp = ptr ;
    pop = *tmp ;
    ptr = ptr->link ;
    free(tmp) ;
    if( isPromising(pop) )
    {
      insertPromising(pop);
    }
  }
}

void display()
{
  Vertex *ptr;
	ptr = PROMISINGLIST;
	if(PROMISINGLIST == NULL)
		printf("Queue is empty\n");
	else
	{
		printf("Queue is :\n");
		printf("Priority       Item\n");
		while(ptr != NULL)
		{
			printf("%5d        %5d\n",ptr->bound,ptr->benefit);
			ptr = ptr->link;
		}
	}
}

void
newChild(Vertex V)
{
  Vertex parent = V ;
  Vertex right ;
  Vertex left ;

  //choose I[item]
  if(parent.item < NUMOFITEMS) {
    right.item = parent.item + 1 ;
  }

  right.benefit = parent.benefit + BBITEMLIST[right.item].b ;
  right.weight = parent.weight + BBITEMLIST[right.item].w ;
  right.bound = right.benefit + findBound( &BBITEMLIST[right.item+1], NUMOFITEMS - right.item - 1 , TOTALWEIGHT - right.weight ) ;
  right.link = NULL;
  if (isPromising(right) )
  {
    insertPromising( right ) ;
    if (right.benefit > MAXBENEFIT)
      MAXBENEFIT = right.benefit ;
  }

  //don't choose I[item]
  if(parent.item < NUMOFITEMS) {
    left.item = parent.item + 1 ;
  }
  left.benefit = parent.benefit ;
  left.weight = parent.weight ;
  left.bound = left.benefit + findBound( &BBITEMLIST[left.item+1], NUMOFITEMS - left.item - 1, TOTALWEIGHT - left.weight ) ;
  left.link = NULL;
  if (isPromising(left)) {
    insertPromising( left ) ;
  }
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

void printVertex(Vertex v) {
  printf("itme = %d ", v.item);
  printf("benefit = %d ", v.benefit);
  printf("MAXbenefit = %d ", MAXBENEFIT);
  printf("weight = %d ", v.weight);
  printf("bound = %d ", v.bound);
  if(v.link == NULL) {
    printf("NULL");
  }
  printf("\n") ;

}

int
BranchBound()
{
  MAXBENEFIT = 0;
  BBITEMLIST = (Item *)malloc( sizeof(Item)*NUMOFITEMS ) ;
  Copy(ITEMLIST, BBITEMLIST, NUMOFITEMS) ;
  Sort(BBITEMLIST, 0, NUMOFITEMS-1) ;

  Vertex root ;
  root.item = -1 ;
  root.benefit = 0 ;
  root.weight = 0 ;
  root.bound = findBound( BBITEMLIST, NUMOFITEMS, TOTALWEIGHT ) ;
  root.link = NULL;

  insertPromising(root) ;

  for(int i=0; i<2000; i++) {
    Vertex copy = *PROMISINGLIST ;
    newChild(popPromising()) ;
    if(PROMISINGLIST == NULL) {
      break ;
    }
    renewPromising();
    //printVertex(*PROMISINGLIST);
    // if(end == 1) {
    //   printf("end!!") ;
    //   break;
    // }
  }
  int result = popPromising().benefit ;

  free(BBITEMLIST);
  return result ;
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

// int
// main()
// {
//   ITEMLIST = (Item *)malloc( sizeof(Item)*4 );
//   TOTALWEIGHT = 16;
//   NUMOFITEMS = 4;
//
//   ITEMLIST[0].b = 40;
//   ITEMLIST[0].w = 2;
//   ITEMLIST[0].bw = ITEMLIST[0].b / ITEMLIST[0].w ;
//
//   ITEMLIST[1].b = 30;
//   ITEMLIST[1].w = 5;
//   ITEMLIST[1].bw = ITEMLIST[1].b / ITEMLIST[1].w ;
//
//   ITEMLIST[2].b = 50;
//   ITEMLIST[2].w = 10;
//   ITEMLIST[2].bw = ITEMLIST[2].b / ITEMLIST[2].w ;
//
//   ITEMLIST[3].b = 10;
//   ITEMLIST[3].w = 5;
//   ITEMLIST[3].bw = ITEMLIST[3].b / ITEMLIST[3].w ;
//
//   printf("greedy  %d = %d\n",4, BranchBound() ) ;
//
// }
//
int
Random( int min, int max )
{
  return ( rand() % (max - min +1) ) + min ;
}

// int
// main()
// {
//   srand( time(NULL) ) ;
//   int min_b = 1 ;
//   int max_v = 300 ;
//   int min_w = 1 ;
//   int max_w = 100 ;
//   int num_items[9] = { 10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000 } ;
//
//   for ( int i=0; i<9; i++ )
//   {
//     ITEMLIST = (Item *)malloc(num_items[i]*sizeof(Item)) ;
//     for ( int j=0; j<num_items[i]; j++ )
//     {
//       ITEMLIST[j].b = Random(min_b, max_v) ;
//       ITEMLIST[j].w = Random(min_w, max_w) ;
//       ITEMLIST[j].bw = (float)ITEMLIST[j].b / (float)ITEMLIST[j].w ;
//     }
//     NUMOFITEMS = num_items[i] ;
//     TOTALWEIGHT = num_items[i]*40 ;
//
//     printf("BranchBound  %d = %d\n",num_items[i],BranchBound() ) ;
//     printf("dp           %d = %d\n",num_items[i],DynamicProgramming() ) ;
//     free(ITEMLIST) ;
//   }
//   return 0;
// }

int
main()
{
  srand( time(NULL) ) ;
  int min_b = 1 ;
  int max_v = 300 ;
  int min_w = 1 ;
  int max_w = 100 ;
  //int num_items[9] = { 10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000 } ;
  int num_item = 10;

  ITEMLIST = (Item *)malloc(num_item*sizeof(Item)) ;
  for ( int j=0; j<num_item; j++ )
  {
    ITEMLIST[j].b = Random(min_b, max_v) ;
    ITEMLIST[j].w = Random(min_w, max_w) ;
    ITEMLIST[j].bw = (float)ITEMLIST[j].b / (float)ITEMLIST[j].w ;
  }
  NUMOFITEMS = num_item ;
  TOTALWEIGHT = num_item*40 ;

  printf("BranchBound  %d = %d\n",num_item,BranchBound() ) ;
  printf("dp           %d = %d\n",num_item,DynamicProgramming() ) ;
  free(ITEMLIST) ;
  return 0;
}
