#include "priorityqueue.hpp"
using namespace GGL;
int main (){

    std::vector ran={1,4,2,5,6,7,3,8,9,2,12,11,11,11,11};
    PriorityQueue<int> q;
    for(auto v:ran){
        q.insert(v);
    }
     q.print();
     q.remove(12);
      q.print();
    while(q.size()){
        std::cout<<q.take()<<" ";
    }

    std::vector<std::pair<std::string, int>> pairs{
             {"Abhilash",5},
             {"Abhila",4},
             {"Pranav",3},
             {"Shreya",1},
               {"Raju",10},
         {"kamalamma",9},
    };
    struct comp{
        bool operator()(const std::pair<std::string, int>& first,const std::pair<std::string, int>& second){
                return first.second > second.second;
        }
    };

    using family=PriorityQueue<std::pair<std::string, int>,comp>;
    family pq;
    for(auto v: pairs){
        pq.insert(v);
    }
    family pq2;
    while(pq.size()){
        auto v=pq.take();
        pq2.insert(v);
        std::cout<<v.first<<" ";
    }

    pq2.update(std::make_pair("kamalamma",12),[](auto v){
        return v.first == "kamalamma";
    });
    pq2.update(std::make_pair("Abhilash",0),[](auto v){
        return v.first == "Abhilash";
    });
    std::cout<<"\n";
    while(pq2.size()){
        auto v=pq2.take();
        pq.insert(v);
        std::cout<<v.first<<" ";
    }
    std::cout<<"\n";
    for(auto v: pq){
        std::cout<<v.first<<" ";
    }
    pq.remove(std::make_pair("kamalamma",12));
    std::cout<<"\n";
    for(auto v: pq){
        std::cout<<v.first<<" ";
    }
    std::cout<<"\n";
    while(pq.size()){
        auto v=pq.take();
        std::cout<<v.first<<" ";
    }
}
