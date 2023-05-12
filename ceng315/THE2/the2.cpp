#include "the2.h"
#include <cmath>
// You may write your own helper functions here

int countSort(int* arr, int n, int k,int* parts){
    int numberOfIterations = 0;
    int *b = new int[n];
    int *c = new int[k+1];
    c[k]={0};
    
    for(int j = 0;j<n;j++){
        c[parts[j]]=c[parts[j]]+1;
        numberOfIterations++;
    }

    for(int i = 1;i<=k;i++){
        c[i]=c[i]+c[i-1];
        numberOfIterations++;
    }

    for(int j = n-1;j>=0;j--){
        b[c[parts[j]]-1]=arr[j];
        c[parts[j]] = c[parts[j]]-1;
        numberOfIterations++;
    }
    
    
    

for(int i = 0;i<n;i++){
        arr[i] = b[i];
       
        numberOfIterations++;
    }
    return  numberOfIterations;}


long multiDigitRadixSort(int* arr, bool ascending, int arraySize, int groupSize, int maxDigitLength){
    long numberOfIterations = 0;
    
    int count = maxDigitLength;
    int timesOfCount =  ceil(float(maxDigitLength)/groupSize);
    for(int i =0;i<timesOfCount;i++){
        int k = count>=groupSize ? groupSize : count;
        count-=groupSize;
        int* parts = new int[arraySize];
        for(int j = 0;j<arraySize;j++){

            parts[j] = int(arr[j]/pow(10,(i*groupSize)))%int (pow(10,groupSize));
        }
        

        numberOfIterations+=countSort(arr,arraySize,pow(10,k)-1,parts);
    }
    if(!ascending){int temp[arraySize];
    for(int i = 0; i<arraySize;i++){
        temp[i]=arr[i];
    }
for(int i = 0; i<arraySize;i++){
        arr[i]=temp[arraySize-i-1];
    }    
    }

    return numberOfIterations;
}
