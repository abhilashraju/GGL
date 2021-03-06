#ifndef GRAPH_HPP
#define GRAPH_HPP
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
#include <vector>
#include <algorithm>
#include <deque>
#include <map>
#include <cmath>
#include <functional>
#include <strstream>
namespace GGL {
template<typename VTYPE,typename EdgeHandler>
struct GraphImpl
{
    using VertexType=VTYPE;
    using AdjList =std::vector<VertexType>;
    using hash_map = std::map<VertexType,AdjList>;
    using vec_map=std::vector<AdjList>;

    using GRAPH=typename std::conditional<std::is_integral_v<VTYPE>,vec_map,hash_map>::type;
    GRAPH m_adj_list;
    void intialise(int noofvertex,vec_map&){
        while(noofvertex-->0) m_adj_list.emplace_back();
    }
    void intialise(int noofvertex,hash_map&){

    }
    GraphImpl(int noofvertex){intialise(noofvertex,m_adj_list);}
    void add_edge(VertexType v1,VertexType v2){
        if(std::find(std::begin(m_adj_list[v1]),std::end(m_adj_list[v1]),v2)==std::end(m_adj_list[v1])){
            EdgeHandler h;
            h(m_adj_list,v1,v2);
        }
    }
    AdjList& adj_list(VertexType v){
        return m_adj_list[v];
    }

    void print(auto& os)
    {
        os<<"\nPrinting...\n";
        print(os,m_adj_list);

    }
    void print(auto& os, vec_map& vmap){
        int vertex=0;
        for(auto& l:vmap){
            if(l.size()){
                os<<vertex<<"-> ";
                for(auto& v:l){
                    os<<v<<" ";
                }
                os<<"\n";
            }
            vertex++;
        }
    }


    void print(auto& os, hash_map& vmap){

        for(auto& l:vmap){
            if(l.second.size()){
                os<<l.first<<"-> ";
                for(auto& v:l.second){
                    os<<v<<" ";
                }
                os<<"\n";
            }

        }
    }

    template<typename Handler>
    void for_each_vertices(Handler h,vec_map& vmap)
    {
        int index{0};
        for(auto& l:vmap){
            h(index);
        }
    }
    template<typename Handler>
    void for_each_vertices(Handler h,hash_map& vmap)
    {
        for(auto& l:vmap){
            h(l.first);
        }
    }
    template<typename Handler>
    void for_each_vertices(Handler h)
    {
        for_each_vertices(std::move(h),m_adj_list);
    }
    auto begin(){
        return  m_adj_list.begin();
    }
    auto end(){
        return  m_adj_list.end();
    }
    auto size()const{
        return m_adj_list.size();
    }
};
struct undirected{
    void operator()(auto& g,auto& v1, auto& v2){
        g[v1].push_back(v2);
        g[v2].push_back(v1);
    }
};
struct directed{
    void operator()(auto& g,auto& v1, auto& v2){
        g[v1].push_back(v2);
    }
};
using Graph=GraphImpl<int,undirected>;
using DGraph=GraphImpl<int,directed>;

using SGraph=GraphImpl<std::string,undirected>;
using SDGraph=GraphImpl<std::string,directed>;
template<typename ENTRY_TYPE>
struct MatrixIterators {
    struct Buffer
    {
        ENTRY_TYPE* data{nullptr};
        size_t s{};
        using Iterator =ENTRY_TYPE*;
        using value_type = ENTRY_TYPE;
        Buffer(ENTRY_TYPE* d, size_t s):data(d),s(s){}
        auto size()const {return  s;}
        auto begin(){return  data;}
        auto end(){
            return  data+s;
        }

    };

