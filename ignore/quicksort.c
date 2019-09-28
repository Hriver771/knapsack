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

void quick_sort(Item[],int,int);
int partition(Item[],int,int);

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

  quick_sort(sample, 0, 3);

  for ( int i=0; i<4; i++ )
  {
    printf("%d %d %f\n",sample[i].b, sample[i].w, sample[i].bw);
  }
}

void
quick_sort(Item a[],int l,int r)
{
	int p;
	if(l<r)
	{
		p=partition(a,l,r);
		quick_sort(a,l,p-1);
		quick_sort(a,p+1,r);
	}
}

int partition(Item a[],int l,int r)
{
	Item v=a[l];
	int i=l;
	int p=r+1;
  Item temp;

	do
	{
		do
			i++;
		while(a[i].bw>v.bw&&i<=r);

		do
			p--;
		while(v.bw>a[p].bw);

		if(i<p)
		{
			temp=a[i];
			a[i]=a[p];
			a[p]=temp;
		}
	}while(i<p);

	a[l]=a[p];
	a[p]=v;

	return(p);
}
