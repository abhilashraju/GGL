#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <deque>
namespace GGL{
    template<typename Tree ,size_t fixeddatasize=5>
    struct Tree_Utilities{
       using Node = typename Tree::Node;
       using value_type=typename Tree::value_type;
       using key_type = typename Tree::key_type;
        static void print(Tree& tree){
            int level = tree.height();


            auto center_aligned = [&](int v,std::string_view str){

                int padding =0;
                std::string newstring;
                if(str.size()>fixeddatasize){
                    newstring = str.substr(0,fixeddatasize);
                }else{
                   padding =  fixeddatasize-str.length();
                   newstring=str;
                   while(padding>0){
                       newstring.append("x");padding--;
                   }
                }
                v-=newstring.size();

                int half=v/2;
                while(half-->0){
                    std::cout<<" ";
                }
                std::cout<<newstring;
                half=v/2;
                while(half-->0){
                    std::cout<<" ";
                }
            };

            int nodelevel=level;
            auto levels_array= make_level_arrays(tree.root.get());
            while(level>=0){
                 
                for(int children=0; children<std::pow(2,nodelevel-level) ;children++){
                    center_aligned(std::pow(2,level+3)*2,std::to_string(levels_array[nodelevel-level][children].second));
                }
                std::cout<<"\n\n";
                level--;
            }
            std::cout<<"\n\n";

        }
       
        static std::vector<std::vector<std::pair<key_type,value_type>>> make_level_arrays(Node* root){
            std::vector<std::vector<std::pair<key_type,value_type>>> levelarray;
            int max_level=6;
            int level=0;
             
            while(level<max_level){
                int children=0;
                levelarray.push_back({});
                while(children<std::pow(2,level)){
                    levelarray[level].emplace_back(key_type{},value_type{});
                    children++;
                }
                level++;
            }
           
            std::deque<std::pair<Node*,int>> q;
            q.emplace_back(root,1);
            while(!q.empty()){
                auto n=q.front(); q.pop_front();
                int level = std::log2(n.second);
                int index=0;
                if(level > 0){
                    index = n.second-std::pow(2,level);
                }
                
                if(n.first){
                    levelarray[level][index]=std::make_pair(n.first->key,n.first->value);
                    q.emplace_back(n.first->left.get(),n.second*2);
                    q.emplace_back( n.first->right.get(),n.second*2+1);    
                }else if(level <max_level){
                    q.emplace_back(nullptr,n.second*2);
                    q.emplace_back( nullptr,n.second*2+1);
                }
                
            }
             return levelarray;

        }
    };
}
