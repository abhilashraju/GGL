#ifndef GRAPHALGORITHMS_HPP
#define GRAPHALGORITHMS_HPP
#include <vector>
#include <algorithm>
#include <deque>
#include <map>
#include <cmath>
#include "priorityqueue.hpp"
#include "union_find.hpp"
namespace GGL {
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



struct m_s_t_kru{
    auto make_uf(auto& g,int){
         return  Union_find<int>(g.size());
    }
    auto make_uf(auto& g,auto any){
        Union_find<decltype (any)> uf;
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
{
    auto make_visited(auto& g,int){
        std::vector<bool> visited;
        std::generate_n(std::back_inserter(visited),g.size(),[](){return false;});
        return visited;
    }
    auto make_visited(auto& g,auto v){
        return std::map<decltype (v),bool>{};
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

struct s_s_shortest_path{

    auto make_visited(const auto& g,int){
        std::vector<bool> visited;
        std::generate_n(std::back_inserter(visited),g.size(),[](){return false;});
        return visited;
    }
    auto make_visited(const auto& g,auto v){
        return std::map<decltype (v),bool>{};
    }
    auto make_aux_arrays(const auto& g, int){
        std::vector<int> lengths;
        std::vector<int> parents;
        std::generate_n(std::back_inserter(lengths),g.size(),[](){return std::numeric_limits<int>::max();});
        std::generate_n(std::back_inserter(parents),g.size(),[](){return -1;});
        return std::make_tuple(lengths,parents);
    }
    auto make_aux_arrays(const auto& g, auto v){
        using  VertexType = decltype (v);
        std::map<VertexType,int> lengths;
        std::map<VertexType,VertexType> parents;
        g.for_each_vertices([&](auto v){
            lengths[v]=std::numeric_limits<int>::max();
            parents[v]=VertexType{};
        });
        return std::make_tuple(lengths,parents);
    }
    auto relax(const auto& e,const auto& pathlengs,const auto& parents){
        auto newlength=pathlengs.at(e.from()) + e.weight();
        if(pathlengs.at(e.to())> newlength){
            return std::make_tuple(std::make_pair(e.to(),newlength),newlength,e.from());
        }
        return std::make_tuple(std::make_pair(e.to(),pathlengs.at(e.to())),pathlengs.at(e.to()),parents.at(e.to()));
    }
    void operator()(const auto& g,const auto& source,auto visit_handler){
        using  Edge=typename std::remove_reference_t<std::remove_cv_t<decltype(g)>>::value_type;
        using  VertexType= typename Edge::VertexType;

        auto [path_lengths,parents]=make_aux_arrays(g,VertexType{});
        auto visited=make_visited(g,VertexType{});
        struct lesser{
            bool operator()(const std::pair<VertexType,int>& v1,const std::pair<VertexType,int>& v2){
              return v1.second<v2.second;
            }
        };
        PriorityQueue<std::pair<VertexType,int>,lesser> queue;
        path_lengths[source]=0;
        parents[source]=source;
        queue.push_back({source,path_lengths[source]});
        while(!queue.empty()){
            auto current=queue.take();
            visited[current.first]=true;

            for(auto e:g.adj_list(current.first)){
                if(!visited[e.to()]){
                    std::pair<VertexType,int> relaxed;
                    std::tie(relaxed,path_lengths[e.to()],parents[e.to()])=relax(e,path_lengths,parents);
                    queue.update_or_add(relaxed,[&](const auto& other){return relaxed.first == other.first;});
                }
            }
            visit_handler(current.first,path_lengths[current.first],parents[current.first]);
        }
    }
};
}



#endif // GRAPHALGORITHMS_HPP
