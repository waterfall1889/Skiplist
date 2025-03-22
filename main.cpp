#include <iostream>
#include "skiplist.h"

using namespace std;
int main() {
    Skiplist newList;
    for(int i=0;i<70;++i){
        newList.insert(i);
    }
    int find = 0;
    cout<<"Try find:";
    cin>>find;
    while(find != -1){
        if(newList.search(find)){
            cout<<find<<" exists!"<<endl;
        }
        else{
            cout<<find<<" not found!"<<endl;
        }
        cout<<"Try find:";
        cin>>find;
    }
    return 0;
}
