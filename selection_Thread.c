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


void *myThreadFun(int A[],int n) 
{ 
  int i,j,k;

 for(i=0;i<n-1;i++)
 {
 for(j=k=i;j<n;j++)
 {
 if(A[j]<A[k])
 k=j;
 }
 swap(&A[i],&A[k]);
 }
} 
int main(){
  printf("________________________THREADED SELECTIONSORT______________________\n\n\n");
int A[]={11,13,7,12,16,9,24,5,10,3},n=10,i;
    pthread_t thread_id; 
    printf("Before Thread:\n"); 
    for( i=0;i<10;i++){
    	printf("%d\n",A[i]);
	}
	 
    pthread_create(&thread_id, NULL, myThreadFun(A,n),NULL); 
    printf("\nAfter Thread\n");
    for(i=0;i<n;i++){
    printf("%d ",A[i]);	
	}
    //pthread_join(thread_id, NULL); 
    
    
    exit(0); 
}
