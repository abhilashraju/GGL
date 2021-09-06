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
#include "geometricalgorithms.hpp"

#include "rasterwindow.h"
#include <random>
#include <cmath>
using namespace GGL;
class Plane : public RasterWindow
{
    constexpr static int noofcols=10;
    constexpr static int noofrows=10;
public:
    Plane();

protected:
    void timerEvent(QTimerEvent *) override;
    void render(QPainter *p) override;

private:
    void intialiseGraph(auto e1);
    std::vector<QPoint> m_points;
    std::vector<QPoint> contour;
    std::random_device r;
    int m_timerId;

    void mousePressEvent(QMouseEvent *);
};

void Plane::intialiseGraph(auto e1)
{
       std::uniform_int_distribution<int> random_d(0,50);
       QPoint offset{50,100};

       for(int i =0;i<50;i++){
           m_points.push_back(QPoint(random_d(e1),random_d(e1))*10+offset);
       }
       contour =convex_hull(m_points);

}
Plane::Plane()
{
    setTitle("Convex Hull");
    resize(700, 700);

    intialiseGraph(std::default_random_engine{r()});
    m_timerId = startTimer(250);

}
void Plane::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId){
        renderLater();


      
    }
}


void Plane::render(QPainter *p)
{
    QColor vertexcolor(127, 0, 127);
    QColor edgecoulor(127, 0, 0);

    auto drawText=[&](auto rect, auto text){
        p->setFont(QFont("Times New Roman",20));
        p->drawText(rect,text,QTextOption(Qt::AlignCenter));
    };
    auto draw_vertex=[&](auto v,auto pen){
        p->setPen(pen);
        p->setBrush(pen.color());
        p->drawEllipse(v,5,5);
        return  v;
    };
    auto draw_line=[&](auto pt1,auto pt2,auto color){

        p->setBrush(color);
        p->drawLine(draw_vertex(pt1,QPen(color,5)),draw_vertex(pt2,QPen(color,5)));
    };
    drawText(QRect(0,50,width(),50),"Convex Hull");
    for(auto pt:m_points){
        draw_vertex(pt,QPen(vertexcolor));
    }
    for(int i =1;i<contour.size();i++){
       draw_line(contour[i],contour[i-1],edgecoulor);
    }
    if(contour.size()>1)
        draw_line(contour[contour.size()-1],contour[0],edgecoulor);
}
void Plane::mousePressEvent(QMouseEvent *evt)
{
    
    m_points.push_back(QPoint (evt->x(),evt->y()));
    contour=convex_hull(m_points);

}
