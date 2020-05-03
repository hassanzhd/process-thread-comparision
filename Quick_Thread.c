#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>   
#include <pthread.h> 
  
 
void swap(int *x,int *y)
{
 int temp=*x;
 *x=*y;
 *y=temp;
}
int partition(int A[],int l,int h)
{
 int pivot=A[l];
 int i=l,j=h;

 do
 {
 do{i++;}while(A[i]<=pivot);
 do{j--;}while(A[j]>pivot);

 if(i<j)swap(&A[i],&A[j]);
 }while(i<j);

 swap(&A[l],&A[j]);
 return j;
}
void *myThreadFun(int A[],int l,int h) 
{ 
    //sleep(1); 
    //printf("Thread executed \n"); 
    //return NULL; 
int j;

 if(l<h)
 {
 j=partition(A,l,h);
 myThreadFun(A,l,j);
 myThreadFun(A,j+1,h);
 }
} 
   
int main() 
{ 
printf("__________________________THREADED QUICKSORT____________________________\n\n\n");
int A[]={11,13,7,12,16,9,24,5,10,3},n=10,i;
    pthread_t thread_id; 
    printf("Before Thread:\n"); 
    for( i=0;i<10;i++){
    	printf("%d\n",A[i]);
	}
	 
    pthread_create(&thread_id, NULL, myThreadFun(A,0,n),NULL); 
    printf("\nAfter Thread\n");
    for(i=0;i<n;i++){
    printf("%d ",A[i]);	
	}
    //pthread_join(thread_id, NULL); 
    
    
    exit(0); 
}
