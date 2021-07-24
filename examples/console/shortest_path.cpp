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
    auto run=[](auto& g){
        std::cout<<"Shortest path\n";
        s_s_shortest_path s_p;
        s_p(g,3,[](const auto& v,const auto& distance, const auto& parent){
           std::cout<<"Vertex "<<v<<" from "<<parent<<" with cost "<<distance<<"\n";
        });

    };

    DEdgeWeightedGraph<int> g(10);
    g.addEdge({1,2,1}).addEdge({3,4,10}).addEdge({3,1,5}).addEdge({4,6,7}).addEdge({6,3,8}).addEdge({4,2,3}).addEdge({0,1,5});
    g.addEdge({3,6,18});
    run(g);

//    EdgeWeightedGraph<std::string> sg;
//    using SEdge=EdgeWeightedGraph<std::string>::Edge;
//    sg.addEdge(SEdge("Abhilash","Abhila",1)).addEdge(SEdge("Pranav","Shreya",10)).addEdge(SEdge("Abhila","Shreya",5)).addEdge(SEdge("Abhilash","Pranav",7)).addEdge(SEdge("Kamalamma","Abhilash",8)).addEdge(SEdge("Abhila","Pranav",3));
//    run(sg);
    return 0;
}
