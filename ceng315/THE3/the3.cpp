#include "the3.h"
using namespace std;

#define start(x) (x)[0]
#define end(x) (x)[1]
int recursive_sln(int n, char**& arr, int*& len, int &number_of_calls) { //direct recursive
    number_of_calls+=1;
    int maxLen =  0;
    if(number_of_calls == 1 ){
    
    
        
        for (int i = n-1; i > -1;i--){
           if( (start(arr[n]) =='I' && end(arr[i]) =='O')|| (start(arr[n])=='O' && end(arr[i])=='I') || (start(arr[n])=='S' && end(arr[i])=='S')){
               
            maxLen =  (recursive_sln(i,arr,len,number_of_calls)+len[n]);
            break;
            }
        }
        for(int i = 0; i<n ; i++){
            maxLen = max(maxLen,recursive_sln(i,arr,len,number_of_calls));
        }
    }
    else{

        maxLen = len[n];
        int index = 0;
        int jaguar = 0;
        for(int i =  n-1;i>-1;i--){
            if(  (start(arr[n]) =='I' && end(arr[i]) =='O')|| (start(arr[n])=='O' && end(arr[i])=='I') || (start(arr[n])=='S' && end(arr[i])=='S') ){
                
                index = recursive_sln(i,arr,len,number_of_calls)+len[n];
                break;
           }
            
        }
        for(int j = n-1;j>-1;j--){
            if(end(arr[n]) == end(arr[j])){
                jaguar = recursive_sln(j,arr,len,number_of_calls);
                
                break;
            }
        }
    maxLen = max(index,maxLen);
    maxLen = max(jaguar,maxLen);
        
        
    }
    
    return maxLen;
    
    
    

}
int memoization_sln(int i, char **&arr, int *&len, int **&mem)
{ // memoization
    int calls = 0;
    if (i == 0)
    {
        mem[0][0] = 0;
        mem[0][1] = 0;
        mem[0][2] = 0;

        if (arr[0][1] == 'I')
        {
            mem[0][0] = len[i];
        }
        if (arr[0][1] == 'O')
        {
            mem[0][1] = len[i];
        }
        if (arr[0][1] == 'S')
        {
            mem[0][2] = len[i];
        }
        return len[i];
    }
    else
    {
        calls += memoization_sln(i - 1, arr, len, mem);
        bool flag = true;
        for (int j = i - 1; j >= 0; j--)
        {
            if (arr[i][0] == 'I' && arr[j][1] == 'O')
            {
                if (arr[i][1] == 'I')
                {
                    mem[i][0] = max(mem[i - 1][0], mem[j][1] + len[i]);
                    mem[i][1] = mem[i - 1][1];
                    mem[i][2] = mem[i - 1][2];
                }
                if (arr[i][1] == 'O')
                {
                    mem[i][0] = mem[i - 1][0];
                    mem[i][1] = max(mem[i - 1][1], mem[j][1] + len[i]);
                    mem[i][2] = mem[i - 1][2];
                }
                if (arr[i][1] == 'S')
                {
                    mem[i][0] = mem[i - 1][0];
                    mem[i][1] = mem[i - 1][1];
                    mem[i][2] = max(mem[i - 1][2], mem[j][1] + len[i]);
                }
                flag = false;
                break;
            }
            if (arr[i][0] == 'O' && arr[j][1] == 'I')
            {
                if (arr[i][1] == 'I')
                {
                    mem[i][0] = max(mem[i - 1][0], mem[j][0] + len[i]);
                    mem[i][1] = mem[i - 1][1];
                    mem[i][2] = mem[i - 1][2];
                }
                if (arr[i][1] == 'O')
                {
                    mem[i][0] = mem[i - 1][0];
                    mem[i][1] = max(mem[i - 1][1], mem[j][0] + len[i]);
                    mem[i][2] = mem[i - 1][2];
                }
                if (arr[i][1] == 'S')
                {
                    mem[i][0] = mem[i - 1][0];
                    mem[i][1] = mem[i - 1][1];
                    mem[i][2] = max(mem[i - 1][2], mem[j][0] + len[i]);
                }
                flag = false;
                break;
            }
            if (arr[i][0] == 'S' && arr[j][1] == 'S')
            {
                if (arr[i][1] == 'I')
                {
                    mem[i][0] = max(mem[i - 1][0], mem[j][2] + len[i]);
                    mem[i][1] = mem[i - 1][1];
                    mem[i][2] = mem[i - 1][2];
                }
                if (arr[i][1] == 'O')
                {
                    mem[i][0] = mem[i - 1][0];
                    mem[i][1] = max(mem[i - 1][1], mem[j][2] + len[i]);
                    mem[i][2] = mem[i - 1][2];
                }
                if (arr[i][1] == 'S')
                {
                    mem[i][0] = mem[i - 1][0];
                    mem[i][1] = mem[i - 1][1];
                    mem[i][2] = max(mem[i - 1][2], mem[j][2] + len[i]);
                }
                flag = false;
                break;
            }
        }
        if (flag)
        {
            if (arr[i][1] == 'I')
            {
                mem[i][0] = max(mem[i - 1][0], len[i]);
                mem[i][1] = mem[i - 1][1];
                mem[i][2] = mem[i - 1][2];
            }
            if (arr[i][1] == 'O')
            {
                mem[i][0] = mem[i - 1][0];
                mem[i][1] = max(mem[i - 1][1], len[i]);
                mem[i][2] = mem[i - 1][2];
            }
            if (arr[i][1] == 'S')
            {
                mem[i][0] = mem[i - 1][0];
                mem[i][1] = mem[i - 1][1];
                mem[i][2] = max(mem[i - 1][2], len[i]);
            }
        }
        calls = max(calls,mem[i][0]);
         calls = max(calls,mem[i][2]);
          calls = max(calls,mem[i][1]);
    }

    return calls; // . YOU SHOULD CHANGE THIS!
}


