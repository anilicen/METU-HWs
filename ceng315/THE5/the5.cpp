#include "the5.h"

using namespace std;

/* 
    in the5.h "struct Room" is defined as below:
    
    struct Room {
        int id;
        char content;
        vector<Room*> neighbors;
    };

*/
bool contains(vector<int> vec,int x){
    bool cont = false;

    for(int i = 0;i<vec.size();i++){
        if(vec[i]==x)return true;
    }
    return cont;

}

vector<int> maze_trace(vector<Room*> maze) { 

    vector<int> visited;
    stack<int> path;
    stack<int> nbrs; //maybe same with vector??
    bool isNotDone = true;
    int i = 0;
    while(1){
        int mazeNumber = maze[i]->id;
        cout<<mazeNumber<<endl;
        visited.push_back(mazeNumber);

        if(maze[i]->content=='*'){
            break;
        }
        if(path.empty() || path.top()!=mazeNumber)
        path.push(mazeNumber);
        
        int nbrsCount = 0;
        int nbrsSize = maze[i]->neighbors.size();
        for(int j=nbrsSize-1;j>=0;j--){
            int nbrsId = maze[i]->neighbors[j]->id;

            if(contains(visited,nbrsId)){
                continue;
            }
            else{
                nbrs.push(nbrsId);
                nbrsCount++;
            }
        }
        
        if(nbrsCount==0){
            path.pop();
            i=path.top();
        }
        else{
        i=nbrs.top();
        nbrs.pop();
        }


    }

    while(!path.empty()){
        int val = path.top();
        visited.push_back(val);
        path.pop();
    }
    

    return visited; // this is a dummy return value. YOU SHOULD CHANGE THIS!
}


//case 1 Expected path : 0, 3, 8, 5, 8, 6, 1, 4, 7, 4, 1, 2, 1, 6, 8, 3, 9, 3, 0

//case 2 Expected path : 0, 3, 8, 14, 10, 13, 7, 12, 11, 12, 7, 13, 10, 14, 8, 3, 0

