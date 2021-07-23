#ifndef GRAPHALGORITHMS_HPP
#define GRAPHALGORITHMS_HPP
#include <vector>
#include <algorithm>
#include <deque>
#include <map>
#include <cmath>
struct DFS{

    void dfs(auto& g ,auto& visited ,auto v,auto& child_handler,auto& visithandler){
        if(std::find(begin(visited),end(visited),v)==end(visited)){
            visited.push_back(v);
            visithandler(v);
            for(auto& c:g.adj_list(v)){
               dfs(g,visited,c,visithandler);
               child_handler(v,c);
            }
        }
    }
    void operator()(auto& g ,auto v,auto& child_handler,auto& visithandler){
         using VertexType=typename std::remove_reference_t<decltype(g)>::VertexType;
        std::vector<VertexType> visited;
        dfs(g,visited,v,child_handler,visithandler);
    }
};
struct BFS{

    void operator()(auto& g ,auto v,auto& child_handler,auto& visithandler){
        using VertexType=typename std::remove_reference_t<decltype(g)>::VertexType;
        std::vector<VertexType> visited;
        std::deque<VertexType> queue;
        queue.push_back(v);
        while(!queue.empty()){
            auto current=queue.front();
            queue.pop_front();
            if(std::find(begin(visited),end(visited),current)==end(visited)){
                visited.push_back(current);
                visithandler(current);
                for(auto& c:g.adj_list(current)){
                    queue.push_back(c);
                    child_handler(current,c);
                }
            }
        }

    }
};
template<typename PUSHSTRATEGY>
struct BFS_DFS:PUSHSTRATEGY{

    void operator()(auto& g ,auto v,auto& child_handler,auto& visithandler){
        using VertexType=typename std::remove_reference_t<decltype(g)>::VertexType;
        std::vector<VertexType> visited;
        std::deque<VertexType> queue;
        queue.push_back(v);
        visited.push_back(v);
        while(!queue.empty()){
            auto current=queue.front();
            queue.pop_front();
            visithandler(current);
            for(const auto& c:g.adj_list(current)){
                if(std::find(begin(visited),end(visited),c)==end(visited)){
                    visited.push_back(c);
                    PUSHSTRATEGY::operator()(queue,c);
                    child_handler(current,c);
                }

            }
        }


    }
};
struct make_bfs_push{
    void operator ()(auto& q,auto v){q.push_back(v);}
};
struct make_dfs_push{
    void operator()(auto& q,auto v){q.push_front(v);}
};
using GBFS=BFS_DFS<make_bfs_push>;
using GDFS=BFS_DFS<make_dfs_push>;
inline void travers(auto& g,auto root,auto alg,auto child_handler,auto handler){

    alg(g,root,child_handler,handler);
}
struct Connected_Cmponents
{
    auto make_visited(auto& g,int){
        std::vector<bool> v;
        std::generate_n(std::back_inserter(v),g.size(),[](){return false;});
        return v;
    }
    auto make_visited(auto& g,auto any){
        using  VertexType=typename std::remove_reference_t<decltype(g)>::VertexType;
        std::map<VertexType,bool> map;
        g.for_each_vertices([&](auto v){
            map[v]=false;
        });
        return map;
    }
    auto next_non_visited(std::vector<bool>& visited){
        auto iter=std::find(visited.begin(),visited.end(),false);
        if(iter != visited.end()){
            return std::pair<int,bool>{std::distance(visited.begin(),iter),true};
        }
        return std::pair<int,bool>{-1,false};

    }
    auto next_non_visited(auto& visited){
        using  KeyType=typename std::remove_reference_t<decltype(visited)>::key_type;
        auto iter=std::find_if(visited.begin(),visited.end(),[](auto& v){return v.second ==false;});
        if(iter != visited.end()){
            return std::pair<KeyType,bool>{iter->first,true};
        }
        return std::pair<KeyType,bool>{KeyType{},false};
    }
    void operator()(auto& g ,auto visithandler){
        using  VertexType=typename std::remove_reference_t<decltype(g)>::VertexType;
        auto map=make_visited(g,VertexType{});
        auto v= next_non_visited(map);
        int compid=1;
        while(v.second){
            travers(g,v.first,GDFS{},[&](auto& ,auto& ){},[&](const auto& c){
                map[c]=true;
                visithandler(compid,c);
            });
            compid++;
            v=next_non_visited(map);
        }
     }
};


#include "priorityqueue.hpp"
#include "union_find.hpp"
struct m_s_t_kru{
    auto make_uf(auto& g,int){
        using  Edge=typename std::remove_reference_t<decltype(g)>::value_type;
        using VertexType=typename Edge::VertexType;
        return  Union_find<VertexType>(g.size());
    }
    auto make_uf(auto& g,auto any){
        using  Edge=typename std::remove_reference_t<decltype(g)>::value_type;
        using VertexType=typename Edge::VertexType;
        Union_find<VertexType> uf;
        std::transform(std::begin(g),std::end(g),std::back_inserter(uf),[](auto& v){
           return v.first;
        });
        return uf;
    }
    auto operator()(auto& g,auto on_add_handler)
    {
        using  Edge=typename std::remove_reference_t<decltype(g)>::value_type;
        PriorityQueue<Edge> pq;
        for(auto& e:g.edges()){
            pq.insert(e);
        };
        using VertexType=typename Edge::VertexType;
        Union_find<VertexType> uf=make_uf(g,VertexType{});

        size_t totaledges=0;
        while(pq.size()&& totaledges < g.size()){
            auto e =pq.take();
            if(!uf.connected(e.either(),e.other(e.either()))){
                  uf.add(e.either(),e.other(e.either()));
                  on_add_handler(e);
                  totaledges++;
            }
        }

    }
};

struct m_s_t_pri
{   auto make_visited(auto& g,int){
        using  Edge=typename std::remove_reference_t<decltype(g)>::value_type;
        using  VertexType= typename Edge::VertexType;
        using  VISITED_CONTAINER=typename std::conditional_t<std::is_integral_v<VertexType>,std::vector<bool>,std::map<VertexType,bool>>;
        VISITED_CONTAINER visited;
        std::generate_n(std::back_inserter(visited),g.size(),[](){return false;});
        return visited;
    }
    auto make_visited(auto& g,auto ){
        using  Edge=typename std::remove_reference_t<decltype(g)>::value_type;
        using  VertexType= typename Edge::VertexType;
        using  VISITED_CONTAINER=typename std::conditional_t<std::is_integral_v<VertexType>,std::vector<bool>,std::map<VertexType,bool>>;
        return VISITED_CONTAINER{};
    }
    auto operator()(auto& g,auto on_add_handler)
    {
        using  Edge=typename std::remove_reference_t<decltype(g)>::value_type;
        using  VertexType= typename Edge::VertexType;
        PriorityQueue<Edge> pq;
        auto visited=make_visited(g,VertexType{});
        auto visit=[&](const auto& v){
          visited[v]=true;
          for(auto& edge:g.adj_list(v)){
              pq.insert(edge);
          }
        };
        const auto& first =g.first();
        visit(first.either());
        size_t totaledges=0;
        while(pq.size()&& totaledges < g.size()){
            auto e =pq.take();
            const auto& v1=e.either();
            const auto& v2=e.other(e.either());

            if(!visited[v1] || !visited[v2]){
                on_add_handler(e);
                if(!visited[v1]) visit(v1);
                if(!visited[v2]) visit(v2);
            }
        }

    }
};
inline auto m_s_t(auto Algo,auto& g,auto on_add_handler){
    return Algo(g,std::move(on_add_handler));
}


#endif // GRAPHALGORITHMS_HPP
