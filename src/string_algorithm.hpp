#pragma once
#include <memory>
#include <string>
namespace GGL
{
    struct DFA{
        static constexpr int char_limit=26;
        DFA(int states):states(states){
            dfa.reset(new int[ char_limit* states]);
            std::fill(dfa.get(),dfa.get()+char_limit*states,0);
        }
        struct ROW{
            int* r{nullptr};
            int& operator[](int index){
                return *(r+index);
            }
        };
        ROW operator[](int index){
            return ROW{dfa.get()+index*states};
        }
        auto print(auto& os){
            for(int i=0;i<char_limit;i++){
                os<<char('a' +i)<<" ";
                for(int j=0; j<states;j++){
                    os<<(*this)[i][j]<<" ";
                }
                os<<"\n";
            }
        }
        std::unique_ptr<int> dfa; 
        int states{0};
    };
    inline auto make_automata( std::string_view str){
        DFA dfa(str.size());
        int currentx=0;
        for(int i=0 ;i <str.size();i++){
            
            for(int j=0;j<DFA::char_limit;j++){
                 dfa[j][i]=dfa[j][currentx];
            }
            currentx=dfa[str[i]-'a'][currentx];
            dfa[str[i]-'a'][i]=i+1;   
        }
        return std::move(dfa);
    }
    inline auto sub_str(std::string_view str, std::string_view substr){
        auto dfa = make_automata(substr);
        
        int state=0;
        for(int i =0;i<str.size();i++){
            state=dfa[str[i]-'a'][state];
            if(state == substr.size()){
                return i-substr.size()+1;
            }
        }
        return str.size();
    }
   
}