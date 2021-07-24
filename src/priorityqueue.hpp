#ifndef PRIORITYQUEUE_HPP
#define PRIORITYQUEUE_HPP
#include <deque>
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>
namespace GGL {
template<typename T,typename Compare=std::less<T>>
struct PriorityQueue
{
    using  value_type=T;
    std::deque<T> queue{T{}};
    void swim(int N){
        while(N>1){

            if(Compare()(queue[N/2] , queue[N])){
                break;
            }
            std::swap(queue[N/2],queue[N]);
            N=N/2;

        }
    }
    void insert(T v){
        queue.push_back(std::move(v));
        swim(queue.size()-1);
    }
    void push_back(T v){
        insert(std::move(v));
    }
    void sink(int N){
        int child = N*2;
        while(child<queue.size()){

            if((child+1)<queue.size()) {
                child =(Compare()(queue[child],queue[child+1]))? child: child+1;
            }
            if(Compare()(queue[N], queue[child])){
                break;
            }
            std::swap(queue[N],queue[child]);
            N =child;child=child*2;
        }
    }
    T take(){

        T ret = queue[1];
        std::swap(queue[1],queue[queue.size()-1]);
        queue.pop_back();
        sink(1);

        return ret;
    }
    template<typename EqComp>
    bool update(const T& d,EqComp comp){
        auto iter = std::find_if(std::begin(queue),std::end(queue),comp);
        if(iter != std::end(queue)){
            auto distance = std::distance(std::begin(queue),iter);
            auto swi=Compare()(d,*iter);
            *iter=d;
            if(swi){
                swim(distance);
                return true;
            }
            sink(distance);
            return true;
        }
        return false;
    }
    bool update(const T& d){
        return update(d,[&](auto f){return f==d;});
    }
    template<typename EqComp>
    void update_or_add(const T& d,EqComp comp){
        if(update(d,std::move(comp))){
            return ;
        }
        insert(d);
    }
    void update_or_add(const T& d){
        update_or_add(d,[&](auto f){return f==d;});
    }
    template<typename EqComp>
    void remove(const T& d,EqComp comp){
        auto iter = std::find_if(std::begin(queue),std::end(queue),comp);
        if(iter != std::end(queue)){
            auto distance = std::distance(std::begin(queue),iter);
            std::swap(queue[distance],queue[queue.size()-1]);
            queue.pop_back();
            if(queue[distance]>queue[distance/2]){
                swim(distance);
                return;
            }
            sink(distance);

        }
    }
    void remove(const T& d){
        remove(d,[&](auto f){return f==d;});
    }
    auto begin(){return queue.begin()+1;}
    auto end(){return queue.end();}
    auto size(){return queue.size()-1;}
    bool empty(){return size()<=0;}
    void print(){
        int level = std::log2(size())+2;
        auto center_aligned = [](int v,auto str){
            v-=str.size();

            int half=v/2;
            while(half-->0){
                std::cout<<" ";
            }
            std::cout<<str;
            half=v/2;
            while(half-->0){
                std::cout<<" ";
            }
        };

        int nodelevel=level;
        int node=1;
        while(level>=0){

            for(int children=0; children<std::pow(2,nodelevel-level) && node<queue.size();children++){
                center_aligned(std::pow(2,level)*2,std::to_string(queue[node++]));
            }
            std::cout<<"\n\n";
            level--;
        }
        std::cout<<"\n\n";

    }
};
}

#endif // PRIORITYQUEUE_HPP
