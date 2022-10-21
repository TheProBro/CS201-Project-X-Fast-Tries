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
node* succesor(int k){
    int l{}, h=W+1, mid{}, prefix{};
    node* temp{};
    while(h-1>l){
        mid=(l+h)/2;
        prefix=k>>(W-mid);
        if(XFast[mid].find(prefix)!=XFast[mid].end()){
            l=mid;
            temp=XFast[mid][prefix];
        }
        else{
            h=mid;
        }
    }
    if(temp==nullptr || temp->level==0) return nullptr;
    if(temp->level==W) return temp;
    int shift=W-temp->level-1;
    int bigPrefix=k>>shift;
    if(bigPrefix%2) temp=temp->right;
    else temp=temp->left;
    if(temp->key<k) return temp->right;
    return temp;
}
node* predecessor(int k){
    int l{}, h=W+1, mid{}, prefix{};
    node* temp{};
    while(h-1>l){
        mid=(l+h)/2;
        prefix=k>>(W-mid);
        if(XFast[mid].find(prefix)!=XFast[mid].end()){
            l=mid;
            temp=XFast[mid][prefix];
        }
        else{
            h=mid;
        }
    }
    if(temp==nullptr || temp->level==0) return nullptr;
    if(temp->level==W) return temp;
    int shift=W-temp->level-1;
    int bigPrefix=k>>shift;
    if(bigPrefix%2) temp=temp->right;
    else temp=temp->left;
    if(temp->key>k) return temp->left;
    return temp;
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