    struct MatrixType:std::vector<ENTRY_TYPE>{
        int row{0};
        int column{0};
        std::function<bool(const ENTRY_TYPE& val)> predicate;
        MatrixType(int r, int c):row(r),column(c){}
        MatrixType(std::pair<int,int> rc):row(rc.first),column(rc.second){}
        bool operator()(const  ENTRY_TYPE& val){
            return predicate(val);
        }
        template<typename OS>
        void print(OS& os)
        {
            int i{0};
            os<<"\n";
            while(i<row){
                int j{0};
                while(j<column){
                    os<<std::vector<ENTRY_TYPE>::at(i*row+j)<<" ";j++;
                }
                os<<"\n"; i++;
            }
        }
        template<typename Handler>
        void for_each(Handler h)
        {
            int i{0};
            while(i<row){
                int j{0};
                while(j<column){
                    h(i,j,std::vector<ENTRY_TYPE>::at(i*row+j));j++;
                }
                i++;
            }
        }

    };

    struct RowList{
        using VertexType=int;

        MatrixType& m;
        typename MatrixType::iterator b{};
        typename MatrixType::iterator e{};

        struct Iterator{

            VertexType index{-1};
            RowList& l;
            Iterator(RowList& l):l(l){}
            Iterator& operator++(){
                auto iter=std::find_if(l.b+index+1,l.e,l.m);
                if(iter != l.e){
                    index=std::distance(l.b,iter);
                    return *this;
                }
                index=-1;
                return *this;
            }
            VertexType& operator*(){
                return index;
            }

            friend bool operator==(const Iterator& f, const Iterator& s){
                return f.index == s.index;
            }
            friend bool operator!=(const Iterator& f, const Iterator& s){
                return !(f==s);
            }

        };

        Iterator begin(){
            auto iter = Iterator(*this);
            return ++iter;
        }
        Iterator end(){
            return Iterator(*this);

        }
        RowList(MatrixType& metrix,int start):m(metrix),b(m.begin()+start*metrix.column),e(b+metrix.column){}
        static RowList adj_list(MatrixType& metrix,const VertexType& v)
        {
            return RowList{metrix,v};
        }
        static void print(MatrixType& m,auto& os){
            VertexType vertex{};
            while(vertex<m.row){
                os<<"\n"<<vertex<<"->";
                for(auto l:adj_list(m,vertex)){
                    os<<l<<" ";

                }
                vertex++;
            }
            os<<"\n";
        }
        template<typename Handler>
        static void for_each_vertices(MatrixType& metrix,Handler h){
            int i{0};
            while(i<metrix.row){
                h(VertexType{i});
            }
        }
        static auto size(const MatrixType& m){
            return m.row;
        }

    };



    struct Neighbours8{
        using VertexType=std::pair<int,int>;
        using type=std::array<VertexType,8>;
        static constexpr std::array<VertexType,8> values={VertexType{-1,0},VertexType{-1,-1},VertexType{0,-1},VertexType{1,-1},VertexType{1,0},VertexType{1,1},VertexType{0,1},VertexType{-1,1},};


    };
    struct Neighbours4{
        using VertexType=std::pair<int,int>;
        using type=std::array<VertexType,4>;
        static constexpr std::array<VertexType,4> values={VertexType{-1,0},VertexType{0,-1},VertexType{1,0},VertexType{0,1}};
    };
    template<typename Neighbours>
    struct NeighbourList:Neighbours{
        using  VertexType=std::pair<int,int>;



        MatrixType& m;
        VertexType v{-1,-1};

        struct Iterator{

            typename Neighbours::type::const_iterator index{std::end(Neighbours::values)};

            NeighbourList& l;
            VertexType add(const VertexType& v1, const VertexType& v2)const{
                return  VertexType(v1.first+v2.first,v1.second+v2.second);
            }
            bool isValid(const VertexType& index){
                return (index.first >=0 && index.first <l.m.row && index.second >=0 && index.second <l.m.column);
            }
            Iterator(NeighbourList& l):l(l){}
            Iterator& operator++(){
                if(std::end(Neighbours::values) == index){
                    index = std::begin(Neighbours::values);
                }else{
                    index++;
                }
                index=std::find_if(index,std::end(Neighbours::values),[&](const auto& v){
                    auto next = add(v,l.v);
                    if(isValid(next) && l.m(l.m[next.first*l.m.row+next.second])){
                        return  true;
                    }
                    return false;
                });
                return *this;
            }
            VertexType operator*()const{
                return add(*index,l.v);
            }

