#pragma once
#include <vector>
#include <memory>
#include <iostream>
namespace GGL
{

    inline void merge(auto b,auto e, auto mid,auto& r, auto& aux,auto comp, auto tracer){

        for(int i=b;i<=e;i++){
            aux[i]=r[i];
        }


        auto dis= e-b;
        int i=0;
        auto left=b;
        auto right =mid+1;
        while(i++<=dis){
            if(left>mid) r[b++]=aux[right++];
            else  if (right > e) r[b++]=aux[left++];
            else if(comp(aux[left],aux[right])) r[b++]=aux[left++];
            else r[b++]=aux[right++];
        }

    }
    inline void merge_sort_impl(auto b, auto e, auto& r, auto& aux,auto comp, auto tracer){
        if(e <= b) return;
        auto mid = b+(e-b)/2;

        merge_sort_impl(b,mid,r,aux,comp,tracer);
        merge_sort_impl(mid+1,e,r,aux,comp,tracer);

        if(!comp(r[mid],r[mid+1])){
            merge(b,e,mid,r,aux,comp,tracer);
        }
        tracer(r[b],r[e]);
    }
    inline void merge_sort(auto& range, auto comp, auto tracer){
        using value_type = typename std::remove_reference_t<decltype(range)>::value_type;
        auto aux = std::vector<value_type>(range.size());
        merge_sort_impl(0,range.size()-1,range,aux,comp,tracer);
    }
}
