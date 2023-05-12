void insertionSort(int* arr, long &comparison, long & swap, int size) 
{
    int i = 1;
    int x = 0;
    int j = 0;
    while(i < size){
        x = arr[i];
        j = i - 1;
        while(j>=0 && arr[j] > x ){
            //std::cout<<" arr[i] "<<arr[i]<<" arr[j] "<<arr[j]<< std::endl;  
            comparison++;
            arr[j+1] = arr[j];
            j-=1;
            swap++;
        }
            //std::cout<<" arr[i] "<<arr[i]<<" x "<<x<< std::endl;  
        if(j>=0)
        comparison++;
        arr[j+1] = x;
        i++;
        
    }
    //Your Code Here
    
}
void heapify(int* arr, int size, int i,long& comparison, long& swap)
{
    int min = i; 
    int l = 2 * i + 1; 
    int r = 2 * i + 2; 
 
    if (l < size && arr[l] < arr[min]){
        min = l;
        if(  arr[l]!=2147483647)
        comparison++;

    }
    else if(l < size &&  arr[l]!=2147483647)comparison++;
 
    if (r < size && arr[r] <arr[min]){
        min = r;
        if( arr[r]!=2147483647)
        comparison++;

    
    }
    else if(r < size &&  arr[r]!=2147483647)comparison++;

    if (min != i) {
        ////std::cout<<'x'<<std::endl;

        int tmp=arr[i];
        arr[i]=arr[min];
        arr[min]=tmp;
        swap++;
        
        heapify(arr, size, min, comparison,swap);
    } 

}
void buildHeap(int* arr, int size,long& comparison, long& swap)
{
    int startIdx = (size / 2) - 1;

    for (int i = startIdx; i >= 0; i--) {
        heapify(arr, size, i, comparison, swap);
        
    }


}
//You can add your own helper funbuildHeap(heap,K);ctions

int kWayMergeSortWithHeap(int* arr, int K, int size, long& comparison, long& swap){
  
  int number_of_calls = 1;
	if(K>size){
        insertionSort(arr,comparison,swap,size);
    }
    else{
        int sizePart = size/K;
        for(int j = 0 ; j < K;j++)
    {  int** part = new int* [sizePart];

        for(int i = 0 ; i<sizePart;i++){
            part[i] = &arr[i + j*sizePart];
            
        }
        number_of_calls+=kWayMergeSortWithHeap(*part,K,sizePart,comparison,swap);
    }      

      
       int result[size];

       int heap[K];
       int count[K][3];
        int *pointCount[K];
for(int x = 0; x<K;x++){
    count[x][0] = 1;
}

       for(int i = 0 ; i<K;i++){
           heap[i] = arr[i*size/K];
           pointCount[i]=&arr[i*size/K];
           count[i][2] = i;
           }

       int max = 2147483647;
       for(int s = 0;s < size;s++)
       {

      
      if(s==0)
       buildHeap(heap,K,comparison,swap);
       else{
        heapify(heap,K,0,comparison,swap);
       }
        result[s] = heap[0];
        for(int search=0;search<K;search++){
            
            if(*pointCount[search]==heap[0]){
                count[search][0]+=1;
                if(count[search][0] <= size/K){
                    pointCount[search] = &arr[size/K*(count[search][2])+count[search][0]-1];
                }
                else{
                    pointCount[search] = &max;
                }
                heap[0] = *pointCount[search];
                
                break;
            }
        }
        
    }
    for(int xx =0 ; xx<size;xx++){
        arr[xx] = result[xx];
    }
	
    }
    //std::cout<<comparison<<std::endl;
	return number_of_calls;

}