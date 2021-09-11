#include "quick_sort.hpp"
#include <vector>
#include <iostream>
#include <utility>
int  main()
{
    using namespace GGL;
    using namespace std;
    std::vector<int> v{1 , 4, 5, 6 ,3, 9, 5,3};
    auto print = [&](){
         for(auto i:v){
            cout <<i<<" ";
        }
        cout <<"\n";
    };
    auto swapper=[&](auto& first,auto& second){
        print();
        std::swap(first,second);
        
    };
    qsort(v,std::less<int>{},swapper);
    print();
    return 0;
}