int dp_sln(int size, char **&arr, int *&len, int **&mem)
{ // dynamic programming

    for (int i = 0; i < size; i++)
    {
        if (i == 0)
        {
            if (arr[0][1] == 'I')
            {
                mem[0][0] = len[0];
                mem[0][1] = 0;
                mem[0][2] = 0;
            }
            if (arr[0][1] == 'O')
            {
                mem[0][0] = 0;
                mem[0][1] = len[0];
                mem[0][2] = 0;
            }
            if (arr[0][1] == 'S')
            {
                mem[0][0] = 0;
                mem[0][1] = 0;
                mem[0][2] = len[0];
            }
        }
        else
        {
            bool flag = true;
            for (int j = i - 1; j >= 0; j--)
            {
                if (arr[i][0] == 'I' && arr[j][1] == 'O')
                {
                    if (arr[i][1] == 'I')
                    {
                        mem[i][0] = max(mem[i - 1][0], mem[j][1] + len[i]);
                        mem[i][1] = mem[i - 1][1];
                        mem[i][2] = mem[i - 1][2];
                    }
                    if (arr[i][1] == 'O')
                    {
                        mem[i][0] = mem[i - 1][0];
                        mem[i][1] = max(mem[i - 1][1], mem[j][1] + len[i]);
                        mem[i][2] = mem[i - 1][2];
                    }
                    if (arr[i][1] == 'S')
                    {
                        mem[i][0] = mem[i - 1][0];
                        mem[i][1] = mem[i - 1][1];
                        mem[i][2] = max(mem[i - 1][2], mem[j][1] + len[i]);
                    }
                    flag = false;
                    break;
                }
                if (arr[i][0] == 'O' && arr[j][1] == 'I')
                {
                    if (arr[i][1] == 'I')
                    {
                        mem[i][0] = max(mem[i - 1][0], mem[j][0] + len[i]);
                        mem[i][1] = mem[i - 1][1];
                        mem[i][2] = mem[i - 1][2];
                    }
                    if (arr[i][1] == 'O')
                    {
                        mem[i][0] = mem[i - 1][0];
                        mem[i][1] = max(mem[i - 1][1], mem[j][0] + len[i]);
                        mem[i][2] = mem[i - 1][2];
                    }
                    if (arr[i][1] == 'S')
                    {
                        mem[i][0] = mem[i - 1][0];
                        mem[i][1] = mem[i - 1][1];
                        mem[i][2] = max(mem[i - 1][2], mem[j][0] + len[i]);
                    }
                    flag = false;
                    break;
                }
                if (arr[i][0] == 'S' && arr[j][1] == 'S')
                {
                    if (arr[i][1] == 'I')
                    {
                        mem[i][0] = max(mem[i - 1][0], mem[j][2] + len[i]);
                        mem[i][1] = mem[i - 1][1];
                        mem[i][2] = mem[i - 1][2];
                    }
                    if (arr[i][1] == 'O')
                    {
                        mem[i][0] = mem[i - 1][0];
                        mem[i][1] = max(mem[i - 1][1], mem[j][2] + len[i]);
                        mem[i][2] = mem[i - 1][2];
                    }
                    if (arr[i][1] == 'S')
                    {
                        mem[i][0] = mem[i - 1][0];
                        mem[i][1] = mem[i - 1][1];
                        mem[i][2] = max(mem[i - 1][2], mem[j][2] + len[i]);
                    }
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                if (arr[i][1] == 'I')
                {
                    mem[i][0] = max(mem[i - 1][0], len[i]);
                    mem[i][1] = mem[i - 1][1];
                    mem[i][2] = mem[i - 1][2];
                }
                if (arr[i][1] == 'O')
                {
                    mem[i][0] = mem[i - 1][0];
                    mem[i][1] = max(mem[i - 1][1], len[i]);
                    mem[i][2] = mem[i - 1][2];
                }
                if (arr[i][1] == 'S')
                {
                    mem[i][0] = mem[i - 1][0];
                    mem[i][1] = mem[i - 1][1];
                    mem[i][2] = max(mem[i - 1][2], len[i]);
                }
            }
        }
    }

    int maxVal = 0;
    maxVal = max(mem[size - 1][0], mem[size - 1][1]);
    maxVal = max(maxVal, mem[size - 1][2]);

    return maxVal; 
    //this is a dummy return value. YOU SHOULD CHANGE THIS!
}