            friend bool operator==(const Iterator& f, const Iterator& s){
                return f.index == s.index;
            }
            friend bool operator!=(const Iterator& f, const Iterator& s){
                return !(f==s);
            }

        };

        Iterator begin(){
            auto iter = Iterator(*this);
            return ++iter;
        }
        Iterator end(){
            return Iterator(*this);

        }
        ENTRY_TYPE& operator*(){
            return m[v.first*m.row+v.second];
        }
        NeighbourList(MatrixType& metrix,const VertexType& v):m(metrix),v(v){}
        static NeighbourList adj_list(MatrixType& metrix,const VertexType& v)
        {
            return NeighbourList{metrix,v};
        }
        static VertexType increment(MatrixType& m,const VertexType& v) {
            VertexType temp=v;
            temp.second+=1;
            if(temp.second>=m.column){
                temp.first+=1;
                temp.second=0;
            }
            return temp;
        }

        static std::string to_string(const VertexType& v){
            std::string s=" {";
            s.append(std::to_string(v.first));
            s.append(",");
            s.append(std::to_string(v.second));
            s.append( "} ");
            return  s;
        }
        static void print(MatrixType& m,auto& os){
            VertexType vertex{0,0};
            while(vertex.first<m.row && vertex.second <m.column){
                os<<"\n"<<to_string(vertex)<<"->";
                for(auto l:adj_list(m,vertex)){
                    os<<to_string(l)<<" ";

                }
                vertex=increment(m,vertex);
            }
            os<<"\n";
        }
        template<typename Handler>
        static void for_each_vertices(MatrixType& metrix,Handler h){
            int i{0};
            while(i<metrix.row){
                int j{0};
                while(j<metrix.column){
                    h(VertexType{i,j});j++;
                }
                i++;
            }
        }
        static void setValue(MatrixType& m,const ENTRY_TYPE& value,const VertexType& v){
            NeighbourList l(m,v);
            *l=value;
        }
        static ENTRY_TYPE& value(MatrixType& m,const VertexType& v){
            NeighbourList l(m,v);
            return *l;
        }
        static auto size(const MatrixType& m){
            return m.row*m.column;
        }


    };

    using CrossList = NeighbourList<Neighbours4>;
    using GridList = NeighbourList<Neighbours8>;


};
template<typename T,typename AdjList>
struct MatrixGraph
{
    using VertexType=typename AdjList::VertexType;

    using Matrix =typename MatrixIterators<T>::MatrixType;
    using Buffer =typename MatrixIterators<T>::Buffer;
    Matrix m;
    auto getDimension(const auto& data,int r){
        if (r<=0){
            return std::pair<int,int>(std::sqrt(data.size()),std::sqrt(data.size()));
        }

        return std::pair<int,int>(r,data.size()/r);
    }
    MatrixGraph():m(0,0){}
    MatrixGraph(Matrix m):m(std::move(m)){}
    MatrixGraph(const std::initializer_list<T>& data,int r=-1):m(getDimension(data,r)){

        std::copy(begin(data),end(data),std::back_inserter(m));
    }
    MatrixGraph(Buffer data,int r=-1):m(getDimension(data,r)){

        std::copy(std::begin(data),std::end(data),std::back_inserter(m));
    }
    template<typename Pred>
    MatrixGraph& setPredicate(Pred pred){
        m.predicate=std::move(pred);
        return *this;
    }
    MatrixGraph& setValue(const VertexType& v,const T& value){
        AdjList::setValue(m,value,v);
        return *this;
    }
    T& value(const VertexType& v){
        return AdjList::value(m,v);

    }
    MatrixGraph& printContent(auto& os){
        m.print(os);
        return *this;
    }
    MatrixGraph& print(auto& os){
        AdjList::print(m,os);
        return *this;
    }

