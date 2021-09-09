#include "interval_tree.hpp"
#include "utilities.hpp"
#include <iostream>
int main (){
    using namespace GGL;
    using namespace std;
    Interval_tree<int> inttree;
    inttree.insert(1,10);
    inttree.insert(13,15);
    inttree.insert(21,22);
 
    for(auto& v:inttree){
         cout <<"{"<<v.key<<" , "<<v.value<<"}";
    }
    cout <<"\n";
    cout <<inttree.intersected(12,22)<<"\n";
  
  
    Tree_Utilities<Interval_tree<int>,5>::print(inttree);
    inttree.delete_key(1);
    inttree.visit([](auto& k, auto& v){
        cout <<"{"<<k<<" , "<<v<<"}";
    });
    
}
