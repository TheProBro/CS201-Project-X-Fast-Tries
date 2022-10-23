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
    if(bigPrefix&1) temp=temp->right;
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
    if(bigPrefix& 1) temp=temp->right;
    else temp=temp->left;
    if(temp->key>k) return temp->left;
    return temp;
}
node *getLeftmostLeaf(node *p){
    node* parent=p;
    while(parent->level != W){
        if(parent->left != nullptr)
            parent = parent->left;
        else
            parent = parent->right;
    }
    return parent;
}
node *getRightmostLeaf(node *p){
    node *parent=p;
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
            inter->key=prefix;
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
        if(XFast[tier][prefix]->left==nullptr or XFast[tier][prefix]->left->level-1!=XFast[tier][prefix]->level)
            XFast[tier][prefix]->left=getLeftmostLeaf(XFast[tier][prefix]->right);
        else if(XFast[tier][prefix]->right == nullptr or XFast[tier][prefix]->right->level-1!=XFast[tier][prefix]->level)
            XFast[tier][prefix]->right = getRightmostLeaf(XFast[tier][prefix]->left);
        --tier;
    }
    if(XFast[0][0]->left == nullptr or XFast[0][0]->left->level-1!=0){
        XFast[0][0]->left = getLeftmostLeaf(XFast[0][0]->right);
    }
    if(XFast[0][0]->right == nullptr or XFast[0][0]->right->level-1!=0){
        XFast[0][0]->right = getRightmostLeaf(XFast[0][0]->left);
    }
}
// void del(int k){
//     if(XFast[W].find(k)==XFast[W].end()){
//         cout<<"Element to be deleted is not present\n";
//         return;   
//     }
//     node* temp=XFast[W][k];
//     node *pre=temp->left;
//     node *suc=temp->right;
//     if(suc!=nullptr){
//         if(pre!=nullptr){
//             pre->right=suc;
//             suc->left=pre;
//         }
//         else pre->right=nullptr;
//     }
//     else{
//         if(pre!=nullptr) pre->right=nullptr;
//     }
//     int tier{W-1}, prefix{};
//     bool DeleteNext=0, DeleteNow=1;
//     while(tier>=0){
//         if(DeleteNext){
//             DeleteNow=1;
//             DeleteNext=0;
//         }
//         node* t=XFast[tier][k>>1];
//         if(k & 1){
//             if(t->left->key==k)
//                 DeleteNext=1;
//         }
//         else{
//             if(t->right->key==k)
//         }
//         if(DeleteNow){
//             free(XFast[tier+1][k]);
//         }
//         tier--;
//         k>>=1;
//     }
// }
int main(){
    U=31;
    cout<<"Enter max number of bits:";
    int n;cin>>n;
    U=pow(2,n);
    W=bitsCounter(U);
    // cout<<W<<'\n';
    XFast=vector<unordered_map<int, node*>>(W+1);
    XFast[0][0]=new node();
    XFast[0][0]->level=0;
    insert(2);
    //cout<<XFast[0][0]->right->key<<'\n';
    // insert(3);
    // cout<<XFast[0][0]->right->key<<'\n';
    // insert(12);
    // cout<<XFast[1][0]->right->key<<'\n';
    while(1){
        char c;cout<<"Enter I to insert"<<"\n"<<"Enter P to get predecessor"<<"\n"<<"Enter S to get successor"<<"\n";
        cout<<"Enter your coice:";cin>>c;
        switch(c){
            case 'I':{
                int n;cout<<"Enter number to be inserted:";
                cin>>n;
                insert(n);
                break;
            }
            case 'P':{
                int k;cout<<"Enter key whose predecessor you want:";cin>>k;
                cout<<"Predecessor of"<<k<<"is"<<predecessor(k)->key<<"\n";
                break;
            }
            case 'S':{
                int k;cout<<"Enter key whose succcessor you want:";cin>>k;
                cout<<"Successor of"<<k<<"is"<<succesor(k)->key<<"\n";
                break;
            }
            case 'Q':{
                return 0;
            }
            default:
               cout<<"Enter valid character"<<"\n";
        }
    }
    return 0;
}
