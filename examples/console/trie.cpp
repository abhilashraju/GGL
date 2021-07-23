#include "trie.hpp"
using namespace GGL;
int main (){
    Trie myTrie;
    myTrie.insert("hello","john");
    myTrie.insert("hi","john");
    myTrie.insert("t","abhi");
    myTrie.insert("histor","I am");
    myTrie.insert("he","I am");
    // myTrie.print();

    std::cout<<myTrie.find("t")<<"\n";
    auto suflist=myTrie.get_starts_with("hi");
    for(auto& s:suflist){
        std::cout<<s <<"  ";
    }
}
