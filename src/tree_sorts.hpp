#pragma once
#include <memory>
#include <algorithm>
#include <memory>

namespace GGL{
    
    template <typename Key , typename Value>
    struct BST
    {
        using value_type =Value;
        using key_type = Key;
        constexpr static bool RED=true;
        constexpr static bool BLACK=false;
        struct Node{
            value_type value;
            key_type key;
            int count{1};
            std::unique_ptr<Node> left{};
            std::unique_ptr<Node> right{};
            bool color{RED};
            Node(const key_type& k, const value_type& v):key(k),value(v){}
            friend auto& operator<<(auto& os, const Node& n){
                os<<"{ "<<n.key<<" , "<<n.value<<"} ";
                return os;
            }
        };
        std::unique_ptr<Node> root;
         
        size_t height(){
            return height(root.get());
        }
        size_t height(Node* n){
            if(n){
                return 1+std::max(height(n->left.get()),height(n->right.get()));
            }
            return 0;
        }
        //printing end
        void insert(const key_type&  k, const value_type& v){
            root=put(root,k,v);
        }
        auto size(auto& node)const{
            if(node) 
                return node->count;
            return 0;
        }
        bool isRed(auto& node){
            if(node){
                return node->color==RED;
            }
            return false;
        }
        std::unique_ptr<Node> put(auto& node,const key_type& k, const value_type& v){
            if(!node){
                return std::make_unique<Node>(k,v);
            }
            if(k < node->key ){
                node->left=std::move(put(node->left,k,v));
            }else if( k > node->key ){
                node->right=std::move(put(node->right,k,v));
            }else{
                node->value=v;
            }
            if(!isRed(node->left) && isRed(node->right)) node=std::move(rotateleft(node));
            if(isRed(node->left) && isRed(node->left->left)) node=std::move(rotateright(node));
            if(isRed(node->left) && isRed(node->right)) node=std::move(invertcolor(node));
            node->count = 1+size(node->left)+size(node->right);
            return std::move(node);
        }
        Node* find(const auto& node,const key_type& k)const{
            if(!node){
                return nullptr;
            }
            if( k < node->key ){
                return find(node->left,k);
            }else if( k >node->key ){
                return find(node->right,k);
            }
            return node.get();
            
        }
        bool contains(const key_type& k)const {
            return find(root,k) != nullptr;
        }
        value_type get(const key_type& k)const {
            auto n = find(root,k);
            if(n){
                return n->value;
            }
            return value_type{};
        }
        int rank(const key_type& k){
            return rank(root,k);
        }
        int rank(auto& node, const key_type& k){
            if(!node){
                return std::numeric_limits<int>::min();
            }
            if(k < node->key  ){
                return rank(node->left,k);
            }
            if(k >node->key ){
                return 1+size(node->left)+rank(node->right,k);
            }
            return 1+size(node->left);
        }
        struct iterator{
            BST* tree{nullptr};
            Node* node{nullptr};
            iterator(BST* t,Node* n):tree(t),node(n){}
            iterator& operator++(){
                node=tree->successor(node);
                return *this;
            }
            iterator operator++(int){
                iterator iter(tree,node);
                ++(*this);
                return iter;
            }
            iterator& operator--(){
                node=tree->predecessor(node);
                return *this;
            }
            iterator operator--(int){
                iterator iter(tree,node);
                --(*this);
                return iter;
            }
            friend bool operator==(const iterator& iter1, const iterator& iter2){
                return iter1.tree == iter2.tree && iter1.node == iter2.node;
            }
            friend bool operator!=(const iterator& iter1, const iterator& iter2){
                return !(iter1== iter2);
            }
            Node* operator->(){return node;}
            Node& operator*(){return *node;}
        };
        iterator begin(){return iterator(this,minimumnode(root.get()));}
        iterator end(){return iterator(this,nullptr);}
        iterator rbegin(){return iterator(this,maximumnode(root.get()));}
        iterator rend(){return iterator(this,nullptr);}
        auto minimum(){
            auto n=minimumnode(root.get());
            if(n){
                return std::make_pair(n->key,n->value);
            }
            return std::make_pair(key_type{},value_type{});
        }
        Node* minimumnode(Node* node){
            if(!node) nullptr;
            if(node->left){
                return minimumnode(node->left.get());
            }
            return node;
        }
        auto maximum(){
            auto n=maximumnode(root.get());
            if(n){
                return std::make_pair(n->key,n->value);
            }
            return std::make_pair(key_type{},value_type{});
        }
        Node* maximumnode(Node* node){
            if(!node) nullptr;
            if(node->right){
                return maximumnode(node->right.get());
            }
            return node;
        }
        auto successor(Node* node){
            if(node->right){
               return minimumnode(node->right.get());
            }
            return successor(node->key);
        }
        auto successor(const key_type& key){
            return successor(root.get(),key);
        }
        Node* successor(Node* node,const key_type& key){
            if(!node) return nullptr;
            if(node->key <= key){
               return successor(node->right.get(),key);
            }
            auto n= successor(node->left.get(),key);
            if(n) 
                return n;
            return node;
        }
         
