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
constexpr int rationr= 7;
constexpr int ratiodr= 8;
class SpGraph : public RasterWindow
{
    constexpr static int noofcols=4;
    constexpr static int noofrows=4;
public:
    SpGraph();

protected:
    void timerEvent(QTimerEvent *) override;
    void render(QPainter *p) override;

private:
    void intialiseGraph(auto e1);
    using Graph=DEdgeWeightedGraph<int>;
    using Edge = Graph::value_type;
    using VertexType = Graph::VertexType;
    DEdgeWeightedGraph<int> graph{1};

    std::map<VertexType,std::deque<VertexType>> short_path_for_vertex;
    std::map<VertexType,std::deque<VertexType>>::iterator short_path_for_vertex_iter;
    std::vector<VertexType> currentshowingpath;
    std::vector<std::vector<VertexType>> currentshowingtree;
    std::map<int,int> noice;
    std::random_device r;
    int m_timerId;
    int current_start{-1};
};
void SpGraph::intialiseGraph(auto e1)
{
    current_start++;
    if(current_start >=noofcols*noofrows || current_start<=0){
        noice.clear();
        auto row_col=[=,cellw=rationr*width()/(ratiodr*noofcols),cellh=rationr*height()/(ratiodr*noofrows)](auto v,auto n){
            int ypos=(v/noofcols)*cellh+n;
            int xpos=(v%noofcols)*cellw+n;
            return std::pair<int,int>{xpos,ypos};
        };
        graph =DEdgeWeightedGraph<int>{noofcols*noofrows};

        constexpr int bottom=noofcols;
        constexpr int top=-noofcols;
        constexpr int left=-1;
        constexpr int right =1;
        constexpr std::array<int,8> dir={left,top-1,top,top+1,right,bottom+1,bottom,bottom-1};
        constexpr std::array<int,5> ledir={top,top+1,right,bottom+1,bottom};
        constexpr std::array<int,5> redir={top,top-1,left,bottom-1,bottom};
        std::uniform_int_distribution<int> random_d(0,7);
        std::uniform_int_distribution<int> random_de(0,4);
        std::uniform_int_distribution<int> random_noice(0,width()/(noofcols));
        auto selectsecond=[&](auto v){
            if(v%noofcols==noofcols-1){
                v=v+redir[random_de(e1)];
            }else if(v%noofcols==0){
                 v=v+ledir[random_de(e1)];
            }else{
                v=v+dir[random_d(e1)];
            }
            return v;
        };
        std::vector<Edge> addedEdge;
        for(size_t i=1; i<graph.size();i++){


            auto v1=DEdgeWeightedGraph<int>::value_type::VertexType(i);
            if(noice.find(v1)==noice.end())
                noice[v1]=random_noice(e1);
                for(int j=0;j<3;j++){
                auto  v2=selectsecond(v1);
                while(v2<1 || v2 >=graph.size()){
                    v2=selectsecond(v1);
                }
                auto iter = std::find_if(addedEdge.begin(),addedEdge.end(),[&](auto v){
                    return (v.from() == v1 || v.to() == v1) && (v.from()==v2 || v.to()==v2);
                });
                if(iter == addedEdge.end()){
                    if(noice.find(v2)==noice.end())
                        noice[v2]=random_noice(e1);
                    auto cell1=row_col(v1,noice[v1]);
                    auto cell2=row_col(v2,noice[v2]);
                    int wt=std::sqrt(std::pow(cell1.first-cell2.first,2) +std::pow(cell1.second-cell2.second,2));
                    graph.addEdge({v1,v2,wt});
                    addedEdge.push_back({v1,v2,wt});
                }

            }

        }
        current_start=1;
    }

    s_s_shortest_path s_p;
    std::map<VertexType,VertexType> paths;
    std::map<VertexType,int> distances;
    s_p(graph,current_start,[&](const auto& v,const auto& distance, const auto& parent){
        paths[v]=parent;distances[v]=distance;

    });
    short_path_for_vertex.clear();
    for(int i=1;i<noofcols*noofrows;i++){
        auto via=paths[i];
        short_path_for_vertex[i].push_front(via);
        while(via!=paths[via]){
            via=paths[via];
            short_path_for_vertex[i].push_front(via);


        }
    }
    short_path_for_vertex_iter=short_path_for_vertex.begin();
    currentshowingtree.clear();

}
SpGraph::SpGraph()
{
    setTitle("Shortest Path");
    resize(700, 700);
    intialiseGraph(std::default_random_engine{r()});
    m_timerId = startTimer(50);

}
void SpGraph::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId){
        renderLater();
        if(short_path_for_vertex.end()== short_path_for_vertex_iter){
            intialiseGraph(std::default_random_engine{r()});
        }
    }
}


