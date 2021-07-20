#include "graphalgorithms.hpp"
#include "graph.hpp"
#include "rasterwindow.h"
#include <random>
#include <cmath>

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


    auto row_col=[=,cellw=width()/(2*noofcols),cellh=height()/(2*noofrows)](auto v,auto n){

        int ypos=(v/noofcols)*cellh+n;
        int xpos=(v%noofcols)*cellw+n;
        return QPoint{xpos,ypos};
    };
    QColor vertexcolor(127, 0, 127);
    QColor edgecoulor(127, 127, 127, 50);




    auto drawMst=[&](auto& current,auto& tree,const auto& pen){
        for(auto& e:graph.edges()){
            const auto& v1=e.either();
            const auto& v2=e.other(e.either());
            auto pt1=row_col(v1,noice[v1]);
            auto pt2=row_col(v2,noice[v2]);

            p->setPen(vertexcolor);
            p->setBrush(vertexcolor);
            p->drawEllipse(pt1,5,5);
//            p->drawText(pt1,QString("%1").arg(v1));
            p->drawEllipse(pt2,5,5);
//            p->drawText(pt2,QString("%1").arg(v2));
            QPen pen(edgecoulor,5);

            p->setPen(pen);
            p->drawLine(pt1,pt2);
        }
        for(auto iter =tree.begin();iter!=current;iter++) {
            auto& e=*iter;
            const auto& v1=e.either();
            const auto& v2=e.other(e.either());
            auto pt1=row_col(v1,noice[v1]);
            auto pt2=row_col(v2,noice[v2]);
            p->setPen(pen);
            p->drawLine(pt1,pt2);

        }
        if(tree.end()!= current){
            current++;
        }

    };
    auto pen=QPen(QPen(Qt::red,3));
    p->setPen(pen);
    drawMst(primIter,primTree, pen);
    p->save();

    p->translate(width()*3/4,height()/4);
    p->drawText(QPoint(0,0),"Primes MST");
    p->restore();
    p->save();
    p->translate(width()/4,height()*3/4);
    pen=QPen(Qt::green,3);
    p->setPen(pen);
    p->drawText(QPoint(0,0),"Krusk MST");
    p->restore();
    p->translate(width()/2,height()/2);
    drawMst(krukIter,krukTree, pen);



}

