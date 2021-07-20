#pragma once
#include <vector>
#include <map>
#include <algorithm>

template<typename VertexType=int>
struct Union_find
{

    using NODE=std::pair<VertexType,int>;
    using NODE_CONTAINER = typename std::conditional_t<std::is_integral<VertexType>::value,std::vector<NODE>,std::map<VertexType,NODE>>;
    using value_type=VertexType;
    NODE_CONTAINER roots;
    Union_find(){
    }
    Union_find(int nov){
        initialise(nov);
    }
    Union_find(const std::initializer_list<VertexType>& vertices){
       initialise(vertices);
    }
    void initialise(int nov){
        std::generate_n(std::back_inserter(roots),nov,[i=0]()mutable{return NODE{i++,1};});
    }
    void initialise(const auto& vertices){
        for(auto& v:vertices){
              roots[v]=NODE{v,1};
         };
    }
    void push_back(const VertexType& v){
        push_back(roots,v);
    }
    void push_back(std::vector<NODE>& c, const VertexType& v){
        c.push_back(NODE{v,1});
    }
    void push_back(std::map<VertexType,NODE>& c, const VertexType& v){
        c[v]=NODE{v,1};
    }
    auto& root(const VertexType& v){
        NODE current{v,1};
        while(roots[current.first].first!=current.first){roots[current.first]=roots[roots[current.first].first];current=roots[current.first];}
        return roots[current.first];
    }

    Union_find& add(const VertexType& v1, const VertexType& v2){
         auto& v1root=root(v1);
         auto& v2root=root(v2);
        if(v1root != v2root){
            if(v1root.second > v2root.second){
                v1root.second+=v2root.second;
                roots[v2root.first]=v1root;
                return *this;
            }
            v2root.second+=v1root.second;
            roots[v1root.first]=v2root;
        }
        return *this;
    }
    bool connected(const VertexType& v1, const VertexType& v2){
        return root(v1)==root(v2);
    }
    auto size(){
        return roots.size();
    }
};
