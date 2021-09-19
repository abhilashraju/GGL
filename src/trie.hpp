#ifndef TRIE_HPP
#define TRIE_HPP
/*
MIT License

Copyright (c) 2021 abhilashraju

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <deque>
#include <iostream>
#include <numeric>

namespace GGL {
template <typename Key, typename Value>
struct Flatmap
{
    using value_type=std::pair<Key,Value>;
    std::vector<value_type> values;
    void insert(const Key& key, const Value& value){
        values.emblace_back(value_type{key,value});
    }
    auto find(const Key& key){
        auto iter = std::find_if(std::begin(values),std::end(values),[&](auto& v){
            return v.first == key;
        });
        return iter;
    }
    bool contains(const Key& key){
        return find(key)!=std::end(values);
    }
    Value& operator[](const Key& key){
        auto iter =find(key);
        if(iter != std::end(values)){
            return iter->second;
        }
        values.push_back(value_type{key,Value{}});
        return values.back().second;
    }
    auto begin(){return values.begin();}
    auto end(){return values.end();}
    auto begin()const{return values.begin();}
    auto end()const{return values.end();}
    auto size()const {return values.size();}
};
bool isValid(const std::string& v){
    return !v.empty();
}
bool isValid(bool v){
    return v;
}

template<typename Node,typename Container>
  struct node_maker{
        std::unique_ptr<Node> operator()(char c)const{
                auto n= std::make_unique<Node>();
                n->c=c;
                return n;
        }
  };
  
  template<typename Key,typename Value, template <typename T> class Container>
  struct TrieImpl{
     
      using link_type=typename Key::value_type;
      struct Node;
      using container_type =Container<std::unique_ptr<Node>>;
      static constexpr link_type start='a';
      using value_type = Value;
      struct Node{
            container_type links;
            value_type value{};
            link_type c{'*'};
            Node(const value_type& v=value_type{}):value(v){ links.initialize();}
      };
      std::unique_ptr<Node> root{node_maker<Node,container_type>()(' ')};
      std::unique_ptr<Node> put(auto& node, const auto& key, const auto& value ,auto current){
          if(current == key.size()){
            node->value=value;
            return std::move(node);
          }
          auto& l=node->links[key[current]];
          if(!l){
              l=node_maker<Node,container_type>()(key[current]);
          }
          
         
         l=put(l,key,value,current+1);
         return std::move(node);
      }
      void insert(const Key& key,const value_type& value){
         root=put(root,key, value,0);
      }
      Node* get(Node* node,const Key& key,auto current){
          if(!node) return nullptr;
          if(current == key.size()){
              return node;
          }
         
          return get(node->links[key[current]].get(),key,current+1);
      }
      value_type get(const Key& key){
          auto node=get(root.get(),key,0);
          if(node){
             return node->value;
          }
           return value_type{};
      }
      auto keys(){
          std::vector<Key> keys;
          keys_impl(root.get(),keys,"");
          return keys;
      }
      void keys_impl(Node* node, std::vector<Key>& keys,const std::string& current){
          if(!node) return;
          if(isValid(node->value)) keys.push_back(current);
          
          for(auto& v:node->links){
              keys_impl(v.get(),keys,current+(v?v->c:' '));
          }
      }
      auto suffixes(auto& prefix){
          auto node=get(root.get(),prefix,0);
          std::vector<Key> keys;
          keys_impl(node,keys,prefix);
          return keys;
      }
      auto levelarray(){
          std::vector<std::vector<link_type>> array;
          levelarray(root,array,0);
          return array;
      }
      auto levelarray(auto& node, auto& arr,auto index){
            if(!node) return;
            if(arr.size()<=index) arr.push_back(std::vector<link_type>());
             arr[index].push_back(node->c);
            for(auto& v:node->links){  
                levelarray(v,arr,index+1);
            }
      }
       auto print(auto& os){
          auto arr= levelarray();
          for(auto& v:arr){
              for(auto& c:v){
                  os<<c<<" ";
              }
              os<<"\n";
          }
        }
      

  };
   template<typename T>
   struct TernaryContainer{
       std::array<T,3> container;
       void initialize(){}
       T& operator[](char c){
          
          if(!container[1] || container[1]->c==c ) {
              return container[1];
          }
            
          if(c<container[1]->c){
                if(container[0] && container[0]->c !=c){
                    return container[0]->links[c];
                }
                return container[0];
            
          }
          if(container[2] && container[2]->c !=c){
              return container[2]->links[c];
          }
          return container[2];
         
          
       }
       auto begin(){
           return container.begin();
       }
       auto end(){
           return container.end();
       }
       
   };
  
   template<typename T>
   struct ArrayContainer:std::array<T,26>{
       using Base=std::array<T,26>;
       void initialize(){}
       T& operator[](char c){
          return Base::operator[](c-'a');
       }
       
   };

 template<typename Key,typename Value>
 using Trie=TrieImpl<Key,Value, ArrayContainer>;
 template<typename Key,typename Value>
 using TernaryTrie=TrieImpl<Key,Value, TernaryContainer>;

}

#endif // TRIE_HPP
