#include <bits/stdc++.h>
using namespace std;

struct node {
    int key;
    int level;
    node* left, *right;
    node(){
        level=-1;
        key=0;
        left=right=nullptr;
    }
};
int U, W, choice;
vector<unordered_map<int, node*>> XFast;

int bitsCounter(int a) {
    int count{};
    while(a!=0){
        count++;
        a>>=1;
    }
    return count;
}
node* find(int k) {
    if(XFast[W].find(k)!=XFast[W].end()) 
        return XFast[W][k];
    return nullptr;
}
node* succesor(int k) {
    int l{}, h=W+1, mid{}, prefix{};
    node* temp{};
    bool nonempty=0;
    while(h-1>l){
        mid=(l+h)/2;
        prefix=k>>(W-mid);
        if(XFast[mid].find(prefix)!=XFast[mid].end()){
            l=mid;
            temp=XFast[mid][prefix];
        } else {
            if(!XFast[mid].empty()) nonempty=1;
            h=mid;
        }
    }
    if(temp==nullptr) {
        if(nonempty){
            temp=XFast[0][0]->left->level==W?XFast[0][0]->left:XFast[0][0]->right;
            if(temp->key>k) return temp;
            else return nullptr;
        }
        else return nullptr;
    }
    if(temp->level==W) return temp;
    int shift=W-temp->level-1;
    int bigPrefix=k>>shift;
    if(bigPrefix&1) temp=temp->right;
    else temp=temp->left;
    if(temp->key<k) return temp->right;
    return temp;
}
node* predecessor(int k) {
    int l{}, h=W+1, mid{}, prefix{};
    node* temp{};
    bool nonempty=0;
    while(h-1>l){
        mid=(l+h)/2;
        prefix=k>>(W-mid);
        if(XFast[mid].find(prefix)!=XFast[mid].end()){
            l=mid;
            temp=XFast[mid][prefix];
        } else {
            if(!XFast[mid].empty()) nonempty=1;
            h=mid;
        }
    }
    if(temp==nullptr) {
        if(nonempty){
            temp=XFast[0][0]->left->level==W?XFast[0][0]->left:XFast[0][0]->right;
            if(temp->key<k) return temp;
            else return nullptr;
        }
        else return nullptr;
    }
    if(temp->level==W) return temp;
    int shift=W-temp->level-1;
    int bigPrefix=k>>shift;
    if(bigPrefix& 1) temp=temp->right;
    else temp=temp->left;
    if(temp->key>k) return temp->left;
    return temp;
}
node *getLeftmostLeaf(node *p) {
    node* parent=p;
    while(parent->level != W){
        if(parent->left != nullptr)
            parent = parent->left;
        else
            parent = parent->right;
    }
    return parent;
}
node *getRightmostLeaf(node *p) {
    node *parent=p;
    while(parent->level != W){
        if(parent->right != nullptr)
            parent = parent->right;
        else
            parent = parent->left;
    }
    return parent;
}
void insert(int value) {
    node *temp=new node();
    temp->key=value;
    temp->level=W;

    node *pre=predecessor(value);
    node *suc=succesor(value);
    if(pre!=nullptr) {
        if(pre->level!=W) {
            cout<<"Wierd level"<<pre->level<<endl;
        }
        temp->right=pre->right;
        pre->right=temp;
        temp->left=pre;
    }
    if(suc!=nullptr) {
        if(suc->level!=W) {
            cout<<"Wierd level"<<suc->level<<endl;
        }
        temp->left=suc->left;
        suc->left=temp;
        temp->right=suc;
    }
    int tier=1,prefix{};
    while(tier!=W) {
        prefix=value>>(W-tier);
        if(XFast[tier].find(prefix)==XFast[tier].end()){
            node *inter=new node();
            inter->level=tier;
            inter->key=prefix;
            XFast[tier][prefix]=inter;
            if(prefix & 1) {
                XFast[tier-1][prefix>>1]->right=inter;
            } else {
                XFast[tier-1][prefix>>1]->left=inter;
            }
        }
        ++tier;
    }
    XFast[W][value]=temp;
    if(value & 1) {
        XFast[W-1][value>>1]->right=temp;
    } else {
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
    if(XFast[0][0]->left == nullptr or XFast[0][0]->left->level-1!=0) {
        XFast[0][0]->left = getLeftmostLeaf(XFast[0][0]->right);
    }
    if(XFast[0][0]->right == nullptr or XFast[0][0]->right->level-1!=0) {
        XFast[0][0]->right = getRightmostLeaf(XFast[0][0]->left);
    }
}
void del(int key) {
    int k=key;
    if(XFast[W].find(k)==XFast[W].end()) {
        cout<<"Element to be deleted is not present\n";
        return;   
    }
    node* temp=XFast[W][k];
    node *pre=temp->left;
    node *suc=temp->right;
    if(suc!=nullptr) {
        if(pre!=nullptr) {
            pre->right=suc;
            suc->left=pre;
        }
        else suc->left=nullptr;
    } else {
        if(pre!=nullptr) pre->right=nullptr;
    }
    vector<int> deletions;
    deletions.push_back(W);
    if(k&1) {
        if(XFast[W-1][k>>1]->left->key==k)
            deletions.push_back(W-1);
        else 
        XFast[W-1][k>>1]->right=pre;
    } else {
        if(XFast[W-1][k>>1]->right->key==k)
            deletions.push_back(W-1);
        else XFast[W-1][k>>1]->left=suc;
    }
    int tier{W-2};
    k>>=1;
    while(tier>=0) {
        if(k&1) {
            if(XFast[tier][k>>1]->left->key==key) {
                if(deletions.back()==tier+1)
                    deletions.push_back(tier);
                else XFast[tier][k>>1]->left=suc;
            }
            else if(tier+1==deletions.back())
                XFast[tier][k>>1]->right=pre;
        } else {
            if(XFast[tier][k>>1]->right->key==key) {
                if(deletions.back()==tier+1)
                    deletions.push_back(tier);
                else XFast[tier][k>>1]->right=pre;
            }
            else if(tier+1==deletions.back())
                XFast[tier][k>>1]->left=suc;
        }
        tier--;
        k>>=1;
    }
    for(int lvl: deletions) {
        XFast[lvl].erase(key>>(W-lvl));
    }
    cout<<"Deleted Successfully!\n";
}

void menu() {
    cout << "1. Insertion" <<\n;
    cout << "2. Deletion" <<\n;
    cout << "3. Successor" <<\n;
    cout << "4. Predecessor" <<\n;
    cout << "5. Search" <<\n;
    cout << "Enter your Choice: " <<\n;
    cin >> choice;
}

int main() {
    
    while(1) (
        
        menu();
        switch(choice) {

            case 1: {   
                //insertion
            }
            case 2: {  
                //deletion
            }
            case 3: {
                // Successor
            }
            case 4: {
                // Predecessor
            }
            case 5: {
                // Search
            }
            default: {
                cout << "Invalid Choice!" <<\n;        

        }
    }
        
    U=15;
    W=bitsCounter(U);
    // cout<<W<<'\n';
    XFast=vector<unordered_map<int, node*>>(W+1);
    XFast[0][0]=new node();
    XFast[0][0]->level=0;
    // insert(14);
    // // cout<<XFast[0][0]->right->key<<'\n';
    // insert(13);
    // // cout<<XFast[0][0]->right->key<<'\n';
    // insert(12);
    // // cout<<succesor(15)->key<<'\n';
    // cout<<succesor(13)->key<<'\n';
    // cout<<succesor(10)->key<<'\n';
    // cout<<predecessor(14)->key<<'\n';
    // del(13);
    // cout<<succesor(13)->key<<'\n';
    // del(12);
    // insert(1);
    // insert(10);
    // insert(12);
    // cout<<succesor(9)->key<<'\n';
    // cout<<predecessor(9)->key<<'\n';
    // del(14);
    // cout<<succesor(11)->key<<'\n';
    insert(1);
    insert(10);
    insert(12);
    insert(14);
    cout<<predecessor(9)->key<<'\n';
    del(12);
    cout<<succesor(11)->key<<'\n';
    del(14);
    cout<<succesor(11)->key<<'\n';
    return 0;
}
