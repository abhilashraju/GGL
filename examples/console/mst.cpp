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
#include "graphalgorithms.hpp"
#include "graph.hpp"
using namespace GGL;
int main()
{
    auto run=[](auto& g){
        std::cout<<"from Kruskal\n";
        m_s_t(m_s_t_kru(),g,[](const auto& e){
           std::cout<<"edge "<<e.either()<<" "<<e.other(e.either())<<"\n";
        });
        std::cout<<"from Primes\n";
        m_s_t(m_s_t_pri(),g,[](const auto& e){
           std::cout<<"edge "<<e.either()<<" "<<e.other(e.either())<<"\n";
        });
    };

    EdgeWeightedGraph<int> g(10);
    g.addEdge({1,2,1}).addEdge({3,4,10}).addEdge({3,1,5}).addEdge({4,6,7}).addEdge({6,3,8}).addEdge({4,2,3});
    run(g);

    EdgeWeightedGraph<std::string> sg;
    using SEdge=EdgeWeightedGraph<std::string>::Edge;
    sg.addEdge(SEdge("Abhilash","Abhila",1)).addEdge(SEdge("Pranav","Shreya",10)).addEdge(SEdge("Abhila","Shreya",5)).addEdge(SEdge("Abhilash","Pranav",7)).addEdge(SEdge("Kamalamma","Abhilash",8)).addEdge(SEdge("Abhila","Pranav",3));
    run(sg);

}
