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
#include "union_find.hpp"
#include <iostream>
using namespace GGL;
int main()
{
    Union_find uf(10);
    uf.add(1,2).add(0,1).add(5,6).add(4,8).add(9,0).add(6,8).add(7,3).add(7,4).add(9,6);
    auto is_connected=[](auto& uf,auto a, auto b){auto con=uf.connected(a,b)?"true\n":"false\n";std::cout<< "connected "<<a<<" , "<<b<<" "<<con ;};
    for(int i=0 ;i<10;i++){
        for(int j=i+1; j<10;j++){
            is_connected(uf,i,j);
        }
    }

    Union_find<std::string> suf({"abhilash","abhila","Pranav","Shreya"});
    suf.add("abhilash","abhila").add("Pranav","Shreya");
    is_connected(suf,"abhilash","Pranav");
    is_connected(suf,"abhilash","abhila");
    suf.add("abhilash","Pranav");
    is_connected(suf,"abhila","Pranav");
    is_connected(suf,"abhila","Shreya");

}
