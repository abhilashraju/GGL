#include "graphalgorithms.hpp"
#include "graph.hpp"
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
