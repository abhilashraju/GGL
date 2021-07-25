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
#include "graph.hpp"
#include "graphalgorithms.hpp"
#include <iostream>
using namespace GGL;
auto& operator<<(auto& os,const std::pair<int,int>& e){
    os<<"{ "<<e.first<<", "<<e.second<<" } ";
    return os;
}
template<typename T>
using ConnectedMap = std::map<int,std::vector<T>>;
int main (){
    auto run=[](auto& g,const auto& start){
        std::cout<<"Shortest path\n";
        s_s_shortest_path s_p;
        using  VertexType = typename std::remove_reference_t<decltype (g)>::VertexType;
        std::map<VertexType,VertexType> paths;
        std::map<VertexType,int> distances;
        s_p(g,start,[&](const auto& v,const auto& distance, const auto& parent){
            paths[v]=parent;distances[v]=distance;

        });
        for(auto& p:paths){
            std::cout<<"Vertex "<<p.first;
            auto via=p.first;
            while(via!=paths[via]){
                via=paths[via];
                std::cout<<" via "<<via;

            }
            std::cout<<" with cost "<<distances[p.first]<<"\n";
        }

    };

    DEdgeWeightedGraph<int> g(10);
    g.addEdge({1,2,1}).addEdge({3,4,10}).addEdge({3,1,5}).addEdge({4,6,7}).addEdge({6,3,8}).addEdge({4,2,3}).addEdge({0,1,5});
    g.addEdge({3,6,18});
    run(g,3);

    DEdgeWeightedGraph<std::string> sg;
    using SEdge=DEdgeWeightedGraph<std::string>::value_type;
    sg.addEdge(SEdge("Abhilash","Abhila",1)).addEdge(SEdge("Pranav","Shreya",10)).addEdge(SEdge("Abhila","Shreya",5)).addEdge(SEdge("Abhilash","Pranav",7)).addEdge(SEdge("Kamalamma","Abhilash",8)).addEdge(SEdge("Abhila","Pranav",3));
    sg.addEdge(SEdge("Pranav","Abhilash",10)).addEdge(SEdge("Pranav","Kamalamma",1));
    run(sg,"Abhila");
    return 0;
}