        auto predecessor(const key_type& key){
            return predecessor(root.get(),key);
        }
        auto predecessor(Node* node){
            if(node->left){
               return maximumnode(node->left.get());
            }
            return predecessor(node->key);
        }
        Node* predecessor(Node* node,const key_type& key){
            if(!node) return nullptr;
            if(node->key >= key){
               return predecessor(node->left.get(),key);
            }
            auto n= predecessor(node->right.get(),key);
            if(n) 
                return n;
            return node;
        }
        auto ceil(const key_type& key){
               
            auto n= ceilnode(root.get(),key);
            if(n)
                return std::make_pair(n->key,n->value);
            return std::make_pair(key_type{},value_type{});
        }
        Node* ceilnode(Node* node, const key_type& key){
            if(node == nullptr) 
                return nullptr;
            if(node->key == key) 
                return node;
            if(node->key < key){
                return ceilnode(node->right.get(),key);
            }
            auto n=ceilnode(node->left.get(),key);
            if(n) 
                return n;
            return node;
        }
        auto floor(const key_type& key){
            
                
            auto n= floornode(root.get(),key);
            if(n)
                return std::make_pair(n->key,n->value);
            return std::make_pair(key_type{},value_type{});
        }
        Node* floornode(Node* node, const key_type& key){
            if(node == nullptr) 
                return nullptr;
            if(node->key == key) 
                return node;
            if(node->key > key){
                return floornode(node->left.get(),key);
            }
            auto n=floornode(node->right.get(),key);
            if(n) 
                return n;
            return node;
        }
       void visit(auto func){
            
            visitimpl(root.get(),func);
            
        }
        void visitimpl(Node* node,auto& func){
            if(!node) return;
            visitimpl(node->left.get(),func);
            func(node->key,node->value);
            visitimpl(node->right.get(),func);
        }
        void delete_min(){
            root=delete_min(root);
        }
        auto delete_min(auto& node){
            if(!node->left) 
                return std::move(node->right);
        
            node->left=delete_min(node->left);
            node->count = 1+ size(node->left)+size(node->right);
            return std::move(node);
        }
        void delete_key(const key_type& k){
            root=deleteimpl(root,k);
        }
        auto deleteimpl(auto& node, const key_type& key){
            if(key == node->key){
                if(node->left && node->right){
                    auto suc=successor(node.get());
                    node->value=suc->value;
                    node->key=suc->key;
                    node->right=delete_min(node->right);
                }
                else if(node->left){
                    return std::move(node->left);
                }
                else if(node->right){
                    return std::move(node->right);
                }
                return std::unique_ptr<Node>();
            }
            else if(key < node->key && node->left){
                node->left=deleteimpl(node->left,key);
            }
            else if(node->right){
                node->right=deleteimpl(node->right,key);
            }
            node->count=1+size(node->left)+size(node->right);
            return std::move(node);
        }

        auto rotateleft(auto& node){
            auto temp=std::move(node->right);
            std::swap(temp->color,node->color);
            node->right=std::move(temp->left);
            temp->left=std::move(node);

            return std::move(temp);
        }
        auto rotateright(auto& node){
            auto temp=std::move(node->left);
            std::swap(temp->color,node->color);
            node->left=std::move(temp->right);
            temp->right=std::move(node);
            return std::move(temp);
        }
        auto invertcolor(auto& node){
            node->left->color=BLACK;
            node->right->color=BLACK;
            node->color=RED;
            return std::move(node);
        }
    };
}

