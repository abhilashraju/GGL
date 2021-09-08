#include "tree_sorts.hpp"
#include <iostream>
int main (){
    using namespace GGL;
    using namespace std;
    BST<int , string> bst;
    bst.insert(1,"abhi");
    cout << bst.get(2)<<"\n";
    cout << bst.get(1)<<"\n";
    bst.insert(1,"abhila");
    bst.insert(2,"abhilash");
    bst.insert(3,"pranav");
    bst.insert(11,"shreya");
    cout << bst.get(1)<<"\n";
    cout << bst.get(2)<<"\n";
    cout << bst.get(3)<<"\n";
    cout << bst.get(4)<<"\n";
    
    for(auto& v:bst){
         cout <<"{"<<v.key<<" , "<<v.value<<"}";
    }
    cout <<"\n";
    cout<<bst.ceil(10).second <<" "<<bst.floor(10).second<<"\n";
    bst.delete_key(11);
    bst.visit([](auto& k, auto& v){
        cout <<"{"<<k<<" , "<<v<<"}";
    });
}
