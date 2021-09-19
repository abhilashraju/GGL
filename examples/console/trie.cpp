/*
MIT License

Copyright (c) 2021 abhilashraju

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include "trie.hpp"

int main (){
    using namespace GGL;
    TernaryTrie<std::string,std::string> trie;
    trie.insert("abhilash","swiss");
    trie.insert("abhila","rus");
    trie.insert("pranav","ken");
    trie.insert("shreya","man");
    trie.insert("shreyakutty","uga");
    trie.print(std::cout);
    std::cout<<trie.get("abhilash")<<" ";
    std::cout<<trie.get("pranav")<<"\n";
    for(auto v: trie.keys()){
            std::cout<<" "<<v<<" ";
    }
    for(auto v: trie.suffixes("p")){
            std::cout<<" "<<v<<" ";
    }
}
