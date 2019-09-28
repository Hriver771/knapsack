//my program works for every problem
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TRUE 1
#define FALSE 0

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
    int nochild ;
}Vertex ;


Item *ITEMLIST = NULL;
Item *BBITEMLIST = NULL ;
Item *GRDITEMLIST = NULL ;
int NUMOFITEMS = 0 ;
int TOTALWEIGHT = 0 ;
int MAXBENEFIT = 0;
Vertex *PROMISINGLIST = NULL ;
int RESULT = FALSE ;
clock_t start, mid, finish;


int
Random( int min, int max )
{
    return ( rand() % (max - min +1) ) + min ;
}

int
findBound( Item I[], int N, int W )
{
    int sumB = 0 ;
    int sumW = 0 ;

    for( int i=0; sumW<W && i<N; i++ )
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
    if ( v.bound > MAXBENEFIT && v.weight <= TOTALWEIGHT )
    {
        return TRUE ;
    }
    else {
        return FALSE ;
    }
}

Vertex
popPromising()
{
    Vertex *ptr = PROMISINGLIST ;
    Vertex *tmp ;
    Vertex pop ;
    if(PROMISINGLIST->nochild == FALSE)
    {
        tmp = PROMISINGLIST ;
        pop = *tmp ;
        PROMISINGLIST = PROMISINGLIST->link;
        free(tmp) ;
        return pop ;
    }
    else
    {
        while ( ptr->link->nochild == TRUE )
        {
            ptr = ptr->link ;
        }
        tmp = ptr->link ;
        ptr->link = ptr->link->link ;
        pop = *tmp ;
        free(tmp) ;
        return pop ;
    }
    printf( "*****ERROR:popPromising()*****" ) ;
    return pop ;
}

void
insertPromising( Vertex v )
{
    Vertex *tmp ;
    Vertex *q ;

    if( v.benefit > MAXBENEFIT )
    {
        MAXBENEFIT = v.benefit ;
    }

    tmp = ( Vertex * )malloc( sizeof(Vertex) ) ;
    *tmp = v ;

    if( PROMISINGLIST == NULL || v.bound > PROMISINGLIST->bound )
    {
        tmp->link = PROMISINGLIST ;
        PROMISINGLIST = tmp ;
    }
    else
    {
        q = PROMISINGLIST ;
        while( q->link != NULL && q->link->bound >= v.bound )
        {
            q = q->link ;
        }
        tmp->link = q->link ;
        q->link = tmp ;
    }
}

void
renewPromising()
{
    Vertex *ptr = PROMISINGLIST ;
    PROMISINGLIST = NULL ;

    while ( ptr != NULL )
    {
        if( isPromising(*ptr) || ( ptr->benefit == ptr->bound && ptr->bound == MAXBENEFIT ) )
        {
            insertPromising( *ptr ) ;
        }
        ptr = ptr->link ;
    }
    if(PROMISINGLIST == NULL) {
    }
}

void
newChild(Vertex V)
{
    Vertex parent = V ;
    Vertex right ;
    Vertex left ;
    int noChild = TRUE ;

    //outofbound -> no child
    if( parent.item == NUMOFITEMS )
    {
        parent.nochild = TRUE ;
        insertPromising( parent );
        return ;
    }

    //choose I[item]
    right.item = parent.item + 1 ;
    right.benefit = parent.benefit + BBITEMLIST[right.item].b ;
    right.weight = parent.weight + BBITEMLIST[right.item].w ;
    right.bound = right.benefit + findBound( &BBITEMLIST[right.item+1], NUMOFITEMS - right.item - 1 , TOTALWEIGHT - right.weight ) ;
    right.link = NULL;
    right.nochild = FALSE ;

    if ( isPromising( right ) )
    {
        insertPromising( right ) ;
        noChild = FALSE ;
    }

    //don't choose I[item]
    left.item = parent.item + 1 ;
    left.benefit = parent.benefit ;
    left.weight = parent.weight ;
    left.bound = left.benefit + findBound( &BBITEMLIST[left.item+1], NUMOFITEMS - left.item - 1, TOTALWEIGHT - left.weight ) ;
    left.link = NULL;
    left.nochild = FALSE ;

    if ( isPromising( left ) )
    {
        insertPromising( left ) ;
        noChild = FALSE ;
    }

    //couldn't get new child
    if( noChild ) {
        parent.nochild = TRUE ;
        insertPromising( parent );
        return ;
    }
    return ;
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
Copy( Item origin[], Item new[], int N )
{
    for ( int i=0; i<N; i++ )
    {
        new[i].b = origin[i].b;
        new[i].w = origin[i].w;
        new[i].bw = origin[i].bw;
    }
}

void
Sort( Item a[], int first, int last )
{
    int pivot, i, j ;
    if( first < last )
    {
        pivot = first ;
        i = first ;
        j = last ;
        while ( i < j )
        {
            while( a[i].bw >= a[pivot].bw && i < last )
            {
                i++ ;
            }
            while( a[j].bw < a[pivot].bw )
            {
                j-- ;
            }
            if( i < j )
            {
                Swap( &a[i], &a[j] ) ;
            }
        }
        Swap( &a[pivot], &a[j] ) ;
        Sort( a, first, j - 1 ) ;
        Sort( a, j + 1, last ) ;
    }
}

int
canFinish()
{
    Vertex *ptr = PROMISINGLIST ;
    while( ptr != NULL )
    {
        if( ptr->nochild == FALSE )
        {
            return FALSE;
        }
        ptr = ptr->link;
        if( ptr == NULL )
        {
            return TRUE ;
        }
    }
    //not possible-I think
    printf( "******ERROR:canFinish()******" ) ;
    return TRUE ;
}

float
Greedy()
{
  GRDITEMLIST = (Item *)malloc( sizeof(Item)*NUMOFITEMS ) ;
  Copy(ITEMLIST, GRDITEMLIST, NUMOFITEMS) ;
  Sort(GRDITEMLIST, 0, NUMOFITEMS-1) ;
    int timeCheck = 0;

  float sumB = 0 ;
  int sumW = 0 ;

  for( int i=0; sumW<TOTALWEIGHT && i<NUMOFITEMS; i++ )
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
      if ( timeCheck % 1000 == 0 ) {
          mid = clock();
          if ( ( ( (float)mid - (float)start ) / CLOCKS_PER_SEC ) > 900 )
          {
              return 0;
          }
      }
      timeCheck++ ;
  }
  free(GRDITEMLIST) ;
  return sumB ;
}

