#pragma once
#include <memory>
#include <algorithm>
#include <memory>

namespace GGL{
    
    template <typename Key , typename Value,typename Meta>
    struct BST_IMPL
    {
        using value_type =Value;
        using key_type = Key;
        constexpr static bool RED=true;
        constexpr static bool BLACK=false;
        struct Node{
            value_type value;
            key_type key;
            Meta metaData_;
            std::unique_ptr<Node> left_{};
            std::unique_ptr<Node> right_{};
            bool color{RED};
            Node(const key_type& k, const value_type& v):key(k),value(v){}
            friend auto& operator<<(auto& os, const Node& n){
                os<<"{ "<<n.key<<" , "<<n.value<<"} ";
                return os;
            }
            auto& metaData(){return metaData_;}
            Node* left(){return left_.get();}
            Node* right(){return right_.get();}
        };
        std::unique_ptr<Node> root;
        Node* getRoot(){return root.get();}
        void clear(){
            root.reset(nullptr);
        }
        size_t height(){
            return height(root.get());
        }
        size_t height(Node* n){
            if(n){
                return 1+std::max(height(n->left_.get()),height(n->right_.get()));
            }
            return 0;
        }
        
        void insert(const key_type&  k, const value_type& v){
            root=put(root,k,v);
            root->color=BLACK;
        }
        
        bool isRed(auto& node){
            if(node){
                return node->color==RED;
            }
            return false;
        }
        std::unique_ptr<Node> put(auto& node,const key_type& k, const value_type& v){
            if(!node){
                auto n= std::make_unique<Node>(k,v);
                Meta::updateData(n.get(),n->left_.get(),n->right_.get());
                return n;
            }
            if(k < node->key ){
                node->left_=std::move(put(node->left_,k,v));
            }else if( k > node->key ){
                node->right_=std::move(put(node->right_,k,v));
            }else{
                node->value=v;
            }
            if(!isRed(node->left_) && isRed(node->right_)) node=std::move(rotateleft(node));
            if(isRed(node->left_) && isRed(node->left_->left_)) node=std::move(rotateright(node));
            if(isRed(node->left_) && isRed(node->right_)) node=std::move(invertcolor(node));
            Meta::updateData(node.get(),node->left_.get(),node->right_.get());
            return std::move(node);
        }
        Node* find(const auto& node,const key_type& k)const{
            if(!node){
                return nullptr;
            }
            if( k < node->key ){
                return find(node->left_,k);
            }else if( k >node->key ){
                return find(node->right_,k);
            }
            return node.get();
            
        }
        bool contains(const key_type& k)const {
            return find(root,k) != nullptr;
        }
        value_type& operator[](const key_type& k){
            auto n =find(root ,k);
            if(n){
                return n->value;
            }
            insert(k,value_type{});
            return find(root,k)->value;
        }
        value_type get(const key_type& k)const {
            auto n = find(root,k);
            if(n){
                return n->value;
            }
            return value_type{};
        }
        
        struct iterator{
            BST_IMPL* tree{nullptr};
            Node* node{nullptr};
            iterator(BST_IMPL* t,Node* n):tree(t),node(n){}
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
            if(node->left_){
                return minimumnode(node->left_.get());
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
            if(node->right_){
                return maximumnode(node->right_.get());
            }
            return node;
        }
        auto successor(Node* node){
            if(node->right_){
               return minimumnode(node->right_.get());
            }
            return successor(node->key);
        }
        auto successor(const key_type& key){
            return successor(root.get(),key);
        }
        Node* successor(Node* node,const key_type& key){
            if(!node) return nullptr;
            if(node->key <= key){
               return successor(node->right_.get(),key);
            }
            auto n= successor(node->left_.get(),key);
            if(n) 
                return n;
            return node;
        }
         
        auto predecessor(const key_type& key){
            return predecessor(root.get(),key);
        }
        auto predecessor(Node* node){
            if(node->left_){
               return maximumnode(node->left_.get());
            }
            return predecessor(node->key);
        }
        Node* predecessor(Node* node,const key_type& key){
            if(!node) return nullptr;
            if(node->key >= key){
               return predecessor(node->left_.get(),key);
            }
            auto n= predecessor(node->right_.get(),key);
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
                return ceilnode(node->right_.get(),key);
            }
            auto n=ceilnode(node->left_.get(),key);
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
                return floornode(node->left_.get(),key);
            }
            auto n=floornode(node->right_.get(),key);
            if(n) 
                return n;
            return node;
        }
       void visit(auto func){
            
            visitimpl(root.get(),func);
            
        }
        void visitimpl(Node* node,auto& func){
            if(!node) return;
            visitimpl(node->left_.get(),func);
            func(node->key,node->value);
            visitimpl(node->right_.get(),func);
        }
        void delete_min(){
            root=delete_min(root);
        }
        auto delete_min(auto& node){
            if(!node->left_)
                return std::move(node->right_);
        
            node->left_=delete_min(node->left_);
            
            Meta::updateData(node.get(),node->left_.get(),node->right_.get());
            
            return std::move(node);
        }
        void delete_key(const key_type& k){
            root=deleteimpl(root,k);
        }
        auto deleteimpl(auto& node, const key_type& key){
            if(key == node->key){
                if(node->left_ && node->right_){
                    auto suc=successor(node.get());
                    node->value=suc->value;
                    node->key=suc->key;
                    node->right_=delete_min(node->right_);
                }
                else if(node->left_){
                    return std::move(node->left_);
                }
                else if(node->right_){
                    return std::move(node->right_);
                }
                return std::unique_ptr<Node>();
            }
            else if(key < node->key && node->left_){
                node->left_=deleteimpl(node->left_,key);
            }
            else if(node->right_){
                node->right_=deleteimpl(node->right_,key);
            }
            
            Meta::updateData(node.get(),node->left_.get(),node->right_.get());
            return std::move(node);
        }

        auto rotateleft(auto& node){
            auto temp=std::move(node->right_);
            std::swap(temp->color,node->color);
            node->right_=std::move(temp->left_);
            temp->left_=std::move(node);

            return std::move(temp);
        }
        auto rotateright(auto& node){
            auto temp=std::move(node->left_);
            std::swap(temp->color,node->color);
            node->left_=std::move(temp->right_);
            temp->right_=std::move(node);
            return std::move(temp);
        }
        auto invertcolor(auto& node){
            node->left_->color=BLACK;
            node->right_->color=BLACK;
            node->color=RED;
            return std::move(node);
        }
    };
    struct RankFinder{
        int count{1};
        static int getCount(auto node){
            if(node){
                return node->metaData().count;
            }
            return 0;
        }
        static int updateData(auto node, auto left , auto right){
            node->metaData().count = 1+getCount(left)+getCount(right);
            return node->metaData().count;
        }
        
        
        static int rank(auto node, const auto& k){
            if(!node){
                return std::numeric_limits<int>::min();
            }
            if(k < node->key  ){
                return rank(node->left_.get(),k);
            }
            if(k >node->key ){
                return 1+getCount(node->left_.get())+rank(node->right_.get(),k);
            }
            return 1+getCount(node->left_.get());
        }
    };
    template<typename Key, typename Value>
    struct BST:BST_IMPL<Key,Value,RankFinder>{
        using BASE = BST_IMPL<Key,Value,RankFinder>;
        using key_type = typename BASE::key_type;
        auto size(auto node)const{
            return RankFinder::getCount(node);
        }
        auto size()const{
            return size(BASE::root.get());
        }
        int rank(const key_type& k){
            return RankFinder::rank(BASE::root.get(),k);
        }
    };
}

