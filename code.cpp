#include <bits/stdc++.h>
using namespace std;

// node of the trie
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
// Size of universe
int U, W;
// Xfast-Trie
vector<unordered_map<int, node*>> XFast;

// Counts number of bits in a number
int bitsCounter(int a) {
    int count{};
    while(a!=0){
        count++;
        a>>=1;
    }
    return count;
}

// search function
node* find(int k) {
    if(XFast[W].find(k)!=XFast[W].end()) 
        return XFast[W][k];
    return nullptr;
}

// finds an element >= the entered element
node* succesor(int k) {
    // binary search to find highest matching prefix
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
    // check if the value is larger or smaller, traverse linked list accordingly by one step
    if(temp->level==W) return temp;
    int shift=W-temp->level-1;
    int bigPrefix=k>>shift;
    if(bigPrefix&1) temp=temp->right;
    else temp=temp->left;
    if(temp->key<k) return temp->right;
    return temp;
}

// finds an element <= the entered element
node* predecessor(int k) {
    // binary search to find highest matching prefix
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
    // check if the value is larger or smaller, traverse linked list accordingly by one step
    if(temp->level==W) return temp;
    int shift=W-temp->level-1;
    int bigPrefix=k>>shift;
    if(bigPrefix& 1) temp=temp->right;
    else temp=temp->left;
    if(temp->key>k) return temp->left;
    return temp;
}

// returns the smallest element in a sub-trie
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
// returns the largest element in a sub-trie
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

// inserts an element in the XFast-Trie
void insert(int value) {
    node *temp=new node();
    temp->key=value;
    temp->level=W;
    // find successor predecessor of the value, add element in between them in linked list in final level
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
    // traverse through all levels and add branches 
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
    // connect and update the descendant pointers
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

// deletes an element from the XFast-Trie
void del(int key) {
    int k=key;
    if(XFast[W].find(k)==XFast[W].end()) {
        cout<<"Element to be deleted is not present\n";
        return;   
    }
    node* temp=XFast[W][k];
    node *pre=temp->left;
    node *suc=temp->right;
    // remove the element out of the linked list in the last level
    if(suc!=nullptr) {
        if(pre!=nullptr) {
            pre->right=suc;
            suc->left=pre;
        }
        else suc->left=nullptr;
    } else {
        if(pre!=nullptr) pre->right=nullptr;
    }
    // delete all the threads that are associated with only the element being deleted
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
    // update descendant pointers at all levels
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

// prints the menu options
int menu() {
    int choice;
    cout << "1. Insertion" <<'\n';
    cout << "2. Deletion" <<'\n';
    cout << "3. Successor" <<'\n';
    cout << "4. Predecessor" <<'\n';
    cout << "5. Search" <<'\n';
    cout << "0. Exit" <<'\n';
    cout << "Enter your Choice: ";
    cin >> choice;
    return choice;
}

// main function
int main() {
    cout<<"Enter size of universe: ";
    cin>>U;
    W=bitsCounter(U);
    XFast=vector<unordered_map<int, node*>>(W+1);
    XFast[0][0]=new node();
    XFast[0][0]->level=0;
    int choice{1};
    while(choice) {
        choice = menu();
        switch(choice) {
            case 0: break;
            case 1: {   
                cout<<"Enter element to insert: ";
                int n; cin>>n;
                insert(n);
                break;
            }
            case 2: {  
                cout<<"Enter element to delete: ";
                int n; cin>>n;
                del(n);
                break;
            }
            case 3: {
                // Successor
                cout<<"Enter element: ";
                int n; cin>>n;
                node* x=succesor(n);
                if(x!=nullptr)
                    cout<<"Succesor of "<<n<<" is: "<<x->key<<'\n';
                else cout<<n<<" does not have a succesor!\n";
                break;
            }
            case 4: {
                // Predecessor
                cout<<"Enter element: ";
                int n; cin>>n;
                node* x=predecessor(n);
                if(x!=nullptr)
                    cout<<"Predecessor of "<<n<<" is: "<<x->key<<'\n';
                else cout<<n<<" does not have a predecessor!\n";
                break;
            }
            case 5: {
                // Search
                cout<<"Enter element: ";
                int n; cin>>n;
                node* x=find(n);
                if(x!=nullptr)
                    cout<<"Found with key as: "<<x->key<<'\n';
                else cout<<n<<" not found\n";
                break;
            }
            default: {
                cout << "Invalid Choice!\n";        
            }
        }
    }
    return 0;
}
