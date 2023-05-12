#include "the4.h"

#define memRow  ncol1+1
#define memColumn  ncol2+1
#define MAX_INT 2147483647
using namespace std;

int dp_sln(char **&arr1, char **&arr2, int nrow, int ncol1, int ncol2, int **&mem)
{ // dynamic programming
    mem[0][0] = 0;
    for (int i = 0; i < ncol2; i++)
    {
        int insertion = 0;
        for (int j = 0; j < nrow; j++)
        {
            if (arr2[j][i] != '-')
                insertion++;
        }
        mem[0][i + 1] = insertion + mem[0][i];
    }
    for (int i = 0; i < ncol1; i++)
    {
        int deletion = 0;
        for (int j = 0; j < nrow; j++)
        {
            if (arr1[j][i] != '-')
                deletion++;
        }
        mem[i + 1][0] = deletion + mem[i][0];
    }

    for (int i = 1; i < memRow; i++)
    {
        for (int j = 1; j < memColumn; j++)
        {
            bool isNotSimilar = false;
            int alphabet[27]={0};
            int reorderCount = MAX_INT;
            int replacementCount = MAX_INT;
            int count = 0;
            for (int k = 0; k < nrow; k++)
            {
                if (arr1[k][i - 1] != '-')
                    alphabet[int(arr1[k][i - 1]) - 65]++;
                else
                {
                    alphabet[26]++;
                }
                if (arr2[k][j - 1] != '-')
                    alphabet[int(arr2[k][j - 1]) - 65]--;
                else
                {
                    alphabet[26]--;
                }
            }
            for (int z = 0; z < 27; z++)
            {
                
                if (alphabet[z] != 0)
                {
                    isNotSimilar = true;
                    break;
                }
            }
           
            if (!isNotSimilar)
            {
                reorderCount = 0;
                for (int ii = 0; ii < nrow; ii++)
                {   
                     
                    if (arr1[ii][i - 1] != arr2[ii][j - 1])
                        reorderCount++;
                        
                }
            }
            else
            {
                
                replacementCount = 0;
                for (int replace = 0; replace < nrow; replace++)
                {
                    if (arr1[replace][i - 1] != arr2[replace][j - 1])
                    {
                        if (arr1[replace][i - 1] == '-' || arr2[replace][j - 1] == '-')
                            replacementCount += 2;
                        else
                            replacementCount++;
                    }
                }
            }
                count = min(reorderCount, replacementCount);
            
              
            int minimum1 = mem[i][j - 1] + (mem[0][j] - mem[0][j - 1]);
            int minimum2 = mem[i - 1][j - 1] + count;
            int minimum3 = mem[i - 1][j] + (mem[i][0] - mem[i - 1][0]);

            minimum1 = min(minimum1, minimum2);
            mem[i][j] = min(minimum1, minimum3);
        }
    }

    return mem[ncol1][ncol2];
}

