#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;


void bubblesort(int* arr,int size)
{

for(int i = 0; i<size; i++) 
{
   for(int j = i+1; j<size; j++)
   {
      if(arr[j] < arr[i]) 
      {
         int temp = arr[i];
         arr[i] = arr[j];
         arr[j] = temp;
      }
   }



}

}
int main()
{
int size = 10;
int *arr = new int[size];
srand(time(0));

for(int i=0;i<size;i++)
{

arr[i] = rand()%50;
}

for(int i=0;i<size;i++)
{

printf("%d ",arr[i]);
}
printf("\n");

bubblesort(arr,size);

for(int i=0;i<size;i++)
{

printf("%d ",arr[i]);
}
printf("\n");


delete arr;

}
