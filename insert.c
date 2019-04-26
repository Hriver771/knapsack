typedef struct _Vertex
{
  int item ;
  int benefit ;
  int weight ;
  int bound ;
  struct _Vertex *link ;
}Vertex ;

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
    int keepgoing = TRUE ;

    if( PROMISINGLIST == NULL || v.bound > PROMISINGLIST->bound )
    {
        tmp->link = PROMISINGLIST;
        PROMISINGLIST = tmp;
    }
    else
    {
        q = PROMISINGLIST;
        while( q->link != NULL && q->link->bound > v.bound ) {
            q=q->link;
        }
        while( q->link != NULL && q->link->bound >= v.bound && q->link->benefit > v.benefit ) {
            q=q->link;
        }
        tmp->link = q->link;
        q->link = tmp;
    }
    printf("insert : (%d)\n", MAXBENEFIT) ;
    display() ;
}

int main() {

  insertPromising()
}
