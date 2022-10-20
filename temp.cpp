#include <bits/stdc++.h>
using namespace std;

struct node
{
    int key;
    int level;
    node* left, *right;
    node(){
        level=-1;
        key=0;
        left=right=nullptr;
    }
};
int U, W;
vector<unordered_map<int, node*>> XFast;

int bitsCounter(int a){
    int count{};
    while(a!=0){
        count++;
        a>>=1;
    }
    return count;
}
node* find(int k){
    if(XFast[W].find(k)!=XFast[W].end()) 
        return XFast[W][k];
    return nullptr;
}
int main(){
    cin>>U;
    W=bitsCounter(U);
    cout<<W<<'\n';
    XFast=vector<unordered_map<int, node*>>(W+1);
    XFast[0][0]=new node();
    XFast[0][0]->level=0;
    return 0;
}