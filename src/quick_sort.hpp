#pragma once
#include <utility>
#include <random>
namespace GGL{
    void shuffle(auto& range){
        std::random_device r;     
        auto engin=std::default_random_engine{r()};
        for(int i=1; i<range.size() ;i++){
             std::uniform_int_distribution<int> random_d(0,i);
             auto r = random_d(engin);
             std::swap(range[i],range[r]);
        }
    }
    auto partition(auto beg, auto end, auto comp,auto swapper){
        auto pivot =beg;
        auto endpoint=end;
        while (true)
        {
            while(beg < end && comp(*beg,*pivot)) beg++;
            while(beg < end && !comp(*end,*pivot)) end--;
            if(beg < end){
                swapper(*beg, *end);
            }else{break;}
        }
        swapper(*pivot,*end);
        return beg;
    }
    void qsortimpl(auto beg, auto end,auto comp,auto swapper){
        if ( beg >= end) return;
        auto partpoint = partition(beg,end ,comp,swapper);
        qsortimpl(beg , partpoint-1,comp,swapper);
        qsortimpl(partpoint+1 , end,comp,swapper);
           
    }
    void qsort(auto& range, auto comp,auto swapper){
        shuffle(range);
        qsortimpl(begin(range), end(range)-1,comp,swapper);
    }
    
}