void SpGraph::render(QPainter *p)
{


    auto row_col=[=,cellw=rationr*width()/(ratiodr*noofcols),cellh=rationr*height()/(ratiodr*noofrows)](auto v,auto n){

        int ypos=(v/noofcols)*cellh+n;
        int xpos=(v%noofcols)*cellw+n;
        return QPoint{xpos,ypos};
    };

    auto draw_vertex=[&](auto v,auto color){
        auto pt1=row_col(v,noice[v]);
        p->setPen(color);
        p->setBrush(color);
        p->drawEllipse(pt1,5,5);
//            p->drawText(pt1,QString("%1").arg(v1));
        return  pt1;
    };
    auto drawLine=[&](auto pt1,auto pt2,auto w,auto color){
        qreal t=0.05;
        p->setPen(QPen(color,2));
        p->setBrush(color);
        QPointF ept = QPointF(pt1)*t+QPointF(pt2)*(1-t);
        QLineF line(QPointF(pt1),ept);
        p->drawLine(line);
        QPointF diff = pt2-pt1;
        if(diff.x()==0)diff.setX(10);
        QPointF pt[4]={
            ept+QPointF{-10,10},
            ept,
            ept+QPointF{-10,-10},
            ept+QPointF{-10,10}
        };
        qreal slope = diff.y()/diff.x();
        auto angle = std::atan(slope);
        p->save();
        p->translate(ept);
        p->rotate(((diff.x()<0)? 180:0) + angle*180/3.14);
        p->translate(-ept);
        p->drawPolygon(pt,4);
        p->restore();
        p->drawText(line.center(),QString("%1").arg(w));
    };
    auto draw_path=[&](auto& path,auto vertexcolor){
        if(path.size()>1){
            auto iter1 = path.begin();
            auto iter2 = path.begin();
            while(++iter2 != path.end()) {
                auto v1=*iter1;
                auto v2=*iter2;
                drawLine(draw_vertex(v1,vertexcolor),draw_vertex(v2,vertexcolor),graph.find_edge(v1,v2).weight(),Qt::red);
                iter1=iter2;
            }
        }
    };

    auto draw=[&](auto vertexcolor,auto edgecoulor){
       for(auto& e:graph.edges()){
            const auto& v1=e.from();
            const auto& v2=e.to();
            drawLine(draw_vertex(v1,vertexcolor),draw_vertex(v2,vertexcolor),e.weight(),edgecoulor);

        }
        for(auto& path:currentshowingtree){
            draw_path(path,vertexcolor);
        }
        if(short_path_for_vertex_iter != short_path_for_vertex.end()) {
            auto v1 = short_path_for_vertex_iter->second.front();
            currentshowingpath.push_back(v1);
            short_path_for_vertex_iter->second.pop_front();
            draw_path(currentshowingpath,vertexcolor);
        }

        if(short_path_for_vertex_iter->second.empty()){
            if(short_path_for_vertex.end()!= short_path_for_vertex_iter){
                short_path_for_vertex_iter++;
            }
            currentshowingtree.push_back(currentshowingpath);
            currentshowingpath.clear();
        }




    };
    draw(QColor(127, 0, 127), QColor(127, 127, 127));
    draw_vertex(current_start,Qt::green);


}


