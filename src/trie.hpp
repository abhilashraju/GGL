#ifndef TRIE_HPP
#define TRIE_HPP
#include <utility>
#include <vector>
#include <algorithm>
#include <memory>
#include <string>
#include <deque>
#include <iostream>
#include <numeric>
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
template<typename Key=std::string,typename Value=std::string>
struct Trie
{
    struct TrieNode;
    using NodePtr=std::unique_ptr<TrieNode>;
    using NodeKey=typename Key::value_type;
    template <typename k, typename v>
    using Flatmap=Flatmap<k,v>;
    struct TrieNode{
        Flatmap<NodeKey,NodePtr> children;
        Value value;
        NodeKey key;
        bool contains(const NodeKey& k){
            return children.contains(k);
        }
        NodePtr& operator[](const NodeKey& k){
            return children[k];
        }
    };
    void insert(const Key& key, const Value& value){
        Key currentkey =key;
        auto currentNode=root.get();
        while(!currentkey.empty()){
            NodeKey k = currentkey[0];
            if(currentNode->contains(k)){
                currentNode=((*currentNode)[k]).get();
            }
            else{
              ((*currentNode)[k])=std::make_unique<TrieNode>(TrieNode{{},Value{},k});
              currentNode=((*currentNode)[k]).get();
            }
            currentkey=currentkey.substr(1);
            if(currentkey.empty()){
                    currentNode->value=value;
            }
        }
    }
    void print(){

        std::deque<TrieNode*> queue{root.get()};
        while(queue.size()){
            auto current = queue.front();
            for(auto& c:current->children){
                std::cout<<"   "<<c.second->key<<"   ";
                queue.push_back(c.second.get());
            }
            std::cout<<"\n";
            queue.pop_front();
        }
    }
     std::vector<Key> get_path_list(TrieNode* node){
        std::vector<Key> keys;
        if(node){
            auto key = node->key;
            if(node->children.size()){
                std::accumulate(begin(node->children),end(node->children),std::back_inserter(keys),[&](auto& iter, auto& v){
                   auto c= v.second.get();
                   auto clist =get_path_list(c);

                   std::vector<Key> ckeys;
                   if(isValid(node->value)){
                       ckeys.emplace_back(Key(1,key));
                   }
                   std::transform(begin(clist),end(clist),std::back_inserter(ckeys),[&](auto& cv){
                       return key+cv;
                   });
                   for(auto& ck:ckeys){
                       *iter=ck;
                   }
                   return iter;
                });
            }else{

                keys.push_back(Key(1,key));
            }

        }
        return keys;
    }
    std::vector<Key> get_starts_with(const Key& prefix){
        std::vector<Key> keys;
        auto node=findNode(prefix);
        if(node){
            if(isValid(node->value)){
                keys.push_back(prefix);
            }
             for(auto& suff:get_suffix_list(node)){
                keys.push_back(prefix+suff);
             }
        }
        return keys;
    }
    std::vector<Key> get_suffix_list(TrieNode* node){
        std::vector<Key> keys;
        if(node){

            for(auto& c: node->children){
                for(auto& suff:get_path_list(c.second.get())){
                    keys.push_back(suff);
                }
            }
        }
        return keys;
    }
    std::vector<Key> get_suffix_list(const Key& prefix){
        std::vector<Key> keys;
        for(auto& suff:get_suffix_list(findNode(prefix))){
            keys.push_back(suff);
        }
        return keys;
    }
    TrieNode* findNode(const Key& prefix){
        auto currentNode=root.get();
        Key currentKey=prefix;
        while(currentNode && !currentKey.empty()){
            if(currentNode->contains(currentKey[0])){
                currentNode=(*currentNode)[currentKey[0]].get();
                currentKey=currentKey.substr(1);
            }else{
                return nullptr;
            }
        }
        return currentNode;

    }
    Value find(const Key& key){
        auto currentNode=root.get();
        Key currentKey=key;
        while(currentNode && !currentKey.empty()){
            if(currentNode->contains(currentKey[0])){
                currentNode=(*currentNode)[currentKey[0]].get();
                currentKey=currentKey.substr(1);
            }else{
                return Value{};
            }
        }
        if(currentNode){
            return currentNode->value;
        }
        return Value{};
    }
    NodePtr root{std::make_unique<TrieNode>(TrieNode{{},Value{},' '})};
};
#endif // TRIE_HPP
