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

  while (ptr != NULL)
  {
    pop = *ptr ;
    ptr = ptr->link ;
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
  right.item = parent.item + 1 ;
  right.benefit = parent.benefit + BBITEMLIST[right.item].b ;
  if (MAXBENEFIT == 0)
    MAXBENEFIT = right.benefit ;
  right.weight = parent.weight + BBITEMLIST[right.item].w ;
  right.bound = right.benefit + findBound( &BBITEMLIST[right.item+1], NUMOFITEMS - right.item - 1 , TOTALWEIGHT - right.weight ) ;
  right.link = NULL;
  if (isPromising(right))
  {
    insertPromising( right ) ;
    if (right.benefit > MAXBENEFIT)
      MAXBENEFIT = right.benefit ;
  }

  //don't choose I[item]
  left.item = parent.item + 1 ;
  left.benefit = parent.benefit ;
  left.weight = parent.weight ;
  left.bound = left.benefit + findBound( &BBITEMLIST[left.item+1], NUMOFITEMS - left.item - 1, TOTALWEIGHT - left.weight ) ;
  left.link = NULL;
  if (isPromising(left)) {
    insertPromising( left ) ;
  }

  if( !isPromising(right) && !isPromising(left) ) {
    insertPromising(parent);
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

void
BranchBound()
{
  BBITEMLIST = (Item *)malloc( sizeof(Item)*NUMOFITEMS ) ;
  Copy(ITEMLIST, BBITEMLIST, NUMOFITEMS) ;
  Sort(BBITEMLIST, 0, NUMOFITEMS-1) ;

  Vertex root ;
  root.item = -1 ;
  root.benefit = 0 ;
  root.weight = 0 ;
  root.bound = findBound( BBITEMLIST, NUMOFITEMS, TOTALWEIGHT ) ;

  insertPromising(root) ;

  newChild(popPromising()) ;
  // newChild(root, sortedI, N, W) ;
  // newChild(promising->link->data, sortedI, N, W) ;
  // newChild(promising->link->link->data, sortedI, N, W) ;
  printf("maxBene = %d\n", MAXBENEFIT) ;
  display();
  printf("displayOK\n") ;
  newChild( popPromising() ) ;
  renewPromising();
  printf("maxBene = %d\n", MAXBENEFIT) ;
  display();
  newChild( popPromising() ) ;
  renewPromising();
  printf("maxBene = %d\n", MAXBENEFIT) ;
  display();
  newChild( popPromising() ) ;
  renewPromising();
  printf("maxBene = %d\n", MAXBENEFIT) ;
  display();
  newChild( popPromising() ) ;
  renewPromising();
  printf("maxBene = %d\n", MAXBENEFIT) ;
  display();
  newChild( popPromising() ) ;
  renewPromising();
  printf("maxBene = %d\n", MAXBENEFIT) ;
  display();



  // printf("root: benefit = %d, weight = %d, bound = %d\n", root->benefit, root->weight, root->bound) ;
  // printf("Rchild: parentI = %d, benefit = %d, weight = %d, bound = %d\n", root->right->parent->item, root->right->benefit, root->right->weight, root->right->bound);
  // printf("Lchild: parentI = %d, benefit = %d, weight = %d, bound = %d\n", root->left->parent->item, root->left->benefit, root->left->weight, root->left->bound);

}

int
main()
{
  ITEMLIST = (Item *)malloc( sizeof(Item)*4 );
  TOTALWEIGHT = 16;
  NUMOFITEMS = 4;

  ITEMLIST[0].b = 40;
  ITEMLIST[0].w = 2;
  ITEMLIST[0].bw = ITEMLIST[0].b / ITEMLIST[0].w ;

  ITEMLIST[1].b = 30;
  ITEMLIST[1].w = 5;
  ITEMLIST[1].bw = ITEMLIST[1].b / ITEMLIST[1].w ;

  ITEMLIST[2].b = 50;
  ITEMLIST[2].w = 10;
  ITEMLIST[2].bw = ITEMLIST[2].b / ITEMLIST[2].w ;

  ITEMLIST[3].b = 10;
  ITEMLIST[3].w = 5;
  ITEMLIST[3].bw = ITEMLIST[3].b / ITEMLIST[3].w ;

  BranchBound() ;

  //printf("%d", BranchBound(sample, 4, 16)) ;
}
