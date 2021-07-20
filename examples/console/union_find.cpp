#include "union_find.hpp"
#include <iostream>
int main()
{
    Union_find uf(10);
    uf.add(1,2).add(0,1).add(5,6).add(4,8).add(9,0).add(6,8).add(7,3).add(7,4).add(9,6);
    auto is_connected=[](auto& uf,auto a, auto b){auto con=uf.connected(a,b)?"true\n":"false\n";std::cout<< "connected "<<a<<" , "<<b<<" "<<con ;};
    for(int i=0 ;i<10;i++){
        for(int j=i+1; j<10;j++){
            is_connected(uf,i,j);
        }
    }

    Union_find<std::string> suf({"abhilash","abhila","Pranav","Shreya"});
    suf.add("abhilash","abhila").add("Pranav","Shreya");
    is_connected(suf,"abhilash","Pranav");
    is_connected(suf,"abhilash","abhila");
    suf.add("abhilash","Pranav");
    is_connected(suf,"abhila","Pranav");
    is_connected(suf,"abhila","Shreya");

}