int
BranchBound()
{
    MAXBENEFIT = 0;
    Vertex pop ;
    Vertex root ;
    int timeCheck = 0 ;

    BBITEMLIST = (Item *)malloc( sizeof(Item)*NUMOFITEMS ) ;
    Copy(ITEMLIST, BBITEMLIST, NUMOFITEMS) ;
    Sort(BBITEMLIST, 0, NUMOFITEMS-1) ;

    root.item = -1 ;
    root.benefit = 0 ;
    root.weight = 0 ;
    root.bound = findBound( BBITEMLIST, NUMOFITEMS, TOTALWEIGHT ) ;
    root.link = NULL ;
    root.nochild = FALSE ;
    insertPromising(root) ;

    while( !canFinish() )
    {
        pop = popPromising() ;
        newChild(pop) ;
        renewPromising() ;
        mid = clock();
        if ( timeCheck % 1000 == 0 ) {
            mid = clock();
            if ( ( ( (float)mid - (float)start ) / CLOCKS_PER_SEC ) > 900 )
            {
                return 0;
            }
        }
        timeCheck++ ;
    }
    free( BBITEMLIST ) ;
    return MAXBENEFIT ;
}

int
DynamicProgramming()
{
    int result ;
    int timeCheck = 0;

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
        if ( timeCheck % 1000 == 0 ) {
            mid = clock();
            if ( ( ( (float)mid - (float)start ) / CLOCKS_PER_SEC ) > 900 )
            {
                return 0;
            }
        }
        timeCheck++ ;
    }
    result = B[NUMOFITEMS][TOTALWEIGHT] ;

    for ( int i=0; i<=NUMOFITEMS; i++ )
    {
        free( B[i] ) ;
    }
    free( B ) ;
    return result ;
    
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

   FILE *fp ;
   fp = fopen( "output.txt", "w" ) ;
     
   fprintf( fp, "------------------------------------------------------------------------------------------------------\n" ) ;
   fprintf( fp, "%-10s%-5s%s\n", "Number of", "|", "           Processing time in miliseconds / Maximum benefit value" ) ;
   fprintf( fp, "           -------------------------------------------------------------------------------------------\n" ) ;
   fprintf( fp, "%-10s%-10s%-16s%-17s%-16s%-16s%s\n","Items","|", "Greedy", "|", "DP", "|", "BB" ) ;
   fprintf( fp, "------------------------------------------------------------------------------------------------------\n" ) ;

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
     TOTALWEIGHT = num_items[i] * 40 ;

     float greedyT, bbT, dpT ;
     float greedyR ;
     int bbR, dpR ;
       
     start = clock() ;
     greedyR = Greedy() ;
     finish = clock() ;
     greedyT = ( (float)finish - (float)start ) / CLOCKS_PER_SEC * 1000 ;


     start = clock() ;
     dpR = DynamicProgramming() ;
     finish = clock() ;
     dpT = ( (float)finish - (float)start ) / CLOCKS_PER_SEC * 1000 ;

     start = clock() ;
     bbR = BranchBound() ;
     finish = clock() ;
     bbT = ( (float)finish - (float)start ) / CLOCKS_PER_SEC * 1000 ;

     fprintf( fp, "%-10d%-5s%.3f%s%-15.3f%-7s%10.3f%s%-15d%-8s%10.3f%s%-20d\n",num_items[i],"|", greedyT,"/", greedyR, "|", dpT, "/", dpR, "|", bbT, "/", bbR ) ;
     fprintf( fp, "------------------------------------------------------------------------------------------------------\n" ) ;
     free( ITEMLIST ) ;
       
   }
   fprintf( fp, "0 means time-out\n" ) ;
   fclose( fp ) ;
   return 0 ;
 }
