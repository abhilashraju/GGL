#include "graphalgorithms.hpp"
#include "graph.hpp"
#include "rasterwindow.h"
#include <random>
#include <cmath>

class MetrixGraphsUI : public RasterWindow
{
    constexpr static int noofcols=25;
    constexpr static int noofrows=25;
public:
    MetrixGraphsUI();

protected:
    void timerEvent(QTimerEvent *) override;
    void render(QPainter *p) override;
    void mousePressEvent(QMouseEvent *);
private:
    void intialiseGraph(auto e1);
    using GridMatrix=GridMatrix<QColor>;
    QColor curselected{Qt::red};
    GridMatrix graph;
    std::deque<GridMatrix::VertexType> bfsQueue;
    std::deque<GridMatrix::VertexType>::iterator bfsIter;
    QRect refreshRect,dfsRect,bfsRect;
    bool mDfsAlg{true};
    std::random_device r;
    int m_timerId;
    QImage bgimage;
};
void MetrixGraphsUI::intialiseGraph(auto e1)
{
    using  VertexType=GridMatrix::VertexType;
    std::array<QColor,2> colorarray{Qt::red,Qt::green};
    std::uniform_int_distribution<int> random_col(0,colorarray.size()-1);
    std::vector<QColor> random_data;
    std::generate_n(std::back_inserter(random_data),noofcols*noofrows,[&](){return colorarray[random_col(e1)]; });
    graph =GridMatrix(GridMatrix::Buffer(random_data.data(),random_data.size()));
    graph.setPredicate([&](auto v){return v==curselected;});
    bfsQueue.clear();
    bfsIter=bfsQueue.begin();
    refreshRect=QRect(width()/2,50,width()/2,50);
    dfsRect=QRect(width()/2,110,width()/4,50);
    bfsRect=QRect(width()*3/4,110,width()/4,50);
    bgimage=QImage("C:/Users/rabhil/work/Algorithms/test1/deep.jpg").scaled(width()/2,height()/2,Qt::KeepAspectRatio);
}
MetrixGraphsUI::MetrixGraphsUI()
{
    setTitle("Graph");
    resize(700, 700);
    intialiseGraph(std::default_random_engine{r()});
    m_timerId = startTimer(50);
    bfsIter=bfsQueue.begin();

}
void MetrixGraphsUI::mousePressEvent(QMouseEvent *evt)
{
    if(refreshRect.contains(evt->pos())){
        intialiseGraph(std::default_random_engine{r()});
        return;
    }
    if(dfsRect.contains(evt->pos())){
        mDfsAlg=true;
        return;
    }
    if(bfsRect.contains(evt->pos())){
        mDfsAlg=false;
        return;
    }
    bfsQueue.clear();
    auto cellw = width()/(2*noofcols);
    auto cellh= height()/(2*noofrows);
    auto initx=cellw/2, inity=cellh/2;
    QPoint p(evt->x()-initx,evt->y()-inity);
    auto c=((p.x()*2/cellw)+1)/2;
    auto r=((p.y()*2/cellh)+1)/2;
    curselected = graph.value(GridMatrix::VertexType{r,c});
    auto run=[&](auto alg){
        travers(graph,GridMatrix::VertexType{r,c},alg,[&](auto p,auto c){

        },[&](auto c){
            bfsQueue.push_back(c);
        });
        bfsIter=bfsQueue.begin();
        return;
    };
    mDfsAlg ? run(GDFS{}):run(GBFS{});


}
void MetrixGraphsUI::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId){
        renderLater();

    }
}


void MetrixGraphsUI::render(QPainter *p)
{

    auto cellw = width()/(2*noofcols);
    auto cellh= height()/(2*noofrows);
    auto drawButton= [&](auto rect, auto text){

        p->setBrush(QBrush(Qt::white));
        p->drawRect(rect);
        p->drawText(rect,text,QTextOption(Qt::AlignCenter));
    };
    drawButton(refreshRect,"Refresh");
    drawButton(dfsRect,"DFS");
    drawButton(bfsRect,"BFS");


    auto drawGraph=[&](auto& q,auto& end)mutable{
        auto initx=cellw/2, inity=cellh/2;
        auto iter= q.begin();
        while(iter != end){
//            graph.setValue(*iter,graph.value(*iter).setAlpha(0));iter++;
            graph.value(*iter).setAlpha(0);iter++;
        }
        if(end != q.end()) end++;
        graph.for_each([&](int r, int c,auto& v){
            p->setPen(graph.value(GridMatrix::VertexType{r,c}));
            p->setBrush(graph.value(GridMatrix::VertexType{r,c}));


            p->drawEllipse(initx+(2*c-1)*cellw/2,inity+(2*r-1)*cellh/2,cellw,cellh);

        });
    };
    p->drawImage(QRect(0,0,width()/2,height()/2),bgimage);
    drawGraph(bfsQueue,bfsIter);

}

