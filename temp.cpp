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
node *getLeftmostLeaf(node *parent){
    while(parent->level != W){
        if(parent->left != nullptr)
            parent = parent->left;
        else
            parent = parent->right;
    }
    return parent;
}

node * getRightmostLeaf(node *parent){
    while(parent->level != W){
        if(parent->right != nullptr)
            parent = parent->right;
        else
            parent = parent->left;
    }
    return parent;
}
void insert(int value){
    node *temp=new node();
    temp->key=value;
    temp->level=W;

    node *pre=predecessor(value);
    node *suc=succesor(value);
    if(pre!=nullptr){
        if(pre->level!=W){
            cout<<"Wierd level"<<pre->level<<endl;
        }
        temp->right=pre->right;
        pre->right=temp;
        temp->left=pre;
    }
    if(suc!=nullptr){
        if(suc->level!=W){
            cout<<"Wierd level"<<suc->level<<endl;
        }
        temp->left=suc->left;
        suc->left=temp;
        temp->right=suc;
    }
    int tier=1,prefix{};
    while(tier!=W){
        prefix=value>>(W-tier);
        if(XFast[tier].find(prefix)==XFast[tier].end()){
            node *inter=new node();
            inter->level=tier;
            XFast[tier][prefix]=inter;
            if(prefix & 1){
                XFast[tier-1][prefix>>1]->right=inter;
            }
            else
                XFast[tier-1][prefix>>1]->left=inter;    
            }
            ++tier;
        }
    XFast[W][value]=temp;
    if(value & 1){
        XFast[W-1][value>>1]->right=temp;
    }
    else{
        XFast[W-1][value>>1]->left=temp;
    }
    prefix=value;
    tier=W-1;
    while(tier!=0){
        prefix=prefix>>1;
        if(XFast[tier][prefix]->left==nullptr){
            XFast[tier][prefix]->left==getLeftmostLeaf(XFast[0][0]->right);
        }
        else if(XFast[tier][prefix]->right == nullptr)
            XFast[tier][prefix]->right = getRightmostLeaf(XFast[tier][prefix]->left);
        --tier;
    }
    if(XFast[0][0]->left == nullptr){
        XFast[0][0]->left = getLeftmostLeaf(XFast[0][0]->right);
    }
    if(XFast[0][0]->right == nullptr){
        XFast[0][0]->right = getRightmostLeaf(XFast[0][0]->left);
    }
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
