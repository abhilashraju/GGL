#pragma once
#include "tree_sorts.hpp"
namespace GGL{
    template<typename T>
    struct  Interval{
        T lo{};
        T hi{};
        Interval(){}
        Interval(T l, T h):lo(l),hi(h){}
        bool intersected(const Interval& other){
            return !(other.hi<lo || other.lo > hi);
        }
        bool isValid(){
            return lo < hi;
        }
        friend auto& operator<<(auto& os ,const Interval& i){
            os<<" {"<<i.lo<<" , "<<i.hi<<"} ";
            return os;
        }
    };
    struct MetaInterval
    {
        int max_;
        static auto maximum(auto node){
            if(node){
                 return node->metaData().max_;
            }
            return std::numeric_limits<int>::min();
        }
        static auto updateData(auto node, auto left ,auto right){
            node->metaData().max_ = std::max(node->value.hi,std::max(maximum(left),maximum(right)));
        }
    };
    
    template<typename T>
    struct Interval_tree:BST_IMPL<T,Interval<T>,MetaInterval>{
        using Base = BST_IMPL<T,Interval<T>,MetaInterval>;
        using Interval =Interval<T>;
        void insert(const Interval& interval){
            Base::insert(interval.lo,interval);
        }
        void insert(const T& l,const T& h){
            insert(Interval(l,h));
        }
        Interval intersected( const Interval& query){
            return intersectedimpl(Base::root,query);
        }
        Interval intersected(const T& l, const T& h){
            return intersected(Interval(l,h));
        }
        Interval intersectedimpl(auto& node, const Interval& query){
            if(!node){
                return Interval{};
            }
            if(node->value.intersected(query)){
                return node->value;
            }
            if(node->left && MetaInterval::maximum(node->left.get()) >= query.lo){
                return intersectedimpl(node->left,query);
            }
            return intersectedimpl(node->right,query);
        }
    };
    
}
namespace std {
    template<typename T>
    inline std::string to_string(const GGL::Interval<T>& interval ){
        return " {"+ std::to_string(interval.lo) +" , "+std::to_string(interval.hi) +"} ";
    }
}