    MatrixGraph& print(const VertexType& v,auto& os){
        os<<AdjList::to_string(v)<<" ";
        return *this;
    }
    static std::string to_string(const VertexType& v){
        return AdjList::to_string(v);
    }
    auto adj_list(VertexType v){
        return AdjList::adj_list(m,v);
    }
    template<typename Handler>
    auto for_each(Handler handler){
        return m.for_each(std::move(handler));
    }

    template<typename Handler>
    auto for_each_vertices(Handler handler){
        return AdjList::for_each_vertices(m,std::move(handler));
    }
    auto size()const
    {
        return AdjList::size(m);
    }

};
template<typename T>
using RowMatrix=MatrixGraph<T,typename MatrixIterators<T>::RowList>;
template<typename T>
using CrossMatrix=MatrixGraph<T,typename MatrixIterators<T>::CrossList>;
template<typename T>
using GridMatrix=MatrixGraph<T,typename MatrixIterators<T>::GridList>;


template <typename T,bool Directed=false>
struct EdgeWeightedGraph
{
    using VertexType=T;
    template<typename U>
    struct Comparable{
        const U& self()const{return static_cast<const U&>(*this);}
        bool operator<(const U& e2)const{
            return self().weight()<e2.weight();
        }
        bool operator>=(const U& e2)const{
            return !(self()<e2);
        }
        bool operator>(const U& e2)const{
            return e2<self();
        }
        bool operator<=(const U& e2)const{
            return !(e2<self());
        }
    };
    struct Edge:Comparable<Edge>{
        using VertexType=T;
        VertexType v1{};
        VertexType v2{};
        int w{};
        VertexType either()const {return v1;}
        VertexType other(VertexType v)const { return (v == v1)?v2:v1;}
        int weight()const{return w;}
        Edge(){}
        Edge(VertexType v1, VertexType v2, int w):v1{v1},v2{v2},w(w){}

        friend bool operator==(const Edge&e1, const Edge& e2){
            return e1.v1 == e2.v1 && e1.v2 == e2.v2 && e1.w == e2.w;
        }
        friend bool operator!=(const Edge&e1, const Edge& e2){
            return !(e1==e2);
        }
        auto to_string()const {std::strstream ss; ss<<"{ "<< v1<<" "<v2<<" "<<weight()<<"} "; return ss.str();}
        void add_me(auto& g)const{
            g[either()].push_back(*this);
            g[other(either())].push_back(*this);
        }
        void add_me_to_collection(auto& edgs)const{
            if(find(std::begin(edgs),std::end(edgs),*this)==std::end(edgs)){
                edgs.push_back(*this);
            }
        }
    };
    struct DEdge:Comparable<DEdge>{
        using VertexType=T;
        VertexType v1{};
        VertexType v2{};
        int w{};
        VertexType from()const {return v1;}
        VertexType to()const { return v2;}
        int weight()const{return w;}
        DEdge(){}
        DEdge(VertexType v1, VertexType v2, int w):v1{v1},v2{v2},w(w){}

