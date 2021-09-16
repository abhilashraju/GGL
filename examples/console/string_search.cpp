#include "string_algorithm.hpp"
#include <iostream>
#include <string_view>
int  main()
{
    using namespace GGL;
    using namespace std;
    string_view s("mynameisabhiabhilashr");
    cout << s.substr(sub_str(s, "abhiadbhilash"));
    
    
    // std::string pattern="ababhilash";
    // auto dfa = make_automata(pattern);
   
    // for(int i=0;i<26;i++){
    //     cout<<char('a' +i)<<" ";
    //     for(int j=0; j<pattern.size();j++){
    //          cout<<dfa[i][j]<<" ";
    //     }
    //     cout<<"\n";
    // }
    return 0;
}