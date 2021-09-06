#include <map>
#include <numeric>
#include <limits>
#include <string>
#include <memory>
#include <vector>
#include <math.h>
#include <iostream>
template<typename T, int COUNT=2>
struct Tree
{
  using value_type=T;
  struct Node
  {
      T val{};
      std::array<std::unique_ptr<Node>,COUNT> children;
  };
  std::unique_ptr<Node> root{new Node};
  int m_size{1};
  int size()const{
      return m_size;
  }
  int nodeHeight(Node* r)const{
      int h=0;
      if(r){
         h=1;
         int max=0;
         for(auto& n:r->children){
             int ch=nodeHeight(n.get());
             if(max<ch)max=ch;
         }
         h+=max;
      }
      return  h;
  }
  int height()const{
      return nodeHeight(root.get());
  }
  Node* getChild(int index,Node* p){
      if(index <COUNT && p){
        return p->children[index].get();
      }
      return nullptr;
  }
  bool addChild(int index,Node* c,Node* p){
      if(index <COUNT){
        if(!p){
            if(!root->children[index]) m_size++;
            root->children[index].reset(c);
            return true;
        }
        if(!p->children[index]) m_size++;
        p->children[index].reset(c);
        return true;
      }
      return false;
  }
};
template<typename T>
struct BinTree:Tree<T,2>
{
    using Node =typename Tree<T,2>::Node;
    using Base =Tree<T,2>;
    auto addLeft(Node* c,Node* p=nullptr){
        return Base::addChild(0,c,p);
    }
    auto left(Node* c){
        return Base::getChild(0,c);
    }
    auto addRight(Node* c,Node* p=nullptr){
        return Base::addChild(1,c,p);
    }
    auto right(Node* c){
        return Base::getChild(1,c);
    }
    Node* makeLeft(const T& v,Node* p=nullptr){
        Node* newNode = new Node{v};
        addLeft(newNode,p);
        return newNode;
    }
    Node* makeRight(const T& v,Node* p=nullptr){
        Node* newNode = new Node{v};
        addRight(newNode,p);
        return newNode;
    }
    void b_f_s(Node* node,std::vector<std::vector<Node*>>& levelt,int level){
            if(left(node)){
                levelt[level].push_back(left(node));
                b_f_s(left(node),levelt,level+1);
            }
            if(right(node)){
                levelt[level].push_back(right(node));
                b_f_s(right(node),levelt,level+1);
            }
    }
    void print(){
        int level = std::log2(Base::height())+1;
        auto center_aligned = [](int v,auto str){
            v-=str.size();

            int half=v/2;
            while(half-->0){
                std::cout<<" ";
            }
            std::cout<<str;
            half=v/2;
            while(half-->0){
                std::cout<<" ";
            }
        };



        int ht=Base::height()+1;
        std::vector<std::vector<Node*>> leveltree(ht);
        leveltree[0].push_back(Base::root.get());
        b_f_s(Base::root.get(),leveltree,1);
        int height=leveltree.size();
        for(auto& l:leveltree){
            for(auto v:l){
                center_aligned(std::pow(2,height)*2,std::to_string(v->val));
            }
            std::cout<<"\n\n";
            height--;
        }
        std::cout<<"\n\n";

    }
    void mirrorNode(auto& n){
        auto temp =std::move(n->children[0]);
        n->children[0]=std::move(n->children[1]);
        n->children[1]=std::move(temp);
        if(left(n.get())){
           mirrorNode(n->children[0]);
        }
        if(right(n.get())){
           mirrorNode(n->children[1]);
        }
    }
    void mirror()
    {
        mirrorNode(Base::root);
    }
};
template<typename T>
struct Unknown;
int main (){

    BinTree<int> tree;
    auto l=tree.makeLeft(1);
    auto r=tree.makeRight(2);
    auto l1l=tree.makeLeft(3,l);
    auto l1r=tree.makeRight(4,l);
    auto r1l=tree.makeLeft(5,r);
    auto r1r=tree.makeRight(6,r);

    tree.print();
    tree.mirror();
    tree.print();
    return 0;

}
