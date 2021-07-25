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
