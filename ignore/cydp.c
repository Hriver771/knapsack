#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int dynamic_programming(int n);

int main()
{

   srand(time(NULL));

   int items[] = { 10, 100, 500, 1000, 3000, 5000, 7000, 9000, 10000 };

   for (int i = 0; i < 9; i++) {

   printf("%d = %d \n", items[i], dynamic_programming(items[i]));

   }

   return 0;

}

int dynamic_programming(int n) {
   int max_weight = (n*40)+1;
   int *benefit;
   int *weight;
   int max_benefit;
   int **B;

   B = (int**)malloc(sizeof(int*) * n);
   for (int i = 0; i < n; i++) {
      B[i] = (int*)malloc(sizeof(int) * max_weight);
   }


   benefit = (int*)malloc(sizeof(int)*(n-1));
   weight = (int*)malloc(sizeof(int)*(n-1));

   for (int i = 0; i < n-1; i++) {
      benefit[i] = (rand() % 300) + 1;
      weight[i] = (rand() % 100) + 1;
   }


   for (int w = 0; w < max_weight; w++) { // MAX_WEIGHT
      B[0][w] = 0;
   }
   for (int i = 1; i < n; i++) { // NUMBER OF ELEMENTS
      B[i][0] = 0;

      for (int w = 1; w < max_weight; w++) {  // MAX_WEIGHT

         if (weight[i - 1] <= w) {
            if (benefit[i - 1] + B[i - 1][w-(weight[i-1])] > B[i-1][w]) {
               B[i][w] = benefit[i - 1] + B[i - 1][w-(weight[i-1])];
            }
            else {
               B[i][w] = B[i - 1][w];
            }
         }
         else {
            B[i][w] = B[i-1][w];
         }
      }

   }

   max_benefit = B[n-1][max_weight-1];
   printf("%d \n", max_benefit);

   for (int i = 0; i < n; i++) {
      free(B[i]);
   }
   free(B);
   free(benefit);
   free(weight);

   return max_benefit;

}g