        friend bool operator==(const DEdge&e1, const DEdge& e2){
            return e1.v1 == e2.v1 && e1.v2 == e2.v2 && e1.w == e2.w;
        }
        friend bool operator!=(const DEdge&e1, const DEdge& e2){
            return !(e1==e2);
        }
        void add_me(auto& g)const{
            g[from()].push_back(*this);
            g[to()];//to keep the vertices in the list;
        }
        void add_me_to_collection(auto& edgs){
            edgs.push_back(*this);
        }
        auto to_string()const {std::strstream ss; ss<<"{ "<< from()<<" "<to()<<" "<<weight()<<"} "; return ss.str();}


    };
    using value_type=typename std::conditional_t<Directed,DEdge,Edge>;
    using AdjList =std::vector<value_type>;
    using hash_map = std::map<VertexType,AdjList>;
    using vec_map=std::vector<AdjList>;
    using GRAPH=typename std::conditional<std::is_integral_v<VertexType>,vec_map,hash_map>::type;
    GRAPH g;
    EdgeWeightedGraph(int noofvertex){std::generate_n(std::back_inserter(g),noofvertex,[](){return std::vector<value_type>();});}
    EdgeWeightedGraph(){static_assert (std::is_same_v<hash_map,decltype (g)>,"call this constructor for non integral vertex types" );}
    EdgeWeightedGraph& addEdge(const value_type& e){
        e.add_me(g);
        return *this;
    }
    const auto& adj_list(const VertexType& v)const{
        return const_cast<EdgeWeightedGraph*>(this)->g[v];
    }
    template<typename OS>
    EdgeWeightedGraph& print(OS& os){
        return  print(os,g);

    }
    template<typename OS>
    EdgeWeightedGraph& print(OS& os,vec_map& map){
        int vertex=0;
        os<<"\n";
        for(const auto& adj:map){
            os<<vertex<<"-> ";
            for(const auto& e:adj){
                os<<e.to_string();
            }
            os<<"\n";vertex++;
        }
        os<<"\n";
        return *this;
    }
    template<typename OS>
    EdgeWeightedGraph& print(OS& os,hash_map& map){

        os<<"\n";
        for(const auto& p:map){
            os<<p.first<<"-> ";
            for(const auto& e:p.second){
                os<<e.to_string();
            }
            os<<"\n";
        }
        os<<"\n";
        return *this;
    }
    auto first()const{
        return first(g);
    }
    auto first(const vec_map& m)const{
        for(auto& l:m){
            if(l.size()){
                return *std::begin(l);
            }
        }
        return value_type();
    }
    auto first(const hash_map& m)const{
        for(auto& l:m){
            if(l.second.size()){
                return *std::begin(l.second);
            }
        }
        return value_type();
    }
    auto edges()const{
        return edges(g);
    }
    auto edges(const vec_map& m)const{
        //find a better algorithm or data structure to avoid search for duplicate
        std::vector<value_type> edgs;
        for(auto& l:m){
            for(auto& e:l){
                if(find(std::begin(edgs),std::end(edgs),e)==std::end(edgs)){
                    edgs.push_back(e);
                }
            }
        }
        return edgs;
    }
    auto edges(const hash_map& m)const {
        //find a better algorithm or data structure to avoid search for duplicate
        std::vector<value_type> edgs;
        for(auto& l:m){
            for(auto& e:l.second){
                e.add_me_to_collection(edgs);
            }
        }
        return edgs;
    }
    auto find_edge(const auto& v1, const auto& v2){
        const auto& edjlist=g[v1];
        auto iter = std::find_if(std::begin(edjlist),std::end(edjlist),[&](auto& e){
            return e.from() == v1 && e.to()==v2;
        });
        if(iter != std::end(edjlist)){
            return *iter;
        }
        return value_type{VertexType{},VertexType{},0};
    }
    template<typename Handler>
    void for_each_vertices(const hash_map& m,Handler h)const{
        for(const auto& p: m){
            h(p.first);
        }
    }
    template<typename Handler>
    void for_each_vertices(const vec_map& m,Handler h)const{
        VertexType i{0};
        while (i<m.size()) {
            h(i++);
        }
    }

    template<typename Handler>
    void for_each_vertices(Handler h)const{
        for_each_vertices(g,std::move(h));
    }
    auto size()const{
        return g.size();
    }
    auto begin(){
        return g.begin();
    }
    auto end(){
        return  g.end();
    }

};
template <typename T>
using DEdgeWeightedGraph=EdgeWeightedGraph<T,true>;
}


#endif // GRAPH_HPP
