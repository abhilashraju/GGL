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
#include "graphalgorithms.hpp"
#include "graph.hpp"
#include "rasterwindow.h"
#include <random>
#include <cmath>
using namespace GGL;
class MstGraph : public RasterWindow
{
    constexpr static int noofcols=10;
    constexpr static int noofrows=10;
public:
    MstGraph();

protected:
    void timerEvent(QTimerEvent *) override;
    void render(QPainter *p) override;

private:
    void intialiseGraph(auto e1);
    EdgeWeightedGraph<int> graph{1};
    std::deque<EdgeWeightedGraph<int>::Edge> krukTree;
    std::deque<EdgeWeightedGraph<int>::Edge>::iterator krukIter;

    std::deque<EdgeWeightedGraph<int>::Edge> primTree;
    std::deque<EdgeWeightedGraph<int>::Edge>::iterator primIter;
    std::map<int,int> noice;
    std::random_device r;
    int m_timerId;
};
void MstGraph::intialiseGraph(auto e1)
{
    noice.clear();
    auto row_col=[=,cellw=width()/2*noofcols,cellh=height()/2*noofrows](auto v,auto n){
        int ypos=(v/noofcols)*cellh+n;
        int xpos=(v%noofcols)*cellw+n;
        return std::pair<int,int>{xpos,ypos};
    };
    graph =EdgeWeightedGraph<int>{100};
    krukTree.clear();
    primTree.clear();
    constexpr int bottom=noofcols;
    constexpr int top=-noofcols;
    constexpr int left=-1;
    constexpr int right =1;
    constexpr std::array<int,8> dir={left,top-1,top,top+1,right,bottom+1,bottom,bottom-1};
    constexpr std::array<int,5> ledir={top,top+1,right,bottom+1,bottom};
    constexpr std::array<int,5> redir={top,top-1,left,bottom-1,bottom};
    std::uniform_int_distribution<int> random_d(0,7);
    std::uniform_int_distribution<int> random_v(1,99);
    std::uniform_int_distribution<int> random_de(0,4);
    std::uniform_int_distribution<int> random_noice(0,width()/(2*noofcols));
    auto selectsecond=[&](auto v){
        if(v%noofcols==9){
            v=v+redir[random_de(e1)];
        }else if(v%noofcols==0){
             v=v+ledir[random_de(e1)];
        }else{
            v=v+dir[random_d(e1)];
        }
        return v;
    };
    for(size_t i=1; i<graph.size();i++){


        auto v1=EdgeWeightedGraph<int>::Edge::VertexType(i);
        if(noice.find(v1)==noice.end())
            noice[v1]=random_noice(e1);
        for(int j=0;j<3;j++){
            auto  v2=selectsecond(v1);
            while(v2<1 || v2 >99){
                v2=selectsecond(v1);
            }
            if(noice.find(v2)==noice.end())
                noice[v2]=random_noice(e1);
            auto cell1=row_col(v1,noice[v1]);
            auto cell2=row_col(v2,noice[v2]);
            int wt=std::sqrt(std::pow(cell1.first-cell2.first,2) +std::pow(cell1.second-cell2.second,2));
            graph.addEdge({v1,v2,wt});
        }

    }

    m_s_t(m_s_t_kru{},graph,[&](auto& e){
        krukTree.push_back(e);

    });
    krukIter=krukTree.begin();
    m_s_t(m_s_t_pri{},graph,[&](auto& e){
        primTree.push_back(e);
    });
    primIter=primTree.begin();
}
MstGraph::MstGraph()
{
    setTitle("MST");
    resize(700, 700);





    intialiseGraph(std::default_random_engine{r()});
    m_timerId = startTimer(50);

}
void MstGraph::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId){
        renderLater();
        if(krukIter == krukTree.end()){
            intialiseGraph(std::default_random_engine{r()});
        }
    }
}


void MstGraph::render(QPainter *p)
{

    auto drawText=[&](auto rect, auto text){
        p->setFont(QFont("Times New Roman",20));
        p->drawText(rect,text,QTextOption(Qt::AlignCenter));
    };
    auto row_col=[=,cellw=width()/(2*noofcols),cellh=height()/(2*noofrows)](auto v,auto n){

        int ypos=(v/noofcols)*cellh+n;
        int xpos=(v%noofcols)*cellw+n;
        return QPoint{xpos,ypos};
    };
    QColor vertexcolor(127, 0, 127);
    QColor edgecoulor(127, 127, 127, 50);


    auto draw_vertex=[&](auto v,auto color){
        auto pt1=row_col(v,noice[v]);
        p->setPen(color);
        p->setBrush(color);
        p->drawEllipse(pt1,5,5);
//            p->drawText(pt1,QString("%1").arg(v1));
        return  pt1;
    };
    auto draw_line=[&](auto pt1,auto pt2,auto color){
        p->setPen(QPen(color,3));
        p->setBrush(color);
        p->drawLine(pt1,pt2);
    };

    auto drawMst=[&](auto& current,auto& tree,const auto& pen){
        for(auto& e:graph.edges()){
            const auto& v1=e.either();
            const auto& v2=e.other(e.either());
            draw_line(draw_vertex(v1,vertexcolor),draw_vertex(v2,vertexcolor),edgecoulor);
        }
        for(auto iter =tree.begin();iter!=current;iter++) {
            auto& e=*iter;
            const auto& v1=e.either();
            const auto& v2=e.other(e.either());
            p->setPen(pen);
            draw_line(draw_vertex(v1,vertexcolor),draw_vertex(v2,vertexcolor),pen);

        }
        if(tree.end()!= current){
            current++;
        }

    };

    drawMst(primIter,primTree, Qt::red);
    p->save();

    p->translate(width()/2,height()/4);
    drawText(QRect(0,0,width()/2,50),"Primes MST");
    p->restore();
    p->save();


    p->translate(0,height()*3/4);
    drawText(QRect(0,0,width()/2,50),"Krusk MST");
    p->restore();
    p->translate(width()/2,height()/2);
    drawMst(krukIter,krukTree, Qt::green);



}

