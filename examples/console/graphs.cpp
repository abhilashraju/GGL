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
    auto empty_visitor=[](auto v){};
    auto empty_childhandler=[](auto p,auto c){};
    auto printparents=[](auto& map,auto node,auto root,auto tostring){
        auto p=node;
        std::cout << tostring(p);
        while(p!=root){
            p=map[p];
            std::cout <<" << " << tostring(p);
        }
    };


    auto print_connected=[](auto& g){
        using  VertexType=typename std::remove_reference_t<decltype(g)>::VertexType;

        ConnectedMap<VertexType> map;
        Connected_Cmponents{}(g,[&](auto index,auto v){
            map[index].push_back(v);
        });
        std::cout<< "\nPrinting connected components..\n";
        for(auto& l:map){
             std::cout<<l.first<< "= ";
            for(auto& e:l.second){
                std::cout<<e<<" ";
            }
            std::cout<<"\n";
        }
    };

    DGraph g(10);
    g.add_edge(0,2);
    g.add_edge(0,5);
    g.add_edge(2,5);
    g.add_edge(1,5);
    g.add_edge(5,1);
    g.print(std::cout);
    std::vector<int> parents;
    parents.reserve(10);
    travers(g,0,GDFS(),[&](auto p,auto c){
       parents[c]=p;
    },empty_visitor);
    printparents(parents,1,0,[](auto v){return std::to_string(v);});



    print_connected(g);


    SDGraph sg(10);
    sg.add_edge("Abhilash","Abhila");
    sg.add_edge("Abhilash","Pranav");
    sg.add_edge("Abhila","Pranav");

    sg.print(std::cout);

    std::map<std::string,std::string> strparents;
    travers(sg,"Abhilash",GDFS(),[&](auto p,auto c){
        strparents[c]=p;
    },empty_visitor);
    printparents(strparents,std::string("Pranav"),std::string("Abhilash"),[](auto& v){return v;});

    print_connected(sg);


    RowMatrix<int> mg({
                       1,0,0,1,
                       1,1,0,1,
                       0,0,1,1,
                       1,0,1,1,
                     },4);
    mg.setPredicate([](int c){return c==1;}).print(std::cout);
    mg.printContent(std::cout);
    travers(mg,1,GBFS(),[&](auto p,auto c){

    },[](auto v){
        std::cout<<v<<" ";
    });

    print_connected(mg);


    using CGVertex=typename CrossMatrix<char>::VertexType;
    CrossMatrix<char> cg({
                       'a','b','a','a',
                       'b','b','b','a',
                       'a','b','a','a',
                       'b','b','a','a',
                     });
    cg.setPredicate([](char c){return c=='a';}).printContent(std::cout);

    std::map<CGVertex,CGVertex> vertexParents;
    travers(cg,CGVertex{3,3},GDFS(),[&](auto p,auto c){
        vertexParents[c]=p;
        std::cout<< cg.to_string(p)<<cg.to_string(c)<<"\n";
    },[&](auto v){
        cg.setValue(v,'b');
    });
    printparents(vertexParents,CGVertex{0,2},CGVertex{0,0},[&](auto& v){return cg.to_string(v);});
    cg.printContent(std::cout);

     print_connected(cg);
    return 0;
}
