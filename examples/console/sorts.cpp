#include "quick_sort.hpp"
#include "merge_sort.hpp"
#include <vector>
#include <iostream>
#include <utility>
void testqsort(){
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
}
void testmergesort(){
    using namespace GGL;
    using namespace std;
    std::vector<int> v{1 , 4, 5, 6 ,3, 9, 5,3};
    shuffle(v);
    auto print = [&](){
         for(auto i:v){
            cout <<i<<" ";
        }
        cout <<"\n";
    };
    auto swapper=[&](auto& first,auto& second){
        print();


    };
    merge_sort(v,std::less<int>{},swapper);
    print();
}
int  main()
{
//    testqsort();
    testmergesort();
    return 0